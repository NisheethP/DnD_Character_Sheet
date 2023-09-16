#include "PreCompiledHeader.h"

#include "SkillProfDialog.h"


SkillProfDialog::SkillProfDialog(
	wxWindow* parent, 
	wxWindowID id, 
	std::vector<Skills> pList,
	int checks,
	const wxString& heading, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name):
	wxDialog(parent, id, heading, pos, size, style, name)
{
	auto sizer = new wxBoxSizer(wxVERTICAL);

	int num = pList.size();
	wxString* choices = new wxString[num];

	for (int i = 0; i < num; ++i)
		choices[i] = getSkillStr(pList[i]);

	list = new wxCheckListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, num, choices);

	int margin = 5;
	sizer->Add(list, 0, wxEXPAND | wxLEFT | wxRIGHT, margin);
	sizer->Add(-1, 5);
	sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_RIGHT | wxLEFT | wxRIGHT, margin);
	sizer->Add(-1, 5);

	for (int i = 0; i < num; ++i)
	{
		if (pList[i] & checks)
			list->Check(i);
	}
	
	SetSizerAndFit(sizer);
	Layout();
	delete[] choices;
}
