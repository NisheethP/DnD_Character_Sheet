#pragma once

#include "Character.h"

class ConditionDialog : public wxDialog
{
	const int number;
	std::vector<std::pair<Conditions, feature>>* conditions;
	wxComboBox* condList = nullptr;
	//wxTextCtrl* title = nullptr;
	wxTextCtrl* desc = nullptr;

public:
	ConditionDialog(wxWindow* parent,
		wxWindowID 	id,
		int n,
		std::vector<std::pair<Conditions, feature>>* pCond,
		const wxString& heading = "Add Feature",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	void onSelect(wxCommandEvent& event);
	wxComboBox* getComboBox() { return condList; }
	wxTextCtrl* getDesc() { return desc; }

	int setSelection(int x);
	int setSelection(std::string str);
};

