#include "PreCompiledHeader.h"

#include "AddCondDialog.h"

AddCondDialog::AddCondDialog(
	wxWindow* parent, 
	wxWindowID id, 
	int n,
	std::vector<std::pair<Conditions, feature>>* pConds,
	const wxString& heading, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name):

	wxDialog(parent, id, heading, pos, size, style, name), 
	conditions(pConds),
	number(n)
{
	wxString* choices = new wxString[number];
	for (int i = 0; i < number; i++)
	{
		choices[i] = conditions->at(i).second.title;
	}

	list = new wxCheckListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, number, choices);
	auto sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(list, 1, wxEXPAND | wxLEFT | wxRIGHT, 5);
	sizer->Add(-1, 10);
	sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_RIGHT | wxLEFT | wxRIGHT, 5);
	sizer->Add(-1, 10);

	SetSizer(sizer);
	Layout();
}
