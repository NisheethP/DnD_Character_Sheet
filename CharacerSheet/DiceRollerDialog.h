#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>

#include <vector>
#include <tuple>

class DiceRollerDialog : public wxDialog
{
	wxListBox* History = nullptr;
	std::vector<std::tuple<wxSpinCtrl*, wxChoice*, wxSpinCtrl*, wxButton*>> DiceLine;
	std::vector<std::tuple<int*, wxString*, int*>> diceData;
	wxButton* Add = nullptr;
	wxButton* Roll = nullptr;
	wxBoxSizer* mainSizer = nullptr;
	wxBoxSizer* diceSizer = nullptr;
	int maxHistory = 100;
	
public:
	DiceRollerDialog(wxWindow* parent,
		wxWindowID	id = wxID_ANY,
		const wxString& heading = "Dice Roller",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	~DiceRollerDialog();

	wxPanel* createDiceLine(wxWindow* parent);

	void BindAll();

	void onRemoveRow(wxCommandEvent& event);
	void onAddRow(wxCommandEvent& event);
	void onRoll(wxCommandEvent& event);
};

