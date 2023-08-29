#include "FeatureDialog.h"

#include <wx/sizer.h>

FeatureDialog::FeatureDialog(wxWindow* parent, wxWindowID id, const wxString& heading, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
	wxDialog(parent, id, heading, pos, size, style, name)
{
	wxStaticText* titleText = new wxStaticText(this, wxID_ANY, "Title");
	wxStaticText* descText = new wxStaticText(this, wxID_ANY, "Description");

	wxSize curSize(400, -1);

	int gap = 3;
	int margin = 10;

	auto mainSizer = new wxBoxSizer(wxVERTICAL);
	auto sizer = new wxFlexGridSizer(3, 2, gap, gap);


	title = new wxTextCtrl(this, wxID_ANY, "Feature", wxDefaultPosition, wxDefaultSize);
	description = new wxTextCtrl(this, wxID_ANY, "Description", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

	sizer->Add(titleText,0, wxALIGN_RIGHT);
	sizer->Add(title,0, wxALIGN_CENTER);
	sizer->Add(descText,0, wxALIGN_RIGHT);
	sizer->Add(description, 0, wxALIGN_RIGHT);
	
	mainSizer->Add(sizer, 1, wxALIGN_CENTER | wxALL, margin);
	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, margin);

	sizer->AddGrowableCol(1);
	sizer->AddGrowableRow(1);
	title->SetFocus();

	title->SetMinSize(curSize);
	description->SetMinSize(wxSize(curSize.x, 100));

	this->SetSizerAndFit(mainSizer);
}
