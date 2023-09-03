#include "SpellSlotDialog.h"

#include <wx/valgen.h>
#include <wx/spinctrl.h>

SpellSlotDialog::SpellSlotDialog(wxWindow* parent, wxWindowID id, const wxString& heading, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
	wxDialog(parent, id, heading, pos, size, style, name)
{
	int gap = 10;
	auto sizer = new wxBoxSizer(wxVERTICAL);

	for (int i = 0; i < 9; ++i)
	{
		int lev = i + 1;
		auto horSizer = new wxBoxSizer(wxHORIZONTAL);

		auto text = new wxStaticText(this, wxID_ANY, "Level " + std::to_string(lev));
		auto spin = new wxSpinCtrl(this, wxID_ANY, "0");

		auto vali = &(slots[lev-1]);
		spin->SetValidator(wxGenericValidator(vali));

		horSizer->Add(text, 0, wxALIGN_CENTER);
		horSizer->Add(3, -1);
		horSizer->Add(spin);

		sizer->Add(-1, 3);
		sizer->Add(horSizer, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, gap);
	}

	sizer->Add(-1, gap);
	sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_RIGHT);
	sizer->Add(-1, gap);

	SetSizerAndFit(sizer);
	Layout();
}

int SpellSlotDialog::getSlots(int i)
{
	TransferDataFromWindow();

	if (i < 1 || i > 9)
		return -1;
	else
		return slots[i-1];
}
