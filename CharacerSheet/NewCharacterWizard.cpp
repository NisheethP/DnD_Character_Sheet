#include "PreCompiledHeader.h"
#include "NewCharacterWizard.h"
#include "Character.h"

using namespace WizardPages;

NewCharacterWizard::NewCharacterWizard(
	wxWindow* parent,
	int id,
	const wxString& title,
	const wxBitmap& bitmap,
	const wxPoint& pos,
	long style)
	:
	wxWizard(parent, id, title, bitmap, pos, style)
{
	wxFrame* frame = static_cast<wxFrame*>(parent);

	page1 = new ClassLevelSelectionPage(this);
	
	GetPageAreaSizer()->Add((wxWizardPageSimple*)page1);

	page2 = new StatSelectionPage(this);
	//page3 = new HP_SpeedSelectionPage(this, page1->getCharLevel());

	page1->Chain(page2);
	//page2->Chain(page3);
}

//======================
/// CLASS LEVEL SELECTION
//======================

ClassLevelSelectionPage::ClassLevelSelectionPage(
	wxWizard* parent, 
	wxWizardPage* prev, 
	wxWizardPage* next, 
	const wxBitmap& bitmap)
	:
	wxWizardPageSimple(parent, prev, next, bitmap)
{
	std::vector<std::string> choicesVector;
	for (int i = 0; i != static_cast<int>(ClassType::none); ++i)
	{
		ClassType type = static_cast<ClassType>(i);
		std::string temp = getStringFromClass(type);
		choicesVector.push_back(temp);
	}

	wxString* choices = new wxString[choicesVector.size()];

	std::copy(choicesVector.begin(), choicesVector.end(), choices);

	auto classText = new wxStaticText(this, wxID_ANY, "Class: ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	charClass = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, choicesVector.size(), choices);
	delete[] choices;
	charClass->SetSelection(0);

	choices = new wxString[4];
	choices[0] = "None";
	choices[1] = "One-Third Caster";
	choices[2] = "Half Caster";
	choices[3] = "Full Caster";

	auto casterTypeText = new wxStaticText(this, wxID_ANY, "Caster Type: ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	classType = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 4, choices);
	delete[] choices;
	classType->SetSelection(0);

	auto charLevelText = new wxStaticText(this, wxID_ANY, "Character Level", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	charLevel = new wxSpinCtrl(this, wxID_ANY, "1");
	charLevel->SetMin(1);
	charLevel->SetMax(20);
	charLevel->SetValue(1);

	int gap = 1;
	auto sizer = new wxGridSizer(3, 2, gap, gap);

	sizer->Add(classText);
	sizer->Add(charClass);

	sizer->Add(casterTypeText);
	sizer->Add(classType);

	sizer->Add(charLevelText);
	sizer->Add(charLevel);

	SetSizerAndFit(sizer);
}

//======================
/// STAT SELECTION
//======================
WizardPages::StatSelectionPage::StatSelectionPage(
	wxWizard* parent, 
	wxWizardPage* prev, 
	wxWizardPage* next, 
	const wxBitmap& bitmap)
	:
	wxWizardPageSimple(parent, prev, next, bitmap)
{
	int gap = 5;
	auto sizer = new wxGridSizer(6, 2, gap, gap);

	auto choices = new wxString[6];
	choices[0] = "STR";
	choices[1] = "DEX";
	choices[2] = "CON";
	choices[3] = "INT";
	choices[4] = "WIS";
	choices[5] = "CHA";

	for (int i = 0; i < 6; ++i)
	{
		sizer->Add(new wxStaticText(this, wxID_ANY, choices[i], wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT), 0, wxEXPAND);
		
		StatInput[i] = new wxSpinCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize);
		sizer->Add(StatInput[i], 0, wxEXPAND);
	}

	SetSizerAndFit(sizer);
}

int WizardPages::StatSelectionPage::getStat(int index)
{
	switch (index)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		return StatInput[index]->GetValue();
	default:
		return -1;
	}
}
//======================
/// HP SPEED SELECTION
//======================
HP_SpeedSelectionPage::HP_SpeedSelectionPage(
	wxWizard* parent, 
	int p_level,
	wxWizardPage* prev, 
	wxWizardPage* next, 
	const wxBitmap& bitmap)
{
	wxString choices[2] = {"Total" , "Level by Level"};

	auto HP_Mode_Text = new wxStaticText(this, wxID_ANY, "HP method");
	HP_Mode = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 2, choices);
	HP_Mode->SetSelection(0);

	int gap = 5;
	auto sizer = new wxGridSizer(p_level+3, 2, gap, gap);

	auto Speed_Text = new wxStaticText(this, wxID_ANY, "Speed", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	Speed_Spin = new wxSpinCtrl(this, wxID_ANY, "0");

	sizer->Add(Speed_Text);
	sizer->Add(Speed_Spin);
	sizer->Add(HP_Mode_Text);
	sizer->Add(HP_Mode);

	for (int i = 0; i < p_level+1; ++i)
	{
		if (i == 0)
		{
			levelHP_Text.push_back(new wxStaticText(this, wxID_ANY, "Enter total HP", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT));
			HP_Spin.push_back(new wxSpinCtrl(this, wxID_ANY, "0"));
			sizer->Add(levelHP_Text[0]);
			sizer->Add(HP_Spin[0]);
			continue;
		}

		levelHP_Text.push_back(new wxStaticText(this, wxID_ANY, "Enter HP for lvl " + std::to_string(i), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT));
		HP_Spin.push_back(new wxSpinCtrl(this, wxID_ANY, "0"));
		sizer->Add(levelHP_Text[i]);
		sizer->Add(HP_Spin[i]);

		levelHP_Text[i]->Hide();
		HP_Spin[i]->Hide();
		
	}
	
	SetSizer(sizer);
	HP_Mode->Bind(wxEVT_CHOICE, &HP_SpeedSelectionPage::onComboBoxChange, this);
}

void WizardPages::HP_SpeedSelectionPage::onComboBoxChange(wxCommandEvent& event)
{
	int x = HP_Mode->GetSelection();

	if (x == 0)
	{
		for (auto& it : levelHP_Text)
			it->Hide();
		for (auto& it : HP_Spin)
			it->Hide();

		levelHP_Text[0]->Show();
		HP_Spin[0]->Show();
	}

	if (x == 1)
	{
		for (auto& it : levelHP_Text)
			it->Show();
		for (auto& it : HP_Spin)
			it->Show();

		levelHP_Text[0]->Hide();
		HP_Spin[0]->Hide();
	}

	Layout();

}
