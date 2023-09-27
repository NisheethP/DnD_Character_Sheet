#include "PreCompiledHeader.h"
#include "SkillProficiencyModifierDialog.h"

SkillProficiencyModifierDialog::SkillProficiencyModifierDialog(
	wxWindow* parent,
	wxWindowID id,
	std::vector<Skills> pSkills,
	std::unordered_map<Skills, int> pOriginalMods,
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

	auto sizer = new wxFlexGridSizer(choicesVector.size(), 2, 2, 5);

	if (choicesVector.size() > 6)
		sizer->SetCols(4);
	auto mainSizer = new wxBoxSizer(wxVERTICAL);

	modValues.reserve(choicesVector.size());

	for (int i = 0; i < choicesVector.size(); ++i)
	{
		auto text = new wxStaticText(this, wxID_ANY, choicesVector[i], wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
		modValues[pSkills[i]] = new wxSpinCtrl(this, wxID_ANY, std::to_string(pOriginalMods[pSkills[i]]));

		sizer->Add(text, 0, wxALIGN_RIGHT);
		sizer->Add(modValues[pSkills[i]]);
	}

	mainSizer->Add(sizer, 0, wxALL, 10);
	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
	SetSizerAndFit(mainSizer);
}
