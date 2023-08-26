#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>

class SliderDialog : public wxDialog
{
	wxTextCtrl* title = nullptr;
	wxSpinCtrl* min = nullptr;
	wxSpinCtrl* max = nullptr;
	wxSpinCtrl* def = nullptr;

public:
	SliderDialog(wxWindow* parent,
		wxWindowID 	id = wxID_ANY,
		const wxString& heading = "Add Feature",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	std::string getTitle() { return title->GetValue().ToStdString(); };
	int getMin() { return min->GetValue(); }
	int getMax() { return max->GetValue(); }
	int getDef() { return def->GetValue(); }

	void onSpinChange(wxSpinEvent& event);
};

