#pragma once

class SliderRemDialog : public wxDialog
{
	wxListView* list = nullptr;
	std::vector<std::pair<wxStaticText*, wxSlider*>>& slider;

public:
	SliderRemDialog(wxWindow* parent,
		wxWindowID 	id,
		std::vector<std::pair<wxStaticText*, wxSlider*>> pSlider,
		const wxString& heading = "Add Feature",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	long getSelected();
};

