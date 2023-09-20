#include "PreCompiledHeader.h"
#include "LevelUpDialog.h"

LevelUpDialog::LevelUpDialog(
	wxWindow* parent,
	wxWindowID id,
	const wxString& heading,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) 
	:
	wxDialog(parent, id, heading, pos, size, style, name)
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
	classCombo = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, choicesVector.size(), choices);
	delete[] choices;
	classCombo->SetSelection(0);

	choices = new wxString[4];
	choices[0] = "None";
	choices[1] = "One-Third Caster";
	choices[2] = "Half Caster";
	choices[3] = "Full Caster";

	auto casterTypeText = new wxStaticText(this, wxID_ANY, "Caster Type: ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	classCasterType = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 4, choices);
	delete[] choices;
	classCasterType->SetSelection(0);

	choices = new wxString[6];
	choices[0] = "STR";
	choices[1] = "DEX";
	choices[2] = "CON";
	choices[3] = "INT";
	choices[4] = "WIS";
	choices[5] = "CHA";

	auto HP_Text = new wxStaticText(this, wxID_ANY, "Total HP Gained: ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	HP_gained = new wxSpinCtrl(this, wxID_ANY, "0");

	int gap = 3;
	auto sizer = new wxBoxSizer(wxVERTICAL);
	auto SubSizer = new wxGridBagSizer(gap, gap);

	std::vector<std::pair<wxGBPosition, wxGBSpan>> items =
	{
		{{0,0}, {1,1}},		//0
		{{0,1}, {1,1}},		//1

		{{1,0}, {1,1}},		//2
		{{1,1}, {1,1}},		//3

		{{2,0}, {1,1}},		//4
		{{2,1}, {1,1}},		//5

		{{3,0}, {1,2}},		//6 STAT CHANGES

		{{4,0}, {1,1}},		//7
		{{4,1}, {1,1}},		//8

		{{5,0}, {1,1}},		//9
		{{5,1}, {1,1}},		//10

		{{6,0}, {1,1}},		//11
		{{6,1}, {1,1}},		//12

		{{7,0}, {1,1}},		//13
		{{7,1}, {1,1}},		//14

		{{8,0}, {1,1}},		//15
		{{8,1}, {1,1}},		//16

		{{9,0}, {1,1}},		//17
		{{9,1}, {1,1}},		//18
	};
	
	std::pair<wxGBPosition, wxGBSpan> curSizer;

	curSizer = items[0];
	SubSizer->Add(classText, curSizer.first, curSizer.second, wxEXPAND);

	curSizer = items[1];
	SubSizer->Add(classCombo, curSizer.first, curSizer.second, wxEXPAND);

	curSizer = items[2];
	SubSizer->Add(casterTypeText, curSizer.first, curSizer.second, wxEXPAND);

	curSizer = items[3];
	SubSizer->Add(classCasterType, curSizer.first, curSizer.second, wxEXPAND);

	curSizer = items[4];
	SubSizer->Add(HP_Text, curSizer.first, curSizer.second, wxEXPAND);

	curSizer = items[5];
	SubSizer->Add(HP_gained, curSizer.first, curSizer.second, wxEXPAND);

	curSizer = items[6];
	auto StatChangeText = new wxStaticText(this, wxID_ANY, "Stat Changes");
	StatChangeText->SetFont(StatChangeText->GetFont().Bold().Underlined().Larger());
	SubSizer->Add(StatChangeText, curSizer.first, curSizer.second, wxALIGN_CENTER);

	for (int i = 0; i < 6; ++i)
	{
		curSizer = items[7+2*i];
		SubSizer->Add(new wxStaticText(this, wxID_ANY, choices[i], wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT), curSizer.first, curSizer.second, wxEXPAND);

		curSizer = items[8+2*i];
		Stat[i] = new wxSpinCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize);
		SubSizer->Add(Stat[i], curSizer.first, curSizer.second, wxEXPAND);
	}
	delete[] choices;

	sizer->Add(SubSizer, 1, wxEXPAND | wxALL, 2*gap);
	sizer->Add(-1, gap);
	sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxBOTTOM, gap);
	SetSizerAndFit(sizer);
	Layout();
}

const int LevelUpDialog::getStat(Skills  pStat) const
{
	switch (pStat)
	{
	case none:
	case Acrobatics:
	case Animal_Handling:
	case Arcana:
	case Athletics:
	case Deception:
	case History:
	case Insight:
	case Intimidation:
	case Investigation:
	case Medicine:
	case Nature:
	case Perception:
	case Performance:
	case Persuasion:
	case Religion:
	case Sleight_of_Hand:
	case Stealth:
	case Survival:
		return -20;
	case Strength:
		return Stat[0]->GetValue();
	case Dexterity:
		return Stat[1]->GetValue();
	case Constitution:
		return Stat[2]->GetValue();
	case Intelligence:
		return Stat[3]->GetValue();
	case Wisdom:
		return Stat[4]->GetValue();
	case Charisma:
		return Stat[5]->GetValue();
	default:
		return -21;
	}
}

const int LevelUpDialog::getStat(int index) const
{
	if (index < 0 || index > 5)
		return -20;
	
	return Stat[index]->GetValue();
}

const CharClass LevelUpDialog::getClass() const
{
	CharClass charClass;

	CharClass::CasterType CastType = CharClass::CasterType::None;
	auto typeStr = classCasterType->GetValue().ToStdString();

	if (typeStr == "None")
		CastType = CharClass::CasterType::None;
	if (typeStr == "One-Third Caster")
		CastType = CharClass::CasterType::Third;
	if (typeStr == "Half Caster")
		CastType = CharClass::CasterType::Half;
	if (typeStr == "Full Caster")
		CastType = CharClass::CasterType::Full;

	charClass.casterType = CastType;
	
	for (int i = 0; i != static_cast<int>(ClassType::none); ++i)
	{
		ClassType type = static_cast<ClassType>(i);
		std::string temp = getStringFromClass(type);
	
		if (temp != classCombo->GetValue())
			continue;
		
		charClass.classType = type;
		break;
	}

	charClass.level = 1;
	charClass.updateHitDie();

	return charClass;
}
