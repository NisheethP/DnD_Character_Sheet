#include "PreCompiledHeader.h"
#include "InventoryControl.h"

#include "Util.h"

InventoryControl::InventoryControl(
	wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) :
	wxPanel(parent, id, pos, size, style, name)
{
	list = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL);

	list->AppendColumn("Item");
	list->AppendColumn("Count");
	list->AppendColumn("Weight");
	list->AppendColumn("Total Weight");

	list->SetColumnWidth(0, size.x * 0.55);
	list->SetColumnWidth(1, size.x * 0.15);
	list->SetColumnWidth(2, size.x * 0.15);
	list->SetColumnWidth(3, size.x * 0.15);

	auto sizer = new wxBoxSizer(wxVERTICAL);
	auto titleSizer = new wxBoxSizer(wxHORIZONTAL);
	auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	wxSize bSize(40, -1);
	Add = new wxButton(this, wxID_ANY, "", wxDefaultPosition, bSize);
	Rem = new wxButton(this, wxID_ANY, "", wxDefaultPosition, bSize);

	Add->SetBitmap(wxArtProvider().GetBitmap(wxART_PLUS, wxART_BUTTON));
	Rem->SetBitmap(wxArtProvider().GetBitmap(wxART_MINUS, wxART_BUTTON));

	title = new wxStaticText(this, wxID_ANY, "Inventory", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	title->SetFont(title->GetFont().MakeBold().MakeLarger());
	title->SetForegroundColour(*wxWHITE);

	auto titleLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxLI_VERTICAL);

	buttonSizer->Add(Rem, 0, wxALL, 3);
	buttonSizer->Add(Add, 0, wxALL, 3);

	weight = new wxTextCtrl(this, wxID_ANY, "Weight : 0", wxDefaultPosition, wxDefaultSize, wxHTML_ALIGN_CENTER | wxTE_READONLY);
	weight->SetBackgroundColour(wxColour(0x28, 0x28, 0x2c));
	weight->SetForegroundColour(wxColour(0xDD, 0xDD, 0xDD));
	weight->SetFont(weight->GetFont().MakeBold().MakeLarger());

	titleSizer->Add(title, 0, wxEXPAND | wxRIGHT | wxLEFT, 10);
	titleSizer->Add(titleLine, 0, wxEXPAND | wxLEFT | wxRIGHT, 3);
	titleSizer->Add(weight, 0, wxEXPAND | wxLEFT, 3);
	titleLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxLI_VERTICAL);
	titleSizer->Add(titleLine, 0, wxEXPAND | wxLEFT | wxRIGHT, 3);
	titleSizer->Add(buttonSizer, 0, wxLEFT, 5);
	
	auto searchSizer = new wxBoxSizer(wxHORIZONTAL);

	search = new wxSearchCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	searchType = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, {"Name", "Tags"});

	searchType->SetSelection(0);
	
	searchSizer->Add(search, 1, wxEXPAND | wxRIGHT, 5);
	searchSizer->Add(searchType, 0);

	sizer->Add(titleSizer, 0, wxEXPAND | wxRIGHT, 5);
	sizer->Add(searchSizer, 0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(list, 1, wxEXPAND | wxTOP, 5);

	SetSizer(sizer);
	Layout();

	Add->Bind(wxEVT_BUTTON, &InventoryControl::onAddButton, this);
	Rem->Bind(wxEVT_BUTTON, &InventoryControl::onRemButton, this);
	list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &InventoryControl::onListDClick, this);
	search->Bind(wxEVT_TEXT, &InventoryControl::onSearchType, this);
	Layout();
}

void InventoryControl::onAddButton(wxCommandEvent& event)
{
	AddDialog dialog(this, wxID_ANY, "Add Item");

	if (dialog.ShowModal() == wxID_CANCEL)
		return;

	updateItem(dialog);

	dialog.Destroy();
}

void InventoryControl::onRemButton(wxCommandEvent& event)
{
	long sel = -1;
	for (;; )
	{
		sel = list->GetNextItem(sel,
			wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
		if (sel == -1)
			break;
		// this item is selected - do whatever is needed with it
		
		for (auto i = items.begin(); i != items.end(); ++i)
		{
			if (i->getName() == list->GetItemText(sel, 0))
				if (std::to_string(i->getCount()) == list->GetItemText(sel, 1))
					if (std::to_string(i->getWeight()) == list->GetItemText(sel, 2))
					{
						i = items.erase(i);
						break;
					}
		}

		list->DeleteItem(sel);
	}

	updateAllTags();
}

void InventoryControl::onListDClick(wxListEvent& event)
{
	long sel = -1;
	for (;; )
	{
		sel = list->GetNextItem(sel,
			wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
		if (sel == -1)
			break;
		// this item is selected - do whatever is needed with it
		AddDialog dialog(this, wxID_ANY, list->GetItemText(sel,0).ToStdString());

		for (auto& i : items)
		{
			std::string curWeight = std::format("{:.2f}", i.getWeight());
			std::string curCount = std::to_string(i.getCount());

			if (i.getName() != list->GetItemText(sel, 0).ToStdString() ||
				curCount != list->GetItemText(sel, 1).ToStdString() ||
				curWeight != list->GetItemText(sel, 2).ToStdString())
						continue;
			
			dialog.setDialog(i.getName(),i.getDescription(), i.getWeight(),i.getCount(), i.getAttunement(), &i.getTags());
			break;
		}

		if (dialog.ShowModal() == wxID_CANCEL)
			return;

		updateItem(dialog, sel);
	}
	
}

void InventoryControl::onSearchType(wxCommandEvent& event)
{
	displayEntries.clear();
	int filter = searchType->GetSelection();
	
	if (filter == 0)
	{
		for (int it = 0; it < items.size(); ++it)
		{
			if (isSearchable(items[it].getName()))
				addToDisplayedList(it);
		}
	}

	if (filter == 1)
	{
		for (int it = 0; it < allTags.size(); ++it)
		{
			if (isSearchable(allTags[it]))
			{
				for (int j = 0; j < items.size(); ++j)
					if (items[j].hasTag(allTags[it]))
						addToDisplayedList(j);
			}
		}
	}

	fillList();
}

void InventoryControl::addItem(Item inv)
{
	for (auto& i : items)
	{
		if (i == inv)
			return;
	}

	items.push_back(inv);

}

void InventoryControl::setInventory(std::vector<Item> inv)
{
	items = inv;
}

void InventoryControl::updateItem(AddDialog& dialog, int curItem)
{
	Item tempItem;
	tempItem.setName(dialog.getName());
	tempItem.setDescription(dialog.getDescription());
	tempItem.setAttunement(dialog.getAttunement());
	tempItem.setCount(dialog.getCount());
	tempItem.setWeight(dialog.getWeight());

	wxArrayString tagStrings;
	dialog.getTags()->GetStrings(tagStrings);

	for (auto& i : tagStrings)
		tempItem.addTag(i.ToStdString());

	if (curItem < 0 || curItem > items.size())
		items.push_back(tempItem);
	else
		items[curItem] = tempItem;

	int totalWeight = 0;
	for (auto& it : items)
		totalWeight += it.getTotalWeight();

	int numItems = items.size() - 1;
	if (isSearchable(items[numItems].getName()))
		addToDisplayedList(numItems);

	fillList();

	weight->SetValue("Weight : " + std::to_string(totalWeight));

	updateAllTags();
}

void InventoryControl::fillList()
{
	list->DeleteAllItems();
	int j = 0;
	for (int i = 0; i < items.size(); ++i)
	{
		if (isDisplayed(i))
		{
			std::string curWeight = std::format("{:.2f}",items[i].getWeight());
			std::string curTotWeight = std::format("{:.2f}",items[i].getTotalWeight());
			
			list->InsertItem(j, items[i].getName());
			list->SetItem(j, 1, std::to_string(items[i].getCount()));
			list->SetItem(j, 2, curWeight);
			list->SetItem(j, 3, curTotWeight);
			++j;

		}
	}
}

bool InventoryControl::isDisplayed(int index)
{
	for (auto& i : displayEntries)
		if (i == index)
			return true;

	return false;
}

void InventoryControl::addToDisplayedList(int x)
{
	for (auto& it : displayEntries)
	{
		if (it == x)
			return;
	}

	displayEntries.push_back(x);
	std::sort(displayEntries.begin(), displayEntries.end());
}

bool InventoryControl::isSearchable(std::string entry)
{
	std::string searchEntry = search->GetValue().ToStdString();

	if (searchEntry == "")
		return true;

	Util::toLowerString(searchEntry);
	Util::toLowerString(entry);
	if (entry.find(searchEntry) != std::string::npos)
		return true;
	
	return false;
}

void InventoryControl::resizeCtrl()
{
	auto size = this->GetMinSize();
	list->SetColumnWidth(0, size.x * 0.55);
	list->SetColumnWidth(1, size.x * 0.15);
	list->SetColumnWidth(2, size.x * 0.15);
	list->SetColumnWidth(3, size.x * 0.15);

	title->SetMinSize(wxSize(size.x - 2 * (40 + 5 + 6 + 10 + 5) - 5 - 3 - weight->GetSize().x*2, -1));
	weight->SetMinSize(weight->GetSize() * 2);
	Layout();
}

bool InventoryControl::hasTag(std::string pTag)
{
	for (auto& it : allTags)
		if (it == pTag)
			return true;

	return false;
}

void InventoryControl::updateAllTags()
{
	std::set<std::string> tempTags;

	allTags.clear();

	for (auto& it : items)
	{
		auto& curTags = it.getTags();
		for (auto& iTag : curTags)
			tempTags.insert(iTag);
	}

	for (auto& it : tempTags)
		allTags.push_back(it);
}

void InventoryControl::updateList()
{
	for (int i = 0; i < items.size(); ++i)
		displayEntries.push_back(i);

	fillList();
}


InventoryControl::AddDialog::AddDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
	wxDialog(parent, id, title, pos, size, style, name)
{
	openDialog("", "", 0, 1, false, nullptr);
}

void InventoryControl::AddDialog::openDialog(std::string pItemName, std::string pItemDesc, float pWeight, int pCount, bool pAttunement, std::vector<std::string>* pTags)
{
	int gap = 3;
	auto sizer = new wxBoxSizer(wxVERTICAL);

	auto horSizer = new wxBoxSizer(wxHORIZONTAL);
	auto text = new wxStaticText(this, wxID_ANY, "Name");
	itemName = new wxTextCtrl(this, wxID_ANY, pItemName);

	//ROW 1 
	{
		horSizer->Add(text, 0, wxALIGN_CENTER | wxRIGHT, gap);
		horSizer->Add(10, -1);
		horSizer->Add(itemName, 1, wxEXPAND, gap);
		sizer->Add(horSizer, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 3 * gap);
	}

	auto Line = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	Line->SetMinSize(wxSize(-1, 3));
	sizer->Add(Line, 0, wxEXPAND | wxTOP, gap);

	//ROW 2
	{
		horSizer = new wxBoxSizer(wxHORIZONTAL);
		attunement = new wxCheckBox(this, wxID_ANY, "Attunement", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
		attunement->SetValue(pAttunement);
		horSizer->Add(attunement, 0, wxALIGN_CENTER | wxTOP | wxRIGHT, gap);

		Line = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
		Line->SetMinSize(wxSize(3, -1));
		horSizer->Add(Line, 0, wxEXPAND | wxLEFT | wxRIGHT, gap);

		text = new wxStaticText(this, wxID_ANY, "Weight");
		weight = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_CENTER_HORIZONTAL, 0, 1e5, pWeight, 0.1);
		horSizer->Add(text, 0, wxALIGN_CENTER);
		horSizer->Add(weight, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, gap);

		Line = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
		Line->SetMinSize(wxSize(3, -1));
		horSizer->Add(Line, 0, wxEXPAND | wxLEFT | wxRIGHT, gap);

		text = new wxStaticText(this, wxID_ANY, "Quantity");
		count = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_CENTER_HORIZONTAL, 0, 1e5, pCount);
		horSizer->Add(text, 0, wxALIGN_CENTER);
		horSizer->Add(count, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, gap);

		sizer->Add(horSizer, 0, wxALIGN_LEFT | wxTOP | wxLEFT | wxRIGHT, gap);

		Line = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
		Line->SetMinSize(wxSize(-1, 3));
		sizer->Add(Line, 0, wxEXPAND | wxTOP, 3*gap);
	}

	//ROW 3
	{
		horSizer = new wxBoxSizer(wxHORIZONTAL);
		auto descSizer = new wxBoxSizer(wxVERTICAL);
		text = new wxStaticText(this, wxID_ANY, "Description");
		description = new wxTextCtrl(this, wxID_ANY, pItemDesc, wxDefaultPosition, wxDefaultSize, wxTE_RICH2 | wxTE_LEFT | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB | wxTE_MULTILINE);

		descSizer->Add(text, 0);
		descSizer->Add(description, 1, wxEXPAND);

		horSizer->Add(descSizer, 4, wxEXPAND);
		
		tags = new wxEditableListBox(this, wxID_ANY, "Tags", wxDefaultPosition, wxDefaultSize, wxEL_DEFAULT_STYLE);

		horSizer->Add(tags, 3, wxLEFT, gap);
		sizer->Add(horSizer, 0, wxTOP | wxLEFT | wxRIGHT, gap);
	}

	if (pTags != nullptr)
	{
		wxArrayString tagsNames;

		for (auto& i : *pTags)
			tagsNames.push_back(i);

		tags->SetStrings(tagsNames);
	}	

	itemName->SetFocus();
	itemName->SetSelection(-1, -1);

	Line = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	Line->SetMinSize(wxSize(-1, 3));
	sizer->Add(Line, 0, wxEXPAND | wxTOP, gap);

	sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, gap);
	sizer->Add(-1, gap);

	SetSizerAndFit(sizer);
	Layout();
}

void InventoryControl::AddDialog::setDialog(std::string pItemName, std::string pItemDesc, float pWeight, int pCount, bool pAttunement, std::vector<std::string>* pTags)
{
	itemName->SetValue(pItemName);
	description->SetValue(pItemDesc);
	weight->SetValue(pWeight);
	count->SetValue(pCount);
	attunement->SetValue(pAttunement);

	if (pTags != nullptr)
	{
		wxArrayString tagsNames;

		for (auto& i : *pTags)
			tagsNames.push_back(i);

		tags->SetStrings(tagsNames);
	}
}
