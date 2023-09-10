#include "AttackControl.h"
#include "DiceRollerDialog.h"

#include <wx/artprov.h>

AttackControl::AttackControl(
	wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) :
	wxPanel(parent, id, pos, size, style, name)
{
	list = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL);

	list->AppendColumn("NAME");
	list->AppendColumn("ATK MOD");
	list->AppendColumn("DMG");
	list->AppendColumn("TYPE");

	list->SetColumnWidth(0, size.x*.4);
	list->SetColumnWidth(1, size.x*.15);
	list->SetColumnWidth(2, size.x*.2);
	list->SetColumnWidth(3, size.x*.3);

	auto sizer = new wxBoxSizer(wxVERTICAL);
	auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	wxSize bSize(40, -1);
	Add = new wxButton(this, wxID_ANY, "", wxDefaultPosition, bSize);
	Rem = new wxButton(this, wxID_ANY, "", wxDefaultPosition, bSize);

	Add->SetBitmap(wxArtProvider().GetBitmap(wxART_PLUS, wxART_BUTTON));
	Rem->SetBitmap(wxArtProvider().GetBitmap(wxART_MINUS, wxART_BUTTON));

	buttonSizer->Add(Rem, 0, wxALL, 3);
	buttonSizer->Add(Add, 0, wxALL, 3);

	sizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxRIGHT, 5);
	sizer->Add(list, 1, wxEXPAND);
	
	SetSizer(sizer);
	Layout();

	Add->Bind(wxEVT_BUTTON, &AttackControl::onAddButton, this);
	Rem->Bind(wxEVT_BUTTON, &AttackControl::onRemButton, this);
	list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &AttackControl::onListDClick, this);
}

void AttackControl::onAddButton(wxCommandEvent& event)
{
	AddDialog dialog(this, wxID_ANY, "");

	if (dialog.ShowModal() == wxID_CANCEL)
		return;

	std::string name = dialog.getName();
	std::string mod = dialog.getInput(1);
	std::string dmg = dialog.getInput(2) + dialog.getDieType();
	std::string dmgType = dialog.getDmgType();

	int x = list->GetItemCount();
	list->InsertItem(x, name);
	list->SetItem(x, 1, mod);
	list->SetItem(x, 2, dmg);
	list->SetItem(x, 3, dmgType);
}

void AttackControl::onRemButton(wxCommandEvent& event)
{
	long item = -1;
	for (;; )
	{
		item = list->GetNextItem(item,
			wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
		if (item == -1)
			break;
		// this item is selected - do whatever is needed with it
		list->DeleteItem(item);
	}
}

void AttackControl::onListDClick(wxListEvent& event)
{
	long item = -1;
	for (;; )
	{
		item = list->GetNextItem(item,
			wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
		if (item == -1)
			break;
		
		// this item is selected
		std::string DMG = list->GetItemText(item, 2).ToStdString();
		int num = 0;
		int numLen = 0;
		for (auto it = DMG.begin(); it != DMG.end(); ++it)
		{
			if (numLen != 0)
				num = std::stoi(DMG.substr(0, numLen));

			if (*it == 'd')
				break;

			numLen++;
		}
		std::string dice = DMG.substr(numLen);
		int mod = std::stoi(list->GetItemText(item, 1).ToStdString());

		auto dialog = new DiceRollerDialog(this);

		dialog->addDiceRow(num, dice, mod);
		dialog->Show();
	}
}

AttackControl::AddDialog::AddDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
	wxDialog(parent, id, title, pos, size, style, name)
{
	int gap = 3;
	auto sizer = new wxBoxSizer(wxVERTICAL);
	std::string names[] = {
		"Name",
		"Attack Bonus",
		"Damage",
		"Type"
	};

	std::string defVal[] =
	{
		"Dagger",
		"0",
		"1",
		"Piercing"
	};

	for (int i = 0; i < 4; ++i)
	{
		auto horSizer = new wxBoxSizer(wxHORIZONTAL);
		auto text = new wxStaticText(this, wxID_ANY, names[i]);
		auto& val = input[i] = new wxTextCtrl(this, wxID_ANY, defVal[i]);

		if (names[i] == "Attack Bonus")
			val->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
		
		horSizer->Add(text);
		horSizer->Add(10, -1);
		horSizer->Add(val);
		
		if (names[i] == "Damage")
		{
			val->SetValidator(wxTextValidator(wxFILTER_DIGITS));
			
			const int NUM_DICE = 7;
			wxString choices[NUM_DICE] =
			{
				"d4",
				"d6",
				"d8",
				"d10",
				"d12",
				"d20",
				"d100",
			};

			auto& combo = diceType = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, NUM_DICE, choices);
			combo->SetSelection(0);
			wxSize tempSize = input[0]->GetSize();
			tempSize.x -= combo->GetSize().x;
			val->SetMaxSize(wxSize(tempSize.x,-1));
			horSizer->Add(combo);
		}

		sizer->Add(horSizer, 0, wxALIGN_RIGHT | wxTOP | wxLEFT | wxRIGHT, gap);
	}
	
	input[0]->SetSelection(-1, -1);
	input[0]->SetFocus();

	sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, gap);
	sizer->Add(-1, gap);

	SetSizerAndFit(sizer);
	Layout();
}

std::string AttackControl::AddDialog::getInput(int index)
{
	switch (index)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		return input[index]->GetValue().ToStdString();
	default:
		return "";
	}
}
