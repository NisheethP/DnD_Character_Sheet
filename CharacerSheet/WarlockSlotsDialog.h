#pragma once

class WarlockSlotsDialog :
    public wxDialog
{
	int numSlots = 0;
	int level = 0;
public:
    WarlockSlotsDialog(wxWindow* parent,
		wxWindowID	id = wxID_ANY,
		const wxString& heading = "Warlock Slots",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	int getNumSlots() 
	{ 
		TransferDataFromWindow(); 
		return numSlots; 
	}

	int getLevel() 
	{ 
		TransferDataFromWindow();
		return level;
	}
};

