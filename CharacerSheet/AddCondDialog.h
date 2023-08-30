#pragma once

#include <wx/wx.h>
#include "Character.h"

class AddCondDialog : public wxDialog
{
	const int number;
	std::vector<std::pair<Conditions, feature>>* conditions;
	wxCheckListBox* list;
public:
	AddCondDialog(wxWindow* parent,
		wxWindowID 	id,
		int n,
		std::vector<std::pair<Conditions, feature>>* pConds,
		const wxString& heading = "Add Feature",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	wxCheckListBox* getList() { return list; }
};

