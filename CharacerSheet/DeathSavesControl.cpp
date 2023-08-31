#include "DeathSavesControl.h"

#include <wx/graphics.h>
#include <wx/dcbuffer.h>

DeathSavesControl::DeathSavesControl(wxWindow* parent, wxWindowID id, wxColour fg, wxColour bg, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
	wxWindow(parent, id, pos, size, style, name),
	fgColour(fg),
	bgColour(bg)
{
	auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
	auto verSizer = new wxBoxSizer(wxVERTICAL);
	auto succSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Successes");
	auto failSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Fails");

	int flag = wxLEFT;
	int margin = 5;
	for (int i = 0; i < 3; ++i)
	{
		success[i] = new wxRadioButton(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxRB_SINGLE);
		fails[i] = new wxRadioButton(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxRB_SINGLE);

		succSizer->Add(success[i], 0, flag, margin);
		failSizer->Add(fails[i], 0, flag, margin);
	}
	
	auto sucBox = succSizer->GetStaticBox();
	auto failBox = failSizer->GetStaticBox();
	sucBox->SetForegroundColour(fgColour);
	failBox->SetForegroundColour(fgColour);

	sucBox->SetBackgroundColour(bgColour);
	failBox->SetBackgroundColour(bgColour);

	this->SetBackgroundColour(bgColour);

	succSizer->GetStaticBox()->SetFont(succSizer->GetStaticBox()->GetFont().Bold().Larger());
	failSizer->GetStaticBox()->SetFont(failSizer->GetStaticBox()->GetFont().Bold().Larger());

	verSizer->Add(-1, margin);
	verSizer->Add(succSizer, 1, wxEXPAND);
	verSizer->Add(-1, margin);
	verSizer->Add(failSizer, 1, wxEXPAND);
	verSizer->Add(-1, margin);

	std::string spacer = "  ";
	std::string buttonText = "";
	buttonText += spacer + "R" + spacer + "\n";
	buttonText += spacer + "E" + spacer + "\n";
	buttonText += spacer + "S" + spacer + "\n";
	buttonText += spacer + "E" + spacer + "\n";
	buttonText += spacer + "T" + spacer + "\n";
	reset = new wxButton(this, wxID_ANY, buttonText, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	reset->SetFont(reset->GetFont().Bold());

	mainSizer->Add(verSizer, 0, wxEXPAND);
	mainSizer->Add(10, -1);
	mainSizer->Add(reset, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);

	reset->Bind(wxEVT_BUTTON, &DeathSavesControl::onReset, this);

	SetSizer(mainSizer);
	Layout();
}

void DeathSavesControl::onReset(wxCommandEvent& event)
{
	for (int i = 0; i < 3; ++i)
	{
		success[i]->SetValue(false);
		fails[i]->SetValue(false);
	}
}

