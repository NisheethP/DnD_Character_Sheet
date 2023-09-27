#include "PreCompiledHeader.h"
#include "SkillProficiencyModifierDialog.h"

SkillProficiencyModifierDialog::SkillProficiencyModifierDialog(
	wxWindow* parent,
	wxWindowID id,
	std::vector<Skills> pSkills,
	const wxString& heading,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name)
	:
	wxDialog(parent, id, heading, pos, size, style, name)
{
	std::vector<std::string> choicesVector;
	choicesVector.reserve(pSkills.size());
	for (auto& i : pSkills)
		choicesVector.emplace_back(getSkillStr(i));

	/*auto choices = new wxString[choicesVector.size()];
	std::copy(choicesVector.begin(), choicesVector.end(), choices);*/

	auto sizer = new wxFlexGridSizer(choicesVector.size(), 2, 2, 5);
	auto mainSizer = new wxBoxSizer(wxVERTICAL);

	modValues.reserve(choicesVector.size());

	for (int i = 0; i < choicesVector.size(); ++i)
	{
		auto text = new wxStaticText(this, wxID_ANY, choicesVector[i], wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
		modValues[pSkills[i]] = new wxSpinCtrl(this, wxID_ANY, "0");

		sizer->Add(text, 0, wxALIGN_RIGHT);
		sizer->Add(modValues[pSkills[i]]);
	}

	mainSizer->Add(sizer, 0, wxALL, 10);
	SetSizerAndFit(mainSizer);
}
