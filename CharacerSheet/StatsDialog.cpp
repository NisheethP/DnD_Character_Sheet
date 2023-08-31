#include "StatsDialog.h"

StatsDialog::StatsDialog(
	wxWindow* parent, 
	wxWindowID id, 
	const wxString& heading, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name):
	wxDialog(parent, id, heading, pos, size, style, name)
{
	auto sizer = new wxGridSizer(7, 2, 5, 1);
	int margin = 5;

	std::string statName[6] =
	{
		"Strength",
		"Dexterity",
		"Constitution",
		"Intelligence",
		"Wisdom",
		"Charisma"
	};

	for (int i = 0; i < 6; ++i)
	{
		auto text = new wxStaticText(this, wxID_ANY, statName[i]);
		auto tempSpin = new wxSpinCtrl(this, wxID_ANY, "10", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL | wxSP_ARROW_KEYS);

		sizer->Add(text, 0, wxALIGN_RIGHT | wxLEFT, margin);
		sizer->Add(tempSpin, 0, wxEXPAND | wxLEFT | wxRIGHT, margin);

		tempSpin->SetMin(0);
		tempSpin->SetMax(30);

		spin.push_back(tempSpin);
	}

	sizer->Add(CreateButtonSizer(wxOK), 0, wxALIGN_RIGHT | wxLEFT | wxTOP | wxBOTTOM, margin);
	sizer->Add(CreateButtonSizer(wxCANCEL), 0, wxALIGN_RIGHT | wxRIGHT | wxTOP | wxBOTTOM, margin);
	//sizer->Add(-1, 2);
	
	//sizer->AddGrowableCol(0, 1);
	SetSizerAndFit(sizer);
	Layout();
}
