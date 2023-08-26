#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>

#include <vector>

class MoneyDialog : public wxDialog
{

	std::vector<wxSpinCtrl*> val;

public:
	MoneyDialog(wxWindow* parent,
		wxWindowID 	id = wxID_ANY,
		const wxString& title = "Money",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	std::vector<int> getMoney()
	{
		std::vector<int> temp;

		temp.push_back(val[0]->GetValue());
		temp.push_back(val[1]->GetValue());
		temp.push_back(val[2]->GetValue());
		temp.push_back(val[3]->GetValue());
	}
	int getPlatinnum()	{ return val[0]->GetValue(); }
	int getGold()		{ return val[1]->GetValue(); }
	int getSilver()		{ return val[2]->GetValue(); }
	int getCopper()		{ return val[3]->GetValue(); }
};
