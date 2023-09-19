#include "PreCompiledHeader.h"
#include "LevelUpDialog.h"

LevelUpDialog::LevelUpDialog(
	wxWindow* parent,
	wxWindowID id,
	const wxString& heading,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) 
	:
	wxDialog(parent, id, heading, pos, size, style, name)
{
	std::vector<std::string> choicesVector;
	for (int i = 0; i != static_cast<int>(ClassType::none); ++i)
	{
		ClassType type = static_cast<ClassType>(i);
		choicesVector.push_back(getStringFromClass(type));
	}
	
	wxString* choices = new wxString[choicesVector.size()];

	std::copy(choicesVector.begin(), choicesVector.end(), choices);

	auto classText = new wxStaticText(this, wxID_ANY, "Class: ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	classCombo = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, choicesVector.size(), choices);
	delete[] choices;
	classCombo->SetSelection(0);

	choices = new wxString[4];
	choices[0] = "None";
	choices[1] = "One-Third Caster";
	choices[2] = "Half Caster";
	choices[3] = "Full Caster";

	auto casterTypeText = new wxStaticText(this, wxID_ANY, "Caster Type: ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	classCasterType = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 4, choices);
	delete[] choices;
	classCasterType->SetSelection(0);

	auto HP_Text = new wxStaticText(this, wxID_ANY, "HP Gained (without Mod): ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	HP_gained = new wxSpinCtrl(this, wxID_ANY, "0");

	int gap = 3;
	auto sizer = new wxBoxSizer(wxVERTICAL);
	auto SubSizer = new wxGridSizer(4, 2, gap, gap);
	
	SubSizer->Add(classText, 0, wxEXPAND);
	SubSizer->Add(classCombo, 0, wxEXPAND);
	SubSizer->Add(casterTypeText, 0, wxEXPAND);
	SubSizer->Add(classCasterType, 0, wxEXPAND);
	SubSizer->Add(HP_Text, 0, wxEXPAND);
	SubSizer->Add(HP_gained, 0, wxEXPAND);

	sizer->Add(SubSizer, 1, wxEXPAND | wxALL, 2*gap);
	SetSizerAndFit(sizer);
	Layout();
}
