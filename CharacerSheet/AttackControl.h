#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>

class AttackControl : public wxPanel
{
	class AddDialog : public wxDialog
	{
		wxTextCtrl* input[3] = { nullptr, nullptr, nullptr };
	public:
		AddDialog(wxWindow* parent,
			wxWindowID 	id,
			const wxString& title,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long 	style = wxDEFAULT_DIALOG_STYLE,
			const wxString& name = wxDialogNameStr
		);

		std::string getInput(int index);
	};

	wxButton* Add = nullptr;
	wxButton* Rem = nullptr;
	wxListView* list = nullptr;
public:
	AttackControl(wxWindow* parent,
		wxWindowID 	id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxTAB_TRAVERSAL,
		const wxString& name = wxPanelNameStr
	);

	wxListView* getList()		{ return list; }
	wxButton* getAddButton()	{ return Add; }
	wxButton* getRemButton()	{ return Rem; }

	void onAddButton(wxCommandEvent& event);
	void onRemButton(wxCommandEvent& event);
};

