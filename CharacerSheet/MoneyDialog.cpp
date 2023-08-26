#include "MoneyDialog.h"
#include <wx/gbsizer.h>


MoneyDialog::MoneyDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
	wxDialog(parent, id, title, pos, size, style, name)
{
	std::string moneyNames[4] = {"Platinum", "Gold", "Silver", "Copper"};
	std::vector<wxStaticText*> titles;

	int gap = 3;
	int margin = 10;
	auto sizer = new wxGridBagSizer(gap, gap);

	for (int i = 0; i < 4; ++i)
	{
		titles.push_back(new wxStaticText(this, wxID_ANY, moneyNames[i]));
		val.push_back(new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL));
		
		val[i]->SetMin(0);
		val[i]->SetMax(1e5);
	}

	sizer->Add(-1,5);
	for (int i = 0; i < 4; i++)
		sizer->Add(titles[i], { i+1,0 }, { 1,1 }, wxALIGN_RIGHT | wxLEFT, 2.5*margin);
	
	for (int i = 0; i < 4; i++)
		sizer->Add(val[i], { i+1,1 }, { 1,1 }, wxALIGN_CENTER | wxRIGHT, margin);
	
	sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), { 5,0 }, { 1,2 }, wxALIGN_CENTER | wxALL, margin);

	this->SetSizerAndFit(sizer);
}
