#pragma once

#include <wx/wx.h>

class DeathSavesControl : public wxWindow
{
public:
	DeathSavesControl(
		wxWindow* parent,
		wxWindowID 	id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr
	);

	virtual ~DeathSavesControl() {}

private:
	void OnPaint(wxPaintEvent& event);
	int thickness = 10;
};

