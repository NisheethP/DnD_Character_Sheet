#include "PreCompiledHeader.h"

#include "SliderRemDialog.h"

SliderRemDialog::SliderRemDialog(
	wxWindow* parent, 
	wxWindowID id, 
	std::vector<std::pair<wxStaticText*, wxSlider*>> pSlider,
	const wxString& heading, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name):
	wxDialog(parent, id, heading, pos, size, style, name),
	slider(pSlider)
{
	auto sizer = new wxBoxSizer(wxVERTICAL);
	list = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);

	list->AppendColumn("Title");
	list->AppendColumn("Min Value");
	list->AppendColumn("Max Value");

	int i = 0;
	for (auto it = slider.begin(); it != slider.end(); ++it)
	{
		list->InsertItem(i, it->first->GetLabel());
		list->SetItem(i, 1, std::to_string(it->second->GetMin()));
		list->SetItem(i, 2, std::to_string(it->second->GetMax()));
		++i;
	}
	auto text = new wxStaticText(this, wxID_ANY, "Please Select the slider to remove");
	
	sizer->Add(text, 0, wxLEFT | wxRIGHT, 5);
	sizer->Add(-1, 3);
	sizer->Add(list, 0 , wxEXPAND | wxLEFT | wxRIGHT, 5);
	sizer->Add(-1, 10);
	sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_RIGHT | wxTOP | wxRIGHT, 5);
	sizer->Add(-1, 10);

	this->SetSizerAndFit(sizer);
}

long SliderRemDialog::getSelected()
{
	long item = -1;
	for (;;)
	{
		item = list->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (item == -1)
			break;
		// this item is selected - do whatever is needed with it
		return item;
	}

	return -1;
}
