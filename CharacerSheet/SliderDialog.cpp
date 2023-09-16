#include "PreCompiledHeader.h"

#include "SliderDialog.h"


SliderDialog::SliderDialog(wxWindow* parent, wxWindowID id, const wxString& heading, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
	wxDialog(parent, id, heading, pos, size, style, name)
{
	auto text = new wxStaticText(this, wxID_ANY, "Slider Title", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	auto min_text = new wxStaticText(this, wxID_ANY, "Min Value", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	auto max_text = new wxStaticText(this, wxID_ANY, "Max Value", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	auto def_text = new wxStaticText(this, wxID_ANY, "Default Value", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

	int gap = 5;
	auto sizer = new wxFlexGridSizer(5, 2, gap, gap);

	title = new wxTextCtrl(this, wxID_ANY, "Slider");
	min = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	max = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	def = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);

	min->SetMin(-1e4);
	min->SetMax(10);
	
	max->SetMin(0);
	max->SetMax(1e4);
	max->SetValue(10);

	def->SetMin(0);
	def->SetMax(10);
	def->SetValue(10);

	min->SetMinSize(FromDIP(wxSize(100,-1)));
	max->SetMinSize(FromDIP(wxSize(100,-1)));
	def->SetMinSize(FromDIP(wxSize(100,-1)));

	sizer->Add(text, 0, wxTOP | wxLEFT, 5);
	sizer->Add(title, 0, wxTOP | wxRIGHT, 5);
	sizer->Add(min_text, 0, wxTOP | wxLEFT, 5);
	sizer->Add(min, 0, wxTOP | wxRIGHT, 5);
	sizer->Add(max_text, 0, wxTOP | wxLEFT, 5);
	sizer->Add(max, 0, wxTOP | wxRIGHT, 5);
	sizer->Add(def_text, 0, wxTOP | wxLEFT, 5);
	sizer->Add(def, 0, wxTOP | wxRIGHT, 5);
	sizer->AddSpacer(0);
	sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxTOP | wxRIGHT | wxBOTTOM, 5);
	this->SetSizerAndFit(sizer);

	title->SetFocus();

	Bind(wxEVT_SPINCTRL, &SliderDialog::onSpinChange, this);
}

void SliderDialog::onSpinChange(wxSpinEvent& event)
{
	auto obj = event.GetEventObject();

	if (obj == min)
	{
		int x = min->GetValue();
		def->SetMin(x);
		max->SetMin(x);

	}

	if (obj == max)
	{
		int x = max->GetValue();
		def->SetMax(x);
		min->SetMax(x);
	}
}
