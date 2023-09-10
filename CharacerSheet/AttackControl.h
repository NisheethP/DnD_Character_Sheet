#pragma once

#include <wx/wx.h>
#include <string>
#include <wx/listctrl.h>

class AttackControl : public wxPanel
{
	class AddDialog : public wxDialog
	{
		wxTextCtrl* input[4] = { nullptr, nullptr, nullptr, nullptr };
		wxChoice* diceType = nullptr;
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
		std::string getName() { return input[0]->GetValue().ToStdString(); }
		int getMod() { return std::stoi(input[1]->GetValue().ToStdString()); }
		int getDmgDice() { return std::stoi(input[2]->GetValue().ToStdString()); }
		std::string getDieType() { return diceType->GetString(diceType->GetSelection()).ToStdString(); };
		std::string getDmgType() { return input[3]->GetValue().ToStdString(); }

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
	void onListDClick(wxListEvent& event);
};

