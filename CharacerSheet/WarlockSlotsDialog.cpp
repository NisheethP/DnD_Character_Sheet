#include "WarlockSlotsDialog.h"
#include <wx/spinctrl.h>
#include <wx/valgen.h>

WarlockSlotsDialog::WarlockSlotsDialog(wxWindow* parent, wxWindowID id, const wxString& heading, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
	wxDialog(parent, id, heading, pos, size, style, name),
	numSlots(0),
	level(0)
{
	std::string str[] =
	{
		"Number of Slots",
		"Slot Level"
	};

	int* x[2] = {&numSlots, &level};
	
	auto sizer = new wxBoxSizer(wxVERTICAL);

	for (int i = 0; i < 2; ++i)
	{
		auto text = new wxStaticText(this, wxID_ANY, str[i]);
		auto spin = new wxSpinCtrl(this, wxID_ANY, "0");
		
		spin->SetValidator(wxGenericValidator(x[i]));
		auto horSizer = new wxBoxSizer(wxHORIZONTAL);

		horSizer->Add(text);
		horSizer->Add(3, -1);
		horSizer->Add(spin);

		sizer->Add(horSizer, 0, wxALIGN_RIGHT | wxALL, 5);
	}

	sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 5);

	SetSizerAndFit(sizer);
	Layout();
}