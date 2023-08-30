#include "ConditionDialog.h"

ConditionDialog::ConditionDialog(
	wxWindow* parent, 
	wxWindowID id, 
	int n,
	std::vector<std::pair<Conditions, feature>>* pCond,
	const wxString& heading, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name):
	wxDialog(parent, id, heading, pos, size, style, name),
	conditions(pCond),
	number(n)
{
	//this->SetMinSize(size);
	wxString* choices = new wxString[number];
	for (int i = 0; i < number; i++)
	{
		choices[i] = conditions->at(i).second.title;
	}
	
	condList = new wxComboBox(this, wxID_ANY,"Conditions", wxDefaultPosition, wxDefaultSize, number, choices, wxCB_READONLY);
	//title = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	desc = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_BESTWRAP);

	auto sizer = new wxBoxSizer(wxVERTICAL);
	auto flag = wxEXPAND | wxLEFT | wxRIGHT;
	sizer->Add(condList, 0, flag, 15);
	sizer->Add(-1, 5);
	//sizer->Add(title, 0, flag, 5);
	//sizer->Add(-1, 5 );
	sizer->Add(desc, 3, flag, 15);
	sizer->Add(-1, 5);
	sizer->Add(CreateButtonSizer(wxCLOSE), 0, wxALIGN_RIGHT | wxLEFT | wxRIGHT, 15);
	sizer->Add(-1, 5);

	condList->Select(1);
	desc->SetValue(conditions->at(1).second.description);
	SetSizer(sizer);
	Layout();

	condList->Bind(wxEVT_COMBOBOX, &ConditionDialog::onSelect, this);

	delete[] choices;
}

void ConditionDialog::onSelect(wxCommandEvent& event)
{
	std::string str = condList->GetValue().ToStdString();
	
	for (auto it = conditions->begin(); it != conditions->end(); ++it)
	{
		if (str == it->second.title)
		{
			//title->SetValue(it->second.title);
			desc->SetValue(it->second.description);
		}
	}
}
