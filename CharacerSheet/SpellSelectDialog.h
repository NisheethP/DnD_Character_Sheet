#pragma once

class SpellSelectDialog : public wxDialog
{
	wxSearchCtrl* search = nullptr;
	wxListBox* list = nullptr;

	wxString* choices = nullptr;
	int num = 0;

public:
	SpellSelectDialog(wxWindow* parent,
		wxWindowID 	id,
		int num,
		wxString* choices,
		const wxString& heading = "Add Feature",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	std::string getSelection();

	void onSearch(wxCommandEvent& event);
	void onSearchType(wxCommandEvent& event);
	void onSearchEnter(wxCommandEvent& event);

	void populateList();
};

