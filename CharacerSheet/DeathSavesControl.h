#pragma once

#include <wx/wx.h>
#include <wx/radiobut.h>
#include <vector>

class DeathSavesControl : public wxWindow
{

	wxRadioButton* success[3];
	wxRadioButton* fails[3];
	wxButton* reset;

	wxColour bgColour;
	wxColour fgColour;

public:
	DeathSavesControl(
		wxWindow* parent,
		wxWindowID 	id,
		wxColour fg,
		wxColour bg,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr
	);

	void SetFGColour(wxColour c) { fgColour = c; }
	void onReset(wxCommandEvent& event);
};

