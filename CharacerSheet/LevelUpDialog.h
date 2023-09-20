#pragma once

#include "CharClass.h"
#include "Character.h"

class LevelUpDialog : public wxDialog
{
	
	wxComboBox* classCombo = nullptr;
	wxComboBox* classCasterType = nullptr;
	wxSpinCtrl* HP_gained = nullptr;

	wxSpinCtrl* Stat[6];
public:
	LevelUpDialog(wxWindow* parent,
		wxWindowID	id = wxID_ANY,
		const wxString& heading = "Dice Roller",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	const int getStat(Skills pStat) const;
	const int getStat(int index) const;

	const CharClass getClass() const;
	const int getExtraHP() const { return HP_gained->GetValue(); }
};

