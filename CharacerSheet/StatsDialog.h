#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>

class StatsDialog : public wxDialog
{
	// STR-DEX-CON-INT-WIS-CHA
	std::vector<wxSpinCtrl*> spin;
public:
	StatsDialog(wxWindow* parent,
		wxWindowID 	id = wxID_ANY,
		const wxString& heading = "Add Feature",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long 	style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr);

	std::vector<int> getStats()
	{
		std::vector<int> temp;
		
		for (int i = 0; i < 6; ++i)
			temp.push_back(spin[i]->GetValue());

		return temp;
	};

	int getStr() { return spin[0]->GetValue(); }
	int getDex() { return spin[1]->GetValue(); }
	int getCon() { return spin[2]->GetValue(); }
	int getInt() { return spin[3]->GetValue(); }
	int getWis() { return spin[4]->GetValue(); }
	int getCha() { return spin[5]->GetValue(); }
	
};

