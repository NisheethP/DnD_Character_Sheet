#pragma once

class FeatureDialog : public wxDialog
{

	wxTextCtrl* title = nullptr;
	wxTextCtrl* description = nullptr;


public:
	FeatureDialog(wxWindow* parent,
		wxWindowID	id = wxID_ANY,
		const wxString& heading = "Add Feature",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	std::string getTitle() { return title->GetValue().ToStdString(); }
	std::string getDescription() { return description->GetValue().ToStdString(); }
};

