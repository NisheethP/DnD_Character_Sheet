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

	page1 = new ClassLevelStatSelectionPage(this);
	//page2 = new StatSelectionPage(this);
	page2 = new HP_SpeedSelectionPage(this, 20);

	page1->Chain(page2);
	//page2->Chain(page3);

	GetPageAreaSizer()->Add((wxWizardPageSimple*)page1);
	
	this->Bind(wxEVT_WIZARD_BEFORE_PAGE_CHANGED, &NewCharacterWizard::onPageChanging, this);
	this->SetMinSize(FromDIP(wxSize(600,400)));
}

void NewCharacterWizard::onPageChanging(wxWizardEvent& event)
{
	auto nextPage = GetCurrentPage()->GetNext();
	if (nextPage == page2)
	{
		page2->setHP_Mode(1);
		page2->setLevel(page1->getCharLevel());
		page2->Create();
		page2->arrange();
	}


	GetPageAreaSizer()->Fit(page2);
	GetPageAreaSizer()->Layout();
}

//======================
/// CLASS LEVEL SELECTION
//======================

ClassLevelStatSelectionPage::ClassLevelStatSelectionPage(
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
	charClass = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choicesVector.size(), choices);
	delete[] choices;
	charClass->SetSelection(0);

	choices = new wxString[4];
	choices[0] = "None";
	choices[1] = "One-Third Caster";
	choices[2] = "Half Caster";
	choices[3] = "Full Caster";

	auto casterTypeText = new wxStaticText(this, wxID_ANY, "Caster Type: ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	classType = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, choices);
	delete[] choices;
	classType->SetSelection(0);

	auto charLevelText = new wxStaticText(this, wxID_ANY, "Character Level", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	charLevel = new wxSpinCtrl(this, wxID_ANY, "1");
	charLevel->SetMin(1);
	charLevel->SetMax(20);
	charLevel->SetValue(1);

	int gap = 1;
	auto sizer = new wxFlexGridSizer(10, 2, gap, gap);

	sizer->Add(classText);
	sizer->Add(charClass, 0, wxTOP | wxLEFT, gap);

	sizer->Add(casterTypeText);
	sizer->Add(classType, 0, wxTOP | wxLEFT, gap);

	sizer->Add(charLevelText);
	sizer->Add(charLevel, 0, wxTOP | wxLEFT, gap);

	auto statTitle = new wxStaticText(this, wxID_ANY, "STATS");
	statTitle->SetFont(statTitle->GetFont().Bold().Larger().Larger());
	sizer->Add(statTitle);
	sizer->AddSpacer(0);

	choices = new wxString[6];
	choices[0] = "STR";
	choices[1] = "DEX";
	choices[2] = "CON";
	choices[3] = "INT";
	choices[4] = "WIS";
	choices[5] = "CHA";


	for (int i = 0; i < 6; ++i)
	{
		sizer->Add(new wxStaticText(this, wxID_ANY, choices[i], wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT), 0, wxEXPAND);

		StatInput[i] = new wxSpinCtrl(this, wxID_ANY, "10", wxDefaultPosition, wxDefaultSize);
		sizer->Add(StatInput[i], 0, wxEXPAND | wxTOP | wxLEFT, gap);
	}

	delete[] choices;
	SetSizerAndFit(sizer);
}

int WizardPages::ClassLevelStatSelectionPage::getStat(int index)
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
	:
	wxWizardPageSimple(parent, prev, next, bitmap),
	level(p_level)
{
	int gap = 5;
	auto sizer = new wxBoxSizer(wxVERTICAL);
	auto hor_Sizer1 = new wxBoxSizer(wxHORIZONTAL);
	auto hor_Sizer2 = new wxBoxSizer(wxHORIZONTAL);
	HP_Sizer = new wxFlexGridSizer(level + 1, 4, gap, gap);

	wxString choices[2] = {"Total" , "Level by Level"};

	auto HP_Mode_Text = new wxStaticText(this, wxID_ANY, "HP method");
	HP_Mode = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choices);
	HP_Mode->SetSelection(1);

	auto Speed_Text = new wxStaticText(this, wxID_ANY, "Speed", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	Speed_Spin = new wxSpinCtrl(this, wxID_ANY, "30");

	hor_Sizer1->Add(Speed_Text, 0);
	hor_Sizer1->Add(Speed_Spin, 0, wxEXPAND | wxLEFT, gap);
	hor_Sizer2->Add(HP_Mode_Text, 0);
	hor_Sizer2->Add(HP_Mode, 0, wxEXPAND | wxLEFT, gap);

	for (int i = 0; i < level+1; ++i)
	{
		if (i == 0)
		{
			levelHP_Text.push_back(new wxStaticText(this, wxID_ANY, "Total HP", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
			HP_Spin.push_back(new wxSpinCtrl(this, wxID_ANY, "0"));
			HP_Sizer->Add(levelHP_Text[0]);
			HP_Sizer->Add(HP_Spin[0]);

			levelHP_Text[0]->Hide();
			HP_Spin[0]->Hide();

			HP_Sizer->AddSpacer(0);
			HP_Sizer->AddSpacer(0);
			continue;
		}

		levelHP_Text.push_back(new wxStaticText(this, wxID_ANY, "HP for level " + std::to_string(i), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
		HP_Spin.push_back(new wxSpinCtrl(this, wxID_ANY, "0"));
		HP_Sizer->Add(levelHP_Text[i]);
		HP_Sizer->Add(HP_Spin[i]);

		//levelHP_Text[i]->Hide();
		//HP_Spin[i]->Hide();
	}

	sizer->Add(hor_Sizer1, 0, wxEXPAND | wxTOP, gap);
	sizer->Add(hor_Sizer2, 0, wxEXPAND | wxTOP, gap);
	sizer->Add(HP_Sizer, 0, wxEXPAND | wxTOP, gap/2.0f);

	SetSizerAndFit(sizer);
	Layout();
	HP_Mode->Bind(wxEVT_CHOICE, &HP_SpeedSelectionPage::onChoiceChange, this);
}

void WizardPages::HP_SpeedSelectionPage::arrange()
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

	GetSizer()->Layout();
	Layout();
}

void WizardPages::HP_SpeedSelectionPage::onChoiceChange(wxCommandEvent& event)
{
	arrange();
}

void WizardPages::HP_SpeedSelectionPage::Create()
{
	for (auto i = HP_Spin.begin() + 1; i != HP_Spin.end(); ++i)
		(*i)->Destroy();

	for (auto i = levelHP_Text.begin() + 1; i != levelHP_Text.end(); ++i)
		(*i)->Destroy();

	HP_Spin.erase(HP_Spin.begin() + 1, HP_Spin.end());
	levelHP_Text.erase(levelHP_Text.begin() + 1, levelHP_Text.end());

	HP_Sizer->SetRows(level+2);

	for (int i = 0; i < level; ++i)
	{
		levelHP_Text.push_back(new wxStaticText(this, wxID_ANY, "HP for lvl " + std::to_string(i + 1), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL));
		HP_Spin.push_back(new wxSpinCtrl(this, wxID_ANY, "0"));
		HP_Sizer->Add(levelHP_Text[i + 1]);
		HP_Sizer->Add(HP_Spin[i + 1]);
	
		//levelHP_Text[i+1]->Hide();
		//HP_Spin[i+1]->Hide();
	}
	
	Layout();
}
