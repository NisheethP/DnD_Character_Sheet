#pragma once

#include "CharClass.h"

class LevelUpDialog : public wxDialog
{
	CharClass charClass;
	int extraHP;
	
	wxComboBox* classCombo = nullptr;
	wxComboBox* classCasterType = nullptr;
	wxSpinCtrl* HP_gained = nullptr;
public:
	LevelUpDialog(wxWindow* parent,
		wxWindowID	id = wxID_ANY,
		const wxString& heading = "Dice Roller",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);
};

