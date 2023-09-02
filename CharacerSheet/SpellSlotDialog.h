#pragma once

#include <wx/wx.h>

class SpellSlotDialog : public wxDialog
{
	int slots[9] = { 0,0,0,0,0,0,0,0,0 };
public:
	SpellSlotDialog(wxWindow* parent,
		wxWindowID	id = wxID_ANY,
		const wxString& heading = "Add Feature",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	int getSlots(int i);
};

