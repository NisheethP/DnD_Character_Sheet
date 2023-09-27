#pragma once
#include "Character.h"

class SkillProficiencyModifierDialog : public wxDialog
{
	std::unordered_map<Skills, wxSpinCtrl*> modValues;
public:
	SkillProficiencyModifierDialog(wxWindow* parent,
		wxWindowID	id,
		std::vector<Skills> pSkills,
		const wxString& heading = "Dice Roller",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);
};

