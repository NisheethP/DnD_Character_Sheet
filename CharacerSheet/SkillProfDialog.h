#pragma once

#include <wx/wx.h>
#include "Character.h"

class SkillProfDialog : public wxDialog
{
	std::vector<Skills> skillList;
	wxCheckListBox* list;
	int checks;
public:
	SkillProfDialog(wxWindow* parent,
		wxWindowID	id,
		std::vector<Skills> pList,
		int checks,
		const wxString& heading = "Select profeciencies",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	wxCheckListBox* getList() { return list; }
};

