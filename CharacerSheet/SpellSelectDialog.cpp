#include "SpellSelectDialog.h"

SpellSelectDialog::SpellSelectDialog(
	wxWindow* parent, 
	wxWindowID id,
	int pnum,
	wxString* pchoices,
	const wxString& heading, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name):
	wxDialog(parent, id, heading, pos, size, style, name),
	num(pnum),
	choices(pchoices)

{
	//auto panel = new wxPanel(this, wxID_ANY);
	auto sizer = new wxBoxSizer(wxVERTICAL);
	auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);

	search = new wxSearchCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, num, choices, wxLB_SINGLE);

	auto sizerFlag = wxEXPAND | wxALL;
	int margin = 10;

	sizer->Add(search, 0, sizerFlag, margin);
	sizer->Add(-1, 10);
	sizer->Add(list, 1, sizerFlag, margin);
	sizer->Add(-1, 10);
	sizer->Add(buttonSizer, 0, wxALIGN_RIGHT);
	sizer->Add(-1, 10);

	this->SetSizerAndFit(sizer);
	this->Layout();

	search->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &SpellSelectDialog::onSearch, this);
	search->Bind(wxEVT_SEARCH, &SpellSelectDialog::onSearchEnter, this);
	search->Bind(wxEVT_TEXT, &SpellSelectDialog::onSearchType, this);
}

std::string SpellSelectDialog::getSelection()
{
	int x = list->GetSelection();
	
	if (x != wxNOT_FOUND)
	{
		std::string str = list->GetString(x).ToStdString();
		return str;
	}

	return "";	
}

void SpellSelectDialog::onSearch(wxCommandEvent& event)
{
	auto str = event.GetString();
	int x = list->FindString(str);

	if (x != wxNOT_FOUND)
	{
		list->SetSelection(x);
		list->EnsureVisible(x);
	}
}

void SpellSelectDialog::onSearchType(wxCommandEvent& event)
{
	auto string = search->GetValue();
	int x = list->FindString(string);
	list->SetSelection(x);

	if (x != wxNOT_FOUND)
	{
		list->EnsureVisible(x);
	}
	
	auto tempList = list;
	auto str = string.ToStdString();

	if (str == "")
	{
		populateList();
		return;
	}

	/*for (int i = 0; i < list->GetCount(); ++i)
		list->Delete(i);*/

	list->Clear();


	for (int j = 0; j < num; ++j)
	{
		bool temp = false;

		if (str.length() > choices[j].length())
			continue;

		std::string tempStrs = choices[j].ToStdString();
		
		for (auto it = tempStrs.begin(); it != tempStrs.end(); ++it)
			*it = std::tolower(*it);

		for (auto it = str.begin(); it != str.end(); ++it)
			*it = std::tolower(*it);

		if (tempStrs.find(str) != std::string::npos)
			list->Append(choices[j]);
	}

}

void SpellSelectDialog::onSearchEnter(wxCommandEvent& event)
{
	auto str = search->GetValue();
	int x = list->FindString(str);
	list->SetSelection(x);

	//this->SetReturnCode();
	this->EndModal(wxID_OK);
}

void SpellSelectDialog::populateList()
{
	list->Clear();

	for (int i = 0; i < num; ++i)
	{
		list->Append(choices[i]);
	}
}
