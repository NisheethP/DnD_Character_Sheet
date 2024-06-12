#pragma once

class InventoryControl :
    public wxPanel
{

    wxButton* Add = nullptr;
    wxButton* Rem = nullptr;
    wxListView* list = nullptr;
	wxTextCtrl* totalWeight = nullptr;
	wxStaticText* title = nullptr;
	wxStaticText* weight = nullptr;

public:
	InventoryControl(wxWindow* parent,
		wxWindowID 	id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxTAB_TRAVERSAL,
		const wxString& name = wxPanelNameStr
	);

	void onAddButton(wxCommandEvent& event);
	void onRemButton(wxCommandEvent& event);
	void onListDClick(wxListEvent& event);

	void resizeCtrl();

	wxListView* getList() { return list; }
};

