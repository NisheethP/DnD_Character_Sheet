#include <wx/sizer.h>
#include <wx/gbsizer.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/notebook.h>
#include <wx/treectrl.h>
#include <wx/html/htmlwin.h>
#include <wx/wrapsizer.h>
#include <wx/numdlg.h> 
#include <wx/choicdlg.h>
#include <wx/artprov.h>	

#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>

#include <string>

#include "MainFrame.h"
#include "Util.h"
#include "Character.h"
#include "Spells.h"
#include "MoneyDialog.h"
#include "FeatureDialog.h"
#include "SpellSelectDialog.h"
#include "DeathSavesControl.h"
#include "SliderDialog.h"
#include "SliderRemDialog.h"
#include "ConditionDialog.h"
#include "AddCondDialog.h"
#include "StatsDialog.h"
#include "SkillProfDialog.h"
#include "DiceRollerDialog.h"
#include "SpellSlotDialog.h"
#include "WarlockSlotsDialog.h"

MainFrame::MainFrame(const wxString& title, const Character& pChar) :
	wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900,600)),
	character(pChar),
	BigFont1(wxFontInfo(wxSize(0, 15))),
	BigFont2(wxFontInfo(wxSize(0, 18))),
	SkillFont(wxFontInfo(wxSize(0, 12))),
	baseColSize(170, -1),
	buttonSize(40,-1),
	masterPanel(new wxPanel(this, wxID_ANY, wxPoint(100, 400), wxDefaultSize)),
	mainNotebook(new wxNotebook(masterPanel, wxID_ANY))
{
	//LOADING MENU BAR
	CreateMenuBar();

	//INITIALISING SOME VARIABLES IN FUNCITONS
	loadSpells();
	fillLevelSpells();
	initConditions();

	//INITIALISING SOME VARIABLES
	spellDesc.ColWidth = wxSize(350, -1);
	spellDesc.dispSpell = &(allSpells[173]);
	knownPagePanels.dispSpell = &(allSpells[173]);

	//INITIALISING COLOURS
	ctrlColour.first = *wxBLACK;
	ctrlColour.second = *wxWHITE;

	descColour.first = wxColour(0xBB, 0xBB, 0xBB);
	descColour.second = wxColour(0x10, 0x10, 0x10);

	listColour.first = wxColour(0xBB, 0xBB, 0xBB);
	listColour.second = wxColour(0x10, 0x10, 0x10);

	mainColour.first = wxColour(0x28, 0x28, 0x2c);
	mainColour.second = wxColour(0xDD, 0xDD, 0xDD);
	
	panelColour.first = wxColour(0x3a, 0x3a, 0x3d);
	panelColour.second = wxColour(0xE1, 0xE1, 0xE1);

	ctrlColour.first = mainColour.second;
	ctrlColour.second = mainColour.first;

	listColour = ctrlColour;

	slotsColour.first = wxColour(0x22, 0x19, 0x19);
	slotsColour.second = wxColour(0xFF, 0xFF, 0xFF);

	proficientColour = wxColour(0xAA, 0xFF, 0xFF);
	expertiseColour = wxColour(0xFF, 0xAA, 0xAA);

	//SETTING UP THE WINDOW
	setWindowColour(this, mainColour);

	BigFont1.MakeBold();
	BigFont2.MakeBold();
	SkillFont.MakeBold();

	masterSizer = new wxBoxSizer(wxVERTICAL);
	
	this->SetSize(FromDIP(wxSize(1000,700)));
	
	updatePlayerConds();

	CreatePages(mainNotebook);
	
	mainNotebook->SetSelection(firstPage);

	masterSizer->Add(mainNotebook, 1, wxEXPAND);
	masterPanel->SetSizerAndFit(masterSizer);
	masterPanel->Layout();

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(masterPanel, 1, wxEXPAND);

	this->SetSizer(mainSizer);
	this->Layout();
	
	//BINDING ALL CONTROLS FOR THE FRAME
	BindControls();
}

void MainFrame::CreateMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar();
	wxMenu* fileMenu = new wxMenu();
	wxMenu* SetMenu = new wxMenu();
	wxMenu* ResetMenu = new wxMenu();
	wxMenu* Rest = new wxMenu();
	wxMenu* ConditionMenu = new wxMenu();
	wxMenu* DiceMenu = new wxMenu();
	

	fileMenu->Append(wxID_NEW);
	fileMenu->Append(wxID_OPEN);
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_SAVE);
	fileMenu->Append(wxID_SAVEAS);

	menuBarItems.SetName = SetMenu->Append(wxID_ANY, "Set Name");
	menuBarItems.SetMaxHP = SetMenu->Append(wxID_ANY, "Set Max HP");
	menuBarItems.SetSpeed = SetMenu->Append(wxID_ANY, "Set Speed");
	menuBarItems.SetStats = SetMenu->Append(wxID_ANY, "Set Stats");
	menuBarItems.SetInitMod = SetMenu->Append(wxID_ANY, "Set Initiative Mod");
	SetMenu->AppendSeparator();
	menuBarItems.SetSavingThrows = SetMenu->Append(wxID_ANY, "Set Saving Throw proficiency");
	menuBarItems.SetSkillProfs = SetMenu->Append(wxID_ANY, "Set Skill Proficiency");
	menuBarItems.SetExpertises = SetMenu->Append(wxID_ANY, "Set Skill Expertises");
	SetMenu->AppendSeparator();
	menuBarItems.SetSP = SetMenu->Append(wxID_ANY, "Set Spell Points");
	menuBarItems.SetSlots = SetMenu->Append(wxID_ANY, "Set Spell Slots");
	menuBarItems.SetWarlockSlots = SetMenu->Append(wxID_ANY, "Set Warlock Slots");


	menuBarItems.ConditionsAll = ConditionMenu->Append(wxID_ANY, "See All Conditions");
	menuBarItems.ConditionsPlayer = ConditionMenu->Append(wxID_ANY, "See Character Condition");
	ConditionMenu->AppendSeparator();
	menuBarItems.ConditionsAdd = ConditionMenu->Append(wxID_ANY, "Add Condition to Player");
	menuBarItems.ConditionsRemove = ConditionMenu->Append(wxID_ANY, "Remove Condition from Player");

	menuBarItems.ResetSP = ResetMenu->Append(wxID_ANY, "Reset Spell Points");
	menuBarItems.ResetSlots = ResetMenu->Append(wxID_ANY, "Reset Slots");
	menuBarItems.ResetSlots = ResetMenu->Append(wxID_ANY, "Uncheck Spells");
	ResetMenu->AppendSeparator();
	menuBarItems.ResetSlots = ResetMenu->Append(wxID_ANY, "Delete Spells");
	
	menuBarItems.DiceRoll = DiceMenu->Append(wxID_ANY, "Roll Dice");

	menuBar->Append(fileMenu, "File");
	menuBar->Append(SetMenu, "Set Values");
	menuBar->Append(ResetMenu, "Reset Values");
	menuBar->Append(ConditionMenu, "Conditions");
	menuBar->Append(DiceMenu, "Dice");
	SetMenuBar(menuBar);
}

void MainFrame::BindControls()
{
	using std::get;

	spellDesc.spellSearch->Bind(wxEVT_TEXT, &MainFrame::onSpellSearchType, this);
	spellDesc.spellSearch->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &MainFrame::onSpellSearch, this);
	spellDesc.spellList->Bind(wxEVT_TREE_SEL_CHANGED, &MainFrame::onSpellTreeSelect, this);

	mainPagePanels.AC->GetParent()->Bind(wxEVT_CHECKBOX, &MainFrame::onACCheckBoxTick, this);
	mainPagePanels.GiveTempHPButton->Bind(wxEVT_BUTTON, &MainFrame::onGiveTempHPButton, this);
	mainPagePanels.Feature_AddButton->GetParent()->Bind(wxEVT_BUTTON, &MainFrame::onFeatureButton, this);
	mainPagePanels.Feature_FullList->Bind(wxEVT_LISTBOX, &MainFrame::onFeatureSelect, this);
	//mainPagePanels.EL_ToolProf->Bind(wxEVT_LIST_ITEM_FOCUSED, &MainFrame::onToolProfecsSelect, this);
	mainPagePanels.moneyButtons.first->GetParent()->Bind(wxEVT_BUTTON, &MainFrame::onAddRemMoney, this);
	
	spellDesc.spellSplitter->Bind(wxEVT_SPLITTER_SASH_POS_CHANGED, &MainFrame::onSpellSplitterResize, this);
	spellDesc.spellSplitter->Bind(wxEVT_SPLITTER_SASH_POS_CHANGING, &MainFrame::onSpellSplitterResize, this);
	spellDesc.spellSplitter->Bind(wxEVT_SPLITTER_DOUBLECLICKED, &MainFrame::onSpellSplitterResize, this);

	knownPagePanels.Combo_SpellCastSkill->Bind(wxEVT_COMBOBOX, &MainFrame::onKnownSpellsEvents, this);

	get<0>(knownPagePanels.SpellSlotLevelList[0])->GetParent()->GetParent()->Bind(wxEVT_BUTTON, &MainFrame::onKnownSpellsUseSpell, this);
	get<0>(knownPagePanels.SpellSlotLevelList[0])->GetParent()->GetParent()->Bind(wxEVT_BUTTON, &MainFrame::onKnownSpellsUseSpellPoint, this);
	get<3>(knownPagePanels.SpellSlotLevelList[0])->GetParent()->GetParent()->Bind(wxEVT_CHECKLISTBOX, &MainFrame::onTickSpellKnownSpells, this);
	get<3>(knownPagePanels.SpellSlotLevelList[0])->GetParent()->GetParent()->Bind(wxEVT_LISTBOX, &MainFrame::onKnownSpellsSelectSpell, this);
	knownPagePanels.AddSpellSlot_Button[0]->GetParent()->GetParent()->Bind(wxEVT_BUTTON, &MainFrame::onKnownSpellsAddRemSpell, this);

	get<2>(mainPagePanels.HealToX)->Bind(wxEVT_BUTTON, &MainFrame::onHealToButton, this);
	mainPagePanels.HP->Bind(wxEVT_SPINCTRL, &MainFrame::onHPSpin, this);
	//mainPagePanels.HP->Bind(wxEVT_TEXT, &MainFrame::onHPText, this);

	mainPagePanels.TempHP->Bind(wxEVT_SPINCTRL, &MainFrame::onTempHPSpin, this);
	//mainPagePanels.TempHP->Bind(wxEVT_TEXT, &MainFrame::onTempHPText, this);

	mainPagePanels.InitMod.second->Bind(wxEVT_TEXT, &MainFrame::onInitMod, this);
	get<1>(mainPagePanels.MaxHPBonus)->Bind(wxEVT_SPINCTRL, &MainFrame::onMaxHPChange, this);

	mainPagePanels.SlidersButtons.first->Bind(wxEVT_BUTTON, &MainFrame::onAddRemSlider, this);
	mainPagePanels.SlidersButtons.second->Bind(wxEVT_BUTTON, &MainFrame::onAddRemSlider, this);

	get<1>(mainPagePanels.ToolProf)->Bind(wxEVT_BUTTON, &MainFrame::onToolProfecs, this);
	get<2>(mainPagePanels.ToolProf)->Bind(wxEVT_BUTTON, &MainFrame::onToolProfecs, this);

	get<1>(mainPagePanels.LangProf)->Bind(wxEVT_BUTTON, &MainFrame::onLangProfecs, this);
	get<2>(mainPagePanels.LangProf)->Bind(wxEVT_BUTTON, &MainFrame::onLangProfecs, this);

	mainPagePanels.EL_Conditions->Bind(wxEVT_LIST_ITEM_ACTIVATED, &MainFrame::onConditionListDClick, this);

	this->Bind(wxEVT_MENU, &MainFrame::onSetMenuEvents, this, menuBarItems.SetSP->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onSetMenuEvents, this, menuBarItems.SetMaxHP->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onSetMenuEvents, this, menuBarItems.SetStats->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onSetMenuEvents, this, menuBarItems.SetSpeed->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onSetMenuEvents, this, menuBarItems.SetName->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onSetMenuEvents, this, menuBarItems.SetInitMod->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onSetMenuEvents, this, menuBarItems.SetSavingThrows->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onSetMenuEvents, this, menuBarItems.SetSkillProfs->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onSetMenuEvents, this, menuBarItems.SetExpertises->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onSetMenuEvents, this, menuBarItems.SetSlots->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onSetMenuEvents, this, menuBarItems.SetWarlockSlots->GetId());

	this->Bind(wxEVT_MENU, &MainFrame::onConditionMenuEvents, this, menuBarItems.ConditionsAll->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onConditionMenuEvents, this, menuBarItems.ConditionsPlayer->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onConditionMenuEvents, this, menuBarItems.ConditionsAdd->GetId());
	this->Bind(wxEVT_MENU, &MainFrame::onConditionMenuEvents, this, menuBarItems.ConditionsRemove->GetId());

	this->Bind(wxEVT_MENU, &MainFrame::onDiceMenuEvents, this, menuBarItems.DiceRoll->GetId());
}

wxScrolled<wxPanel>* MainFrame::CreateMainPage(wxNotebook* parent)
{	
	wxScrolled<wxPanel>* panel = new wxScrolled<wxPanel>(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	panel->SetBackgroundColour(mainColour.first);
	panel->SetScrollRate(0, FromDIP(10));
	
	wxGridBagSizer* mainGridSizer = new wxGridBagSizer(5, 5);

	std::vector<std::pair<wxGBPosition, wxGBSpan>> items =
	{
		//Name Title
		{{0,0}, {1,1}},		//0

		//Health Plate
		{{0,4}, {1,2}},		//1  AC
		{{0,1}, {1,1}},		//2  SPEED
		{{0,2}, {1,1}},		//3	 HP
		{{0,3}, {1,1}},		//4  TEMP-HP

		//Stats
		{{1,0}, {6,1}},		//5

		//Saving Throws
		{{2,1}, {2,1}},		//6

		//Skills
		{{4,1}, {6,1}},		//7

		//Features
		{{2,2}, {5,2}},		//8

		//Tool Proficiencies
		{{3,4}, {2,2}},		//9   TOOL PROFS

		//Money
		{{7,0}, {1,1}},		//10

		//Below Health
		{{1,1}, {1,1}},		//11  INITIATIVE
		{{1,2}, {1,1}},		//12  SUB-HP
		{{1,3}, {1,1}},		//13  HIT-DIE
		{{1,4}, {1,2}},		//14  DEATH SAVES

		//Language Proficiencies
		{{2,4}, {1,2}},		//15   LANG PROFS

		//Sliders
		{{7,2}, {4,2}},		//16   SLIDERS

		//Conditions
		{{5,4}, {1,2}}		//17   CONDITIONS
	};

	wxPanel* p;
	std::pair<wxGBPosition, wxGBSpan> curItem;

	wxSize initSize(120, -1);
	wxColour panelBGColour(panelColour.first);

	curItem = items[0];
	p = CreateNamePanel(panel);
	p->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	
	curItem = items[1];
	p = CreateACPanel(panel);
	p->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[2];
	p = CreateSpeedPanel(panel);
	p->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[3];
	p = CreateHPPanel(panel);
	p->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[4];
	p = CreateTempHPPanel(panel);
	p->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[5];
	p = CreateStats(panel);
	p->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[6];
	p = CreateSavingThrows(panel);
	p->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[7];
	p = CreateSkillProf(panel);
	p->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxALIGN_CENTER_HORIZONTAL);

	curItem = items[8];
	p = CreateFeaturesPanel(panel);
	p->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[9];
	p = CreateToolProficiencies(panel);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[10];
	p = CreateMoney(panel);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[11];
	p = CreateInitiativePanel(panel);
	p->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[12];
	p = CreateSubHPPanel(panel);
	p->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[13];
	p = CreateDeathSavesPanel(panel);
	p->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[14];
	auto p1 = CreateHitDiePanel(panel);
	p1->SetBackgroundColour(panelBGColour);
	mainGridSizer->Add(p1, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[15];
	p = CreateLangProficiencies(panel);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[16];
	p = CreateSliderPanel(panel);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	curItem = items[17];
	p = CreatePlayerConditions(panel);
	mainGridSizer->Add(p, curItem.first, curItem.second, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);

	//mainGridSizer->AddGrowableRow(0);

	//mainGridSizer->AddGrowableCol(1);
	//mainGridSizer->AddGrowableCol(2);
	//mainGridSizer->AddGrowableCol(3);
	//mainGridSizer->AddGrowableCol(5);
	
	//mainGridSizer->SetMinSize(FromDIP(wxSize(masterPanel->GetSize().x, 300)));


	panel->SetSizer(mainGridSizer);
	panel->Layout();
	
	return panel;
}

wxScrolled<wxPanel>* MainFrame::CreateSpellsPage(wxNotebook* parent)
{
	wxSize initSize(120, -1);

	wxScrolled<wxPanel>* panel = new wxScrolled<wxPanel>(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	panel->SetBackgroundColour(mainColour.first);
	panel->SetScrollRate(0, FromDIP(10));

	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
	spellDesc.spellSplitter = new wxSplitterWindow(panel, wxID_ANY, wxDefaultPosition, initSize, wxSP_LIVE_UPDATE | wxSP_NOBORDER);

	wxPanel *left, *right;

	//wxColour headPlateColor(*wxBLACK);

	left = CreateSpellsDescPanel(static_cast<wxWindow*>(spellDesc.spellSplitter));
	setWindowColour(left, mainColour);
	//mainSizer->Add(p, 1, wxEXPAND);

	right = CreateSpellTreeList(static_cast<wxWindow*>(spellDesc.spellSplitter));
	setWindowColour(right, mainColour);
	///mainSizer->Add(p, 0, wxEXPAND);

	spellDesc.spellSplitter->SplitVertically(left, right);
	
	mainSizer->Add(spellDesc.spellSplitter, 1, wxEXPAND);

	spellDesc.spellSplitter->SetMinimumPaneSize(20);
	spellDesc.spellSplitter->SetSashGravity(1);

	panel->SetSizerAndFit(mainSizer);
	panel->Layout();
	spellDesc.spellSplitter->SetSashPosition(baseColSize.x*5 * 0.9);

	return panel;
}

wxScrolled<wxPanel>* MainFrame::CreateKnownSpellsPage(wxNotebook* parent)
{
	wxScrolled<wxPanel>* panel = new wxScrolled<wxPanel>(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	panel->SetBackgroundColour(mainColour.first);
	panel->SetScrollRate(FromDIP(10), FromDIP(10));

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* subSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticLine* horLine = new wxStaticLine(panel, wxID_ANY);

	int margin = 10;

	wxPanel* p;

	p = CreateKnownSpellAbilityPanel(panel);
	mainSizer->Add(p, 0, wxEXPAND | wxLEFT, margin);

	mainSizer->Add(-1,0);
	mainSizer->Add(horLine, 0, wxEXPAND | wxLEFT, margin);
	mainSizer->Add(-1, 30);

	p = CreateSpellDescription(
		panel, 
		*knownPagePanels.dispSpell, 
		knownPagePanels.Title, 
		knownPagePanels.Level, 
		knownPagePanels.CastingTime, 
		knownPagePanels.Range, 
		knownPagePanels.Duration, 
		knownPagePanels.Components, 
		knownPagePanels.Type, 
		knownPagePanels.AppClasses, 
		knownPagePanels.Description, 
		wxSize(350,-1));
	
	wxSize maxSize = FromDIP(wxSize(-1, 575));
	auto box = new wxStaticBoxSizer(wxVERTICAL, panel, "SELECTED SPELL");

	knownPagePanels.Description->SetMaxSize(maxSize);
	box->GetStaticBox()->SetForegroundColour(mainColour.second);
	box->GetStaticBox()->SetFont(box->GetStaticBox()->GetFont().MakeBold().MakeLarger());
	box->Add(p, 1, wxEXPAND | wxALL, 5);
	
	subSizer->Add(box, 0, wxEXPAND);

	p = CreateKnownSpellsAllLevels(panel);
	wxSize listSize = std::get<0>(knownPagePanels.SpellSlotLevelList[0])->GetParent()->GetSize();
	listSize.x *= 4;
	listSize.x += 50;
	p->SetMinSize(listSize);
	subSizer->Add(10, -1);
	subSizer->Add(p, 1, wxEXPAND | wxTOP, 10);


	subSizer->Layout();

	mainSizer->Add(subSizer, 1, wxEXPAND | wxLEFT, margin);

	panel->SetSizerAndFit(mainSizer);
	panel->Layout();
	return panel;
}

wxScrolled<wxPanel>* MainFrame::CreateSpellSlotsTable(wxNotebook* parent)
{
	wxScrolled<wxPanel>* panel = new wxScrolled<wxPanel>(parent, wxID_ANY);
	panel->SetBackgroundColour(mainColour.first);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxListView* SlotsTable = new wxListView(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_HRULES | wxLC_VRULES);

	SlotsTable->SetFont(BigFont1);
	setWindowColour(SlotsTable, mainColour);

	SlotsTable->AppendColumn("Level");
	SlotsTable->SetColumnWidth(0, 100);
	for (int i = 1; i < 10; ++i)
	{
		SlotsTable->AppendColumn(std::to_string(i));
		SlotsTable->SetColumnWidth(i, 50);
	}

	for (int i = 0; i < 20; ++i)
	{
		SlotsTable->InsertItem(i, std::to_string(i+1));
		for (int j = 1; j <= 9; ++j)
		{
			int x = character.getSpellSlots().getNumSlots(i + 1, j);
			std::string temp = "";
			if (x == -1)
				temp = "-";
			else
				temp = std::to_string(x);
			SlotsTable->SetItem(i, j, temp);
		}
	}

	mainSizer->Add(SlotsTable, 1, wxEXPAND);


	panel->SetSizerAndFit(mainSizer);
	panel->Layout();
	return panel;
}

wxScrolled<wxPanel>* MainFrame::CreateInventoryPage(wxNotebook* parent)
{
	wxScrolled<wxPanel>* panel = new wxScrolled<wxPanel>(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	panel->SetBackgroundColour(mainColour.first);
	panel->SetScrollRate(0, FromDIP(10));

	return panel;
}

wxScrolled<wxPanel>* MainFrame::CreateTestPanel(wxNotebook* parent)
{
	wxScrolled<wxPanel>* panel = new wxScrolled<wxPanel>(parent, wxID_ANY);
	panel->SetBackgroundColour(mainColour.first);

	panel->SetScrollRate(0, FromDIP(10));
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	std::vector<wxArtID> art =
	{
		wxART_ERROR,
		wxART_QUESTION,
		wxART_WARNING,
		wxART_INFORMATION,
		wxART_ADD_BOOKMARK,
		wxART_DEL_BOOKMARK,
		wxART_HELP_SIDE_PANEL,
		wxART_HELP_SETTINGS,
		wxART_HELP_BOOK,
		wxART_HELP_FOLDER,
		wxART_HELP_PAGE,
		wxART_GO_BACK,
		wxART_GO_FORWARD,
		wxART_GO_UP,
		wxART_GO_DOWN,
		wxART_GO_TO_PARENT,
		wxART_GO_HOME,
		wxART_GOTO_FIRST,
		wxART_GOTO_LAST,
		wxART_PRINT,
		wxART_HELP,
		wxART_TIP,
		wxART_REPORT_VIEW,
		wxART_LIST_VIEW,
		wxART_NEW_DIR,
		wxART_FOLDER,
		wxART_FOLDER_OPEN,
		wxART_GO_DIR_UP,
		wxART_EXECUTABLE_FILE,
		wxART_NORMAL_FILE,
		wxART_TICK_MARK,
		wxART_CROSS_MARK,
		wxART_MISSING_IMAGE,
		wxART_NEW,
		wxART_FILE_OPEN,
		wxART_FILE_SAVE,
		wxART_FILE_SAVE_AS,
		wxART_DELETE,
		wxART_COPY,
		wxART_CUT,
		wxART_PASTE,
		wxART_UNDO,
		wxART_REDO,
		wxART_PLUS,
		wxART_MINUS,
		wxART_CLOSE,
		wxART_QUIT,
		wxART_FIND,
		wxART_FIND_AND_REPLACE,
		wxART_HARDDISK,
		wxART_FLOPPY,
		wxART_CDROM,
		wxART_REMOVABLE
	};

	wxArtProvider prov;

	for (int i = 0; i < art.size(); ++i)
	{
		auto button = new wxButton(panel, wxID_ANY, "");
		std::string label = "";
		if (i + 1 < 10)
			label += "0";
		label += std::to_string(i+1);
		auto text = new wxStaticText(panel, wxID_ANY, label + " ........");
		text->SetFont(text->GetFont().Bold().Larger());
		text->SetForegroundColour(*wxWHITE);
		auto sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(text, 0, wxLEFT, 20);
		sizer->Add(button);
		
		wxBitmap logo = prov.GetBitmap(art[i], wxART_BUTTON);;
		button->SetBitmap(logo);
		
		mainSizer->Add(sizer);
		mainSizer->Add(-1, 5);

	}


	panel->SetSizer(mainSizer);
	panel->Layout();
	return panel;
}

//------------------------------
///MAIN PAGE PANELS
//------------------------------
wxPanel* MainFrame::CreateNamePanel(wxPanel* parent)
{
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize);
	setWindowColour(panel, mainColour);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	auto& charName = mainPagePanels.PlayerName = new wxStaticText(panel, wxID_ANY, character.getName());
	auto charClass = new wxStaticText(panel, wxID_ANY, character.getClass());

	wxFont nameFont;
	nameFont.SetPointSize(19);
	nameFont.SetFamily(wxFONTFAMILY_SCRIPT);
	nameFont.MakeBold();
	nameFont.MakeUnderlined();
	
	charName->SetFont(nameFont);
	charClass->SetFont(BigFont1);

	sizer->Add(-1, 10);
	sizer->Add(charName, 0, wxLEFT, 15);
	sizer->Add(baseColSize.x, 2);
	sizer->Add(charClass, 0, wxLEFT, 15);
	sizer->Add(-1, 10);

	panel->SetSizer(sizer);

	return panel;
}

wxPanel* MainFrame::CreateACPanel(wxPanel* parent)
{
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize*1.3);
	wxColour headPlateColor(wxColour(200, 100, 100));
	setWindowColour(panel, mainColour);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* subSizer1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* subSizer2 = new wxBoxSizer(wxVERTICAL);

	wxStaticText* AC_Label = new wxStaticText(panel, wxID_ANY, "AC", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);

	wxStaticLine* horLine = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	mainPagePanels.AC = new wxTextCtrl(panel, wxID_ANY, std::to_string(character.getAC()), wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
	mainPagePanels.AC_Shield = new wxCheckBox(panel, wxID_ANY, "Shield (+2)");
	mainPagePanels.AC_MageArmor = new wxCheckBox(panel, wxID_ANY, "Mage Armor");
	//mainPagePanels.AC_AddArmor = new wxButton(panel, wxID_ANY, "Set Armor");

	setWindowColour(mainPagePanels.AC, ctrlColour);

	mainSizer->Add(AC_Label, 1, wxEXPAND | wxTOP, 5);
	mainSizer->Add(horLine, 0, wxEXPAND | wxTOP | wxBOTTOM, 1);
	mainSizer->Add(-1, 5);
	
	subSizer1->Add(mainPagePanels.AC, 1, wxEXPAND | wxLEFT | wxRIGHT, 5);
	subSizer2->Add(mainPagePanels.AC_Shield);
	subSizer2->Add(-1, 2);
	subSizer2->Add(mainPagePanels.AC_MageArmor);
	//subSizer2->Add(-1, 2);
	//subSizer2->Add(mainPagePanels.AC_AddArmor);
	subSizer1->Add(subSizer2, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	mainSizer->Add(subSizer1, 2, wxEXPAND | wxLEFT | wxRIGHT, 5);
	mainSizer->Add(-1, 5);

	AC_Label->SetFont(BigFont1);
	mainPagePanels.AC->SetFont(BigFont1.Larger())
		;
	mainPagePanels.AC->SetValidator(wxTextValidator(wxFILTER_NUMERIC));

	panel->SetSizerAndFit(mainSizer);
	panel->Layout();

	return panel;
}

wxPanel* MainFrame::CreateHPPanel(wxPanel* parent)
{
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize);
	setWindowColour(panel, mainColour);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	auto& headHP = mainPagePanels.HPText = new wxStaticText(panel, wxID_ANY,
		"HP | " + std::to_string(character.getTotHP()), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);

	wxStaticLine* horLine = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	mainPagePanels.HP = new wxSpinCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);

	sizer->Add(headHP, 1, wxEXPAND |  wxTOP, 5);
	sizer->Add(horLine, 0, wxEXPAND | wxTOP | wxBOTTOM, 1);
	sizer->Add(-1, 5);
	sizer->Add(mainPagePanels.HP, 2, wxEXPAND | wxLEFT | wxRIGHT, 13);
	sizer->Add(-1, 5);


	mainPagePanels.HP->SetValue(character.getTotHP());
	mainPagePanels.HP->SetMin(0);
	mainPagePanels.HP->SetMax(character.getTotHP());

	setWindowColour(mainPagePanels.HP, ctrlColour);

	headHP->SetFont(BigFont1);
	mainPagePanels.HP->SetFont(BigFont1.Larger());

	panel->SetSizer(sizer);

	return panel;
}

wxPanel* MainFrame::CreateTempHPPanel(wxPanel* parent)
{
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize);
	setWindowColour(panel, mainColour);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* subSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* headHP = new wxStaticText(panel, wxID_ANY, "Temp HP", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxStaticLine* horLine = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	mainPagePanels.TempHP = new wxSpinCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	mainPagePanels.GiveTempHPButton = new wxButton(panel, wxID_ANY, "");

	mainPagePanels.GiveTempHPButton->SetBitmap(wxArtProvider().GetBitmap(wxART_PLUS));

	mainPagePanels.GiveTempHPButton->SetMaxSize(wxSize(30,-1));

	sizer->Add(headHP, 1, wxEXPAND | wxTOP, 5);
	sizer->Add(horLine, 0, wxEXPAND | wxTOP | wxBOTTOM, 1);
	sizer->Add(-1, 5);
	subSizer->Add(mainPagePanels.TempHP, 1, wxEXPAND | wxLEFT | wxRIGHT, 5);
	subSizer->Add(5,-1);
	subSizer->Add(mainPagePanels.GiveTempHPButton, 0, wxEXPAND);
	sizer->Add(subSizer, 2, wxEXPAND | wxLEFT | wxRIGHT, 7);
	sizer->Add(-1, 5);

	mainPagePanels.TempHP->SetValue(0);
	mainPagePanels.TempHP->SetMin(0);

	setWindowColour(mainPagePanels.TempHP, ctrlColour);

	headHP->SetFont(BigFont1);
	mainPagePanels.TempHP->SetFont(BigFont1.Larger());

	panel->SetSizer(sizer);

	return panel;
}

wxPanel* MainFrame::CreateSpeedPanel(wxPanel* parent)
{
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize);
	setWindowColour(panel, mainColour);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* headSpeed = new wxStaticText(panel, wxID_ANY,
		"Speed", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);

	wxStaticLine* horLine = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	mainPagePanels.Speed = new wxTextCtrl(panel, wxID_ANY,	std::to_string(character.getSpeed()), wxDefaultPosition, wxDefaultSize, wxTE_CENTER);

	sizer->Add(headSpeed, 1, wxEXPAND | wxTOP, 5);
	sizer->Add(horLine, 0, wxEXPAND | wxTOP | wxBOTTOM, 1);
	sizer->Add(-1, 5);
	sizer->Add(mainPagePanels.Speed, 2, wxEXPAND | wxLEFT | wxRIGHT, 13);
	sizer->Add(-1, 5);

	setWindowColour(mainPagePanels.Speed, ctrlColour);

	mainPagePanels.Speed->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
	
	headSpeed->SetFont(BigFont1);	
	mainPagePanels.Speed->SetFont(BigFont1.Larger());

	panel->SetSizer(sizer);

	return panel;
}

wxPanel* MainFrame::CreateStats(wxPanel* parent)
{
	int margin = 15;
	using std::to_string;

	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize);
	setWindowColour(panel, mainColour);

	//wxStaticText* profBonus = new wxStaticText(panel, wxID_ANY, "Profeciency: " + std::to_string(character.getProfBonus()));

	std::string StatNames[6] = {"STR", "DEX", "CON","INT", "WIS", "CHA"};
	Skills skillNames[6] =	{Skills::Strength, Skills::Dexterity, Skills::Constitution, Skills::Intelligence, Skills::Wisdom, Skills::Charisma};

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxFont titleFont(wxFontInfo(wxSize(0,15)));
	titleFont.MakeBold();

	wxSize boxSize(40,-1);
	wxSize wideSize(120,-1);
	
	std::vector<wxStaticBoxSizer*> boxes;
	std::vector<wxStaticLine*> horLine;
	std::vector<wxStaticText*> titles;

	Stats* stat = character.getStats();

	int statsArray[6] = { stat->Str,stat->Dex, stat->Con, stat->Int, stat->Wis, stat->Cha };

	for (int i = 0; i < 6; ++i)
	{
		wxTextCtrl *val, *mod;
		boxes.push_back(new wxStaticBoxSizer(wxVERTICAL, panel, ""));
		horLine.push_back(new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wideSize));
		titles.push_back(new wxStaticText(panel, wxID_ANY, StatNames[i], wxDefaultPosition, wideSize, wxALIGN_CENTER_HORIZONTAL));
		val = new wxTextCtrl(panel, wxID_ANY, to_string(statsArray[i]), wxDefaultPosition, boxSize, wxTE_CENTER | wxTE_READONLY);
		mod = new wxTextCtrl(panel, wxID_ANY, to_string(character.getSkillMod(skillNames[i])), wxDefaultPosition, boxSize, wxTE_CENTER | wxTE_READONLY);

		titles[i]->SetFont(titleFont);

		mainPagePanels.Stat_TextCtrls.push_back({val, mod});
		mainPagePanels.Stat_TextCtrls[i].first->SetFont(BigFont2);
		mainPagePanels.Stat_TextCtrls[i].second->SetFont(BigFont2);

		wxBoxSizer* subSizer = new wxBoxSizer(wxHORIZONTAL);

		subSizer->Add(mainPagePanels.Stat_TextCtrls[i].first, 0, wxALIGN_CENTER);
		subSizer->Add(10, -1);
		subSizer->Add(mainPagePanels.Stat_TextCtrls[i].second, 0, wxALIGN_CENTER);

		boxes[i]->Add(-1, 2);
		boxes[i]->Add(titles[i], 1, wxEXPAND);
		boxes[i]->Add(horLine[i], 0, wxALIGN_CENTER | wxBOTTOM, 6);
		boxes[i]->Add(subSizer, 2, wxALIGN_CENTER);
		boxes[i]->Add(-1, 2);

		setWindowColour(mainPagePanels.Stat_TextCtrls[i].first, ctrlColour);
		setWindowColour(mainPagePanels.Stat_TextCtrls[i].second, ctrlColour);

		mainPagePanels.Stat_TextCtrls[i].first->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
		mainPagePanels.Stat_TextCtrls[i].second->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
	}
	
	wxStaticBoxSizer* profBox = new wxStaticBoxSizer(wxVERTICAL, panel, "Profeciency Bonus");
	wxTextCtrl* profVal = new wxTextCtrl(panel, wxID_ANY, to_string(character.getProfBonus()), wxDefaultPosition, 
		wxSize(30, -1), wxTE_READONLY | wxTE_CENTER);

	profBox->GetStaticBox()->SetForegroundColour(mainColour.second);
	profBox->GetStaticBox()->SetFont(profBox->GetStaticBox()->GetFont().MakeBold().MakeLarger().MakeUnderlined());

	profBox->Add(profVal, 1, wxEXPAND | wxLEFT | wxRIGHT, margin);
	profVal->SetMaxSize(wxSize(-1,30));
	profVal->SetFont(titleFont);

	setWindowColour(profVal, ctrlColour);

	mainSizer->Add(profBox, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, margin);


	for (int i = 0; i < 6; ++i)
		mainSizer->Add(boxes[i], 1, wxALIGN_CENTER | wxLEFT | wxRIGHT, margin);

	mainSizer->Add(-1,5);
	panel->SetSizer(mainSizer);

	return panel;
}

wxPanel* MainFrame::CreateSavingThrows(wxPanel* parent)
{
	wxSize tempSize = mainPagePanels.HP->GetParent()->GetSize();
	tempSize.y = -1;
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, tempSize);
	setWindowColour(panel, mainColour);

	wxStaticBoxSizer* stSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Saving Throws");

	stSizer->GetStaticBox()->SetForegroundColour(mainColour.second);
	stSizer->GetStaticBox()->SetFont(stSizer->GetStaticBox()->GetFont().MakeBold().MakeLarger());

	//std::wstring profMarker = std::wstring(1, char(159));
	std::vector<Skills> tempSkills =
	{
		Skills::Strength, Skills::Dexterity, Skills::Constitution, Skills::Intelligence, Skills::Wisdom, Skills::Charisma
	};

	wxStaticText* savingThrowName = nullptr;
	wxTextCtrl* savingThrowValue = nullptr;
	
	for (int i = 0; i < 6; ++i)
	{
		savingThrowName = new wxStaticText(panel, wxID_ANY, "");
		savingThrowValue = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
		savingThrowValue->SetValidator(wxTextValidator(wxFILTER_NUMERIC));

		savingThrowName->SetFont(SkillFont);
		setWindowColour(savingThrowValue, ctrlColour);

		makeSavingThrowPair(savingThrowName, savingThrowValue, tempSkills[i]);
		mainPagePanels.SavingThrows.push_back({ savingThrowName, savingThrowValue });
	}

	wxFlexGridSizer* gridSizer = new wxFlexGridSizer(6, 2, 2, 10);

	for (int i = 0; i < 6; ++i)
	{
		gridSizer->Add(mainPagePanels.SavingThrows[i].second,1);
		mainPagePanels.SavingThrows[i].second->SetMaxSize(wxSize(40, -1));
		gridSizer->Add(mainPagePanels.SavingThrows[i].first,4);
	}

	gridSizer->AddGrowableCol(1);

	stSizer->Add(-1, 2);
	stSizer->Add(gridSizer, 0, wxALIGN_LEFT | wxLEFT, 10);
	stSizer->Add(-1, 2);

	panel->SetSizer(stSizer);
	panel->Layout();

	updateSavingThrows();
	return panel;
}

wxPanel* MainFrame::CreateSkillProf(wxPanel* parent)
{
	wxSize tempSize = mainPagePanels.HP->GetParent()->GetSize();
	tempSize.y = -1;
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, tempSize);
	setWindowColour(panel, mainColour);

	wxStaticBoxSizer* skillSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Skills");
	
	skillSizer->GetStaticBox()->SetForegroundColour(mainColour.second);
	skillSizer->GetStaticBox()->SetFont(skillSizer->GetStaticBox()->GetFont().MakeBold().MakeLarger());

	std::wstring profMarker = std::wstring(1, char(159));
	//std::wstring profMarker = L"\t*";
	
	std::vector<Skills> curSkill =
	{
		Skills::Acrobatics,
		Skills::Animal_Handling,
		Skills::Arcana,
		Skills::Athletics,
		Skills::Deception,
		Skills::History,
		Skills::Insight,
		Skills::Intimidation,
		Skills::Investigation,
		Skills::Medicine,
		Skills::Nature,
		Skills::Perception,
		Skills::Performance,
		Skills::Persuasion,
		Skills::Religion,
		Skills::Sleight_of_Hand,
		Skills::Stealth,
		Skills::Survival
	};	
	
	wxStaticText* skillName = nullptr;
	wxTextCtrl* skillValue = nullptr;

	for (int i = 0; i < curSkill.size(); ++i)
	{
		skillName = new wxStaticText(panel, wxID_ANY, "");
		skillValue = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
		skillValue->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
	
		skillName->SetFont(SkillFont);
		setWindowColour(skillValue, ctrlColour);
		skillName->SetForegroundColour(panelColour.second);

		makeSkillPair(skillName, skillValue, curSkill[i]);
		mainPagePanels.Skills.push_back({skillName, skillValue});
	}

	wxFlexGridSizer* gridSizer = new wxFlexGridSizer(curSkill.size(), 2, 2, 10);

	for (int i = 0; i < curSkill.size(); ++i)
	{
		gridSizer->Add(mainPagePanels.Skills[i].second, 1);
		mainPagePanels.Skills[i].second->SetMaxSize(wxSize(40, -1));
		gridSizer->Add(mainPagePanels.Skills[i].first, 4);
	}

	gridSizer->AddGrowableCol(1);

	skillSizer->Add(-1,2);
	skillSizer->Add(gridSizer, 0, wxLEFT | wxLEFT, 10);
	skillSizer->Add(-1, 2);

	updateSkills();
	panel->SetSizer(skillSizer);
	panel->Layout();

	return panel;
}

wxPanel* MainFrame::CreateFeaturesPanel(wxPanel* parent)
{
	wxSize tempSize;
	tempSize.x = baseColSize.x * 2;
	tempSize.y = -1;

	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, tempSize);
	setWindowColour(panel, mainColour);
	auto mainSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Features");
	wxBoxSizer* subSizer = new wxBoxSizer(wxHORIZONTAL);

	mainSizer->GetStaticBox()->SetForegroundColour(mainColour.second);
	mainSizer->GetStaticBox()->SetFont(mainSizer->GetStaticBox()->GetFont().MakeBold().MakeLarger());

	wxFont curFont(wxFont(wxFontInfo(wxSize(0, 13))));
	curFont.SetWeight(wxFontWeight::wxFONTWEIGHT_BOLD);

	mainPagePanels.Feature_Title = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(tempSize.x, -1));
	mainPagePanels.Feature_Desc = new wxHtmlWindow(panel, wxID_ANY, wxDefaultPosition, wxSize(tempSize.x, -1), wxHW_SCROLLBAR_AUTO);
	
	mainPagePanels.Feature_FullList = new wxListBox(panel, wxID_ANY, wxDefaultPosition, tempSize, 0, {},wxLB_SINGLE);

	mainPagePanels.Feature_AddButton = new wxButton(panel, wxID_ANY, "", wxDefaultPosition, buttonSize, wxBU_EXACTFIT);
	mainPagePanels.Feature_RemoveButton = new wxButton(panel, wxID_ANY, "", wxDefaultPosition, buttonSize, wxBU_EXACTFIT);
	mainPagePanels.Feature_AddButton->SetMinSize(buttonSize);
	mainPagePanels.Feature_RemoveButton->SetMinSize(buttonSize);

	wxArtProvider prov;
	mainPagePanels.Feature_AddButton->SetBitmap(prov.GetBitmap(wxART_PLUS, wxART_BUTTON));
	mainPagePanels.Feature_RemoveButton->SetBitmap(prov.GetBitmap(wxART_MINUS, wxART_BUTTON));

	mainPagePanels.Feature_Title->SetFont(curFont);
	mainPagePanels.Feature_FullList->SetFont(curFont);

	auto listTitle = new wxStaticText(panel, wxID_ANY, "Features List", wxDefaultPosition, wxDefaultSize, wxALIGN_BOTTOM);
	listTitle->SetFont(listTitle->GetFont().Bold().Larger());

	wxSize titleSpacing = mainPagePanels.Feature_Desc->GetSize();
	titleSpacing.y = -1;
	titleSpacing.x -= mainPagePanels.Feature_AddButton->GetSize().x;
	titleSpacing.x -= mainPagePanels.Feature_RemoveButton->GetSize().x;
	titleSpacing.x -= listTitle->GetSize().x;
	titleSpacing.x -= 16;

	subSizer->Add(listTitle,0);
	subSizer->Add(titleSpacing.x, -1);
	subSizer->Add(mainPagePanels.Feature_RemoveButton, 0, wxALIGN_CENTER);
	subSizer->Add(4, -1);
	subSizer->Add(mainPagePanels.Feature_AddButton, 0 , wxALIGN_CENTER);

	setWindowColour(mainPagePanels.Feature_Title, descColour);
	setWindowColour(mainPagePanels.Feature_Desc, descColour);
	setWindowColour(mainPagePanels.Feature_FullList, listColour);


	mainSizer->Add(mainPagePanels.Feature_Title, 0, wxEXPAND);
	mainSizer->Add(-1, 5);
	mainSizer->Add(mainPagePanels.Feature_Desc, 4, wxEXPAND);
	mainSizer->Add(-1, 5);
	mainSizer->Add(subSizer, 0, wxALIGN_LEFT);
	mainSizer->Add(-1, 5);
	mainSizer->Add(mainPagePanels.Feature_FullList, 3, wxEXPAND);

	panel->SetSizer(mainSizer);
	return panel;
}

wxPanel* MainFrame::CreateLangProficiencies(wxPanel* parent)
{
	wxSize tempSize;
	//tempSize.x = baseColSize.x*1.3;
	tempSize.x = mainPagePanels.AC->GetParent()->GetSize().x;

	tempSize.y = -1;
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, tempSize);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	//panel->SetBackgroundColour(DescColor.first);

	auto& text = std::get<0>(mainPagePanels.LangProf) = new wxStaticText(panel, wxID_ANY, "Languages");
	auto& add = std::get<1>(mainPagePanels.LangProf);
	auto& rem = std::get<2>(mainPagePanels.LangProf);
	auto& list = std::get<3>(mainPagePanels.LangProf);

	makeAddRemList(text, add, rem, list, sizer, panel);

	setWindowColour(panel, descColour);
	setWindowColour(list, listColour);

	panel->SetSizer(sizer);
	panel->Layout();

	return panel;
}

wxPanel* MainFrame::CreateToolProficiencies(wxPanel* parent)
{
	wxSize tempSize;
	//tempSize.x = baseColSize.x*1.3;
	tempSize.x = mainPagePanels.AC->GetParent()->GetSize().x;

	tempSize.y = -1;
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, tempSize);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	auto& text = std::get<0>(mainPagePanels.ToolProf) = new wxStaticText(panel, wxID_ANY, "Profeciencies");
	auto& add = std::get<1>(mainPagePanels.ToolProf);
	auto& rem = std::get<2>(mainPagePanels.ToolProf);
	auto& list = std::get<3>(mainPagePanels.ToolProf);

	makeAddRemList(text, add, rem, list, sizer, panel);

	setWindowColour(panel, descColour);
	setWindowColour(list, listColour);

	panel->SetSizer(sizer);
	panel->Layout();

	return panel;
}

wxPanel* MainFrame::CreatePlayerConditions(wxPanel* parent)
{
	wxSize tempSize;
	//tempSize.x = baseColSize.x*1.3;
	tempSize.x = mainPagePanels.AC->GetParent()->GetSize().x;

	tempSize.y = -1;
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, tempSize);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	mainPagePanels.EL_Conditions = new wxEditableListBox(panel, wxID_ANY, "Player Conditions", wxDefaultPosition, wxDefaultSize, wxEL_NO_REORDER);

	setWindowColour(mainPagePanels.EL_Conditions->GetListCtrl(), descColour);
	setWindowColour(mainPagePanels.EL_Conditions->GetParent(), descColour);

	mainPagePanels.EL_Conditions->SetupColours();
	mainPagePanels.EL_Conditions->SetMaxSize(FromDIP(wxSize(-1, 120)));
	mainPagePanels.EL_Conditions->SetFont(mainPagePanels.EL_Conditions->GetFont().Bold());

	auto list = mainPagePanels.EL_Conditions->GetListCtrl();

	for (auto it = playerConditions.begin(); it != playerConditions.end(); ++it)
	{
		int count = list->GetItemCount();
		list->InsertItem(count, it->second.title);
	}

	sizer->Add(mainPagePanels.EL_Conditions, 1, wxEXPAND);

	panel->SetSizer(sizer);
	panel->Layout();

	return panel;
}

wxPanel* MainFrame::CreateMoney(wxPanel* parent)
{
	wxSize curSize = baseColSize;
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, curSize);
	setWindowColour(panel, panelColour);
	std::string tempStr[4] = { "Platinum", "Gold", "Silver", "Copper" };
	wxFont curFont(wxFont(wxFontInfo(wxSize(0, 16))));
	curFont.MakeBold();

	int gap = 5;

	auto mainSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Money");

	sizer->GetStaticBox()->SetForegroundColour(mainColour.second);
	sizer->GetStaticBox()->SetFont(sizer->GetStaticBox()->GetFont().MakeBold().MakeLarger());

	std::vector<wxStaticBoxSizer*> box;
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	//wxSize butSiz = FromDIP(wxSize(0.5 * curSize.x - 40, 25));
	wxSize butSiz(buttonSize.x, buttonSize.y);
	mainPagePanels.moneyButtons.first = new wxButton(panel, wxID_ANY, "", wxDefaultPosition, butSiz, wxBU_EXACTFIT);
	mainPagePanels.moneyButtons.second = new wxButton(panel, wxID_ANY, "", wxDefaultPosition, butSiz, wxBU_EXACTFIT);

	wxArtProvider prov;
	mainPagePanels.moneyButtons.first->SetBitmap(prov.GetBitmap(wxART_MINUS, wxART_BUTTON));
	mainPagePanels.moneyButtons.second->SetBitmap(prov.GetBitmap(wxART_PLUS, wxART_BUTTON));

	buttonSizer->Add(mainPagePanels.moneyButtons.first, 1, wxALIGN_CENTER | wxRIGHT, 2);
	buttonSizer->Add(mainPagePanels.moneyButtons.second, 1, wxALIGN_CENTER | wxLEFT, 2);

	sizer->Add(curSize.x, 5);
	sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 25);
	sizer->Add(curSize.x, 10);

	for (int i = 0; i < 4; ++i)
	{
		box.push_back(new wxStaticBoxSizer(wxVERTICAL, panel, tempStr[i]));
		wxSpinCtrl* tempSpin = new wxSpinCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL | wxSP_ARROW_KEYS);
		wxSize spinSize = tempSpin->GetSize();
		spinSize.y *= 1.33;
		tempSpin->SetMinSize(spinSize);
		
		mainPagePanels.moneyVals.push_back(tempSpin);

		box[i]->Add(mainPagePanels.moneyVals[i], 0, wxEXPAND);
		box[i]->GetStaticBox()->SetForegroundColour(mainColour.second);
		box[i]->GetStaticBox()->SetFont(box[i]->GetStaticBox()->GetFont().MakeBold().MakeLarger());

		mainPagePanels.moneyVals[i]->SetMax(1e5);
		mainPagePanels.moneyVals[i]->SetFont(curFont);
		setWindowColour(mainPagePanels.moneyVals[i], ctrlColour);
		
		sizer->Add(box[i], 0, wxEXPAND | wxLEFT | wxRIGHT, 25);
		sizer->Add(curSize.x, 5);
	}

	mainSizer->Add(sizer, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

	panel->SetSizer(mainSizer);
	panel->Layout();

	TransferDataToWindow();

	return panel;
}

wxPanel* MainFrame::CreateSubHPPanel(wxPanel* parent)
{
	using std::get;
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	auto& healText = get<0>(mainPagePanels.HealToX) = new wxStaticText(panel, wxID_ANY, "Heal to % ", wxDefaultPosition, wxDefaultSize,
		wxALIGN_RIGHT);
	auto& healSpin = get<1>(mainPagePanels.HealToX) = new wxSpinCtrl(panel, wxID_ANY, "100", wxDefaultPosition, wxDefaultSize, 
		wxSP_ARROW_KEYS | wxALIGN_CENTER_HORIZONTAL);
	auto& healButton = get<2>(mainPagePanels.HealToX) = new wxButton(panel, wxID_ANY, "Heal", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	auto& hpText = get<0>(mainPagePanels.MaxHPBonus) = new wxStaticText(panel, wxID_ANY, "Max HP Mod: ");
	auto& hpSpin = get<1>(mainPagePanels.MaxHPBonus) = new wxSpinCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_CENTER_HORIZONTAL);

	wxBoxSizer* sizer1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);

	healSpin->SetMax(100);
	healSpin->SetMin(0);
	hpSpin->SetMax(1e5);
	hpSpin->SetMin(0);
	
	sizer1->Add(5,-1);
	sizer1->Add(healText, 0, wxALIGN_CENTER);
	sizer1->Add(5, -1);
	sizer1->Add(healSpin, 0, wxALIGN_CENTER);
	sizer1->Add(5, -1);
	sizer1->Add(healButton, 0, wxALIGN_CENTER);
	sizer1->Add(5, -1);

	sizer2->Add(5, -1);
	sizer2->Add(hpText, 0, wxALIGN_CENTER);
	sizer2->Add(hpSpin, 0, wxALIGN_CENTER);
	sizer2->Add(5, -1);

	sizer->Add(-1, 5);
	sizer->Add(sizer1, 0, wxALIGN_CENTER);
	sizer->Add(-1,5);
	sizer->Add(sizer2, 0, wxALIGN_CENTER);
	sizer->Add(-1, 5);

	setWindowColour(healSpin, ctrlColour);
	setWindowColour(hpSpin, ctrlColour);
	setWindowColour(hpText, panelColour);
	setWindowColour(healText, panelColour);

	panel->SetSizer(sizer);
	return panel;
}

wxPanel* MainFrame::CreateInitiativePanel(wxPanel* parent)
{
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
	auto& Init = mainPagePanels.InitMod.first = new wxTextCtrl(panel, wxID_ANY, std::to_string(character.getStats()->Dex), wxDefaultPosition, wxDefaultSize,
		wxALIGN_CENTER_HORIZONTAL | wxTE_READONLY);
	auto& initMod = mainPagePanels.InitMod.second = new wxTextCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 
		wxALIGN_CENTER_HORIZONTAL | wxTE_READONLY);

	auto initModText = new wxStaticText(panel, wxID_ANY, "Initiative Mod");
	
	Init->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
	initMod->SetValidator(wxTextValidator(wxFILTER_NUMERIC));

	Init->SetFont(Init->GetFont().Bold().Larger());
	initMod->SetFont(initMod->GetFont().Bold());

	int initiative = character.getInitiative();

	Init->SetValue(std::to_string(initiative));

	auto InitBox = new wxStaticBoxSizer(wxHORIZONTAL, panel, "Inititative");
	auto subSizer = new wxBoxSizer(wxHORIZONTAL);
	
	InitBox->Add(Init, 1, wxEXPAND | wxLEFT | wxRIGHT, 30);

	subSizer->Add(initModText, 0, wxTOP, 5);
	subSizer->Add(5, -1);
	subSizer->Add(initMod, 0, wxBOTTOM, 5);

	sizer->Add(-1, 5);
	sizer->Add(InitBox, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
	sizer->Add(-1,5);
	sizer->Add(subSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 20);
	sizer->Add(-1, 5);

	InitBox->GetStaticBox()->SetForegroundColour(panelColour.second);
	InitBox->GetStaticBox()->SetFont(InitBox->GetStaticBox()->GetFont().MakeBold().MakeLarger().MakeLarger());
	
	setWindowColour(Init, ctrlColour);
	setWindowColour(initMod, ctrlColour);
	initModText->SetForegroundColour(panelColour.second);
	initModText->SetFont(initModText->GetFont().MakeBold());
	panel->SetSizer(sizer);
	return panel;
}

wxPanel* MainFrame::CreateDeathSavesPanel(wxPanel* parent)
{

	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	DeathSavesControl* deathSaves = new DeathSavesControl(panel, wxID_ANY, panelColour.second, panelColour.first);

	sizer->Add(deathSaves, 0, wxALIGN_CENTER);

	panel->SetSizer(sizer);
	Layout();
	return panel;
}

wxScrolled<wxPanel>* MainFrame::CreateSliderPanel(wxPanel* parent)
{
	auto panel = new wxScrolled<wxPanel>(parent, wxID_ANY, wxDefaultPosition, 2*baseColSize, wxVSCROLL);
	panel->SetBackgroundColour(mainColour.first);
	auto sizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Sliders");
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	panel->SetScrollRate(0, FromDIP(10));

	auto remButton = mainPagePanels.SlidersButtons.first = new wxButton(panel, wxID_ANY, "", wxDefaultPosition, buttonSize);
	auto addButton = mainPagePanels.SlidersButtons.second= new wxButton(panel, wxID_ANY, "", wxDefaultPosition, buttonSize);

	wxArtProvider prov;
	remButton->SetBitmap(prov.GetBitmap(wxART_MINUS, wxART_BUTTON));
	addButton->SetBitmap(prov.GetBitmap(wxART_PLUS, wxART_BUTTON));

	buttonSizer->Add(remButton, 0, wxALL, 2);
	buttonSizer->Add(addButton, 0, wxALL, 2);

	setWindowColour(sizer->GetStaticBox(), mainColour);

	sizer->GetStaticBox()->SetFont(sizer->GetStaticBox()->GetFont().Larger().Bold());

	sizer->Add(buttonSizer, 0, wxALIGN_RIGHT);
	auto horLine = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxSize(1.5*baseColSize.x, -1));
	sizer->Add(horLine, 0, wxEXPAND);

	panel->SetSizer(sizer);
	panel->FitInside();
	panel->Layout();

	return panel;
}

wxScrolled<wxPanel>* MainFrame::CreateHitDiePanel(wxPanel* parent)
{
	wxSize ACSize = mainPagePanels.AC->GetParent()->GetSize();
	ACSize.y = -1;

	auto panel = new wxScrolled<wxPanel>(parent, wxID_ANY, wxDefaultPosition, ACSize);
	auto sizer = new wxBoxSizer(wxVERTICAL);
	panel->SetScrollRate(0, FromDIP(10));
	panel->SetMaxSize(FromDIP(wxSize(this->GetSize().x, 200)));
	auto& charClass = character.getCharClass();

	std::vector<CharClass> tempClasses;

	for (auto i = charClass.begin(); i != charClass.end(); ++i)
	{
		int charLevel = i->level;
		bool classExists = false;
		for (auto j = tempClasses.begin(); j < tempClasses.end(); ++j)
		{
			if (j->classType == i->classType)
			{
				j->level += i->level;
				classExists = true;
			}
		}

		if (!classExists)
			tempClasses.push_back(*i);
	}
	
	sizer->Add(-1, 5);

	for (auto it = tempClasses.begin(); it != tempClasses.end(); ++it)
	{
		auto text = new wxStaticText(panel, wxID_ANY, "");
		auto spin = new wxSpinCtrl(panel, wxID_ANY, "0", wxDefaultPosition, FromDIP(wxSize(40,-1)), wxALIGN_CENTER_HORIZONTAL);
		
		setWindowColour(text, panelColour);
		setWindowColour(spin, ctrlColour);

		auto subSizer = new wxBoxSizer(wxHORIZONTAL);

		std::string name = it->getClassName();
		DieType die = it->getHitDie();
		std::string tempStr = "";
		tempStr = " of " + std::to_string(it->level);

		switch (die)
		{
		case DieType::d6:
			tempStr += "d6";
			break;
		case DieType::d8:
			tempStr += "d8";
			break;
		case DieType::d10:
			tempStr += "d10";
			break;
		case DieType::d12:
			tempStr += "d12";
			break;
		case DieType::d4:
		case DieType::d20:
		case DieType::d100:
			tempStr += "d(4,20,100)";
			break;
		case DieType::error:
			tempStr += "d(ERR)";
			break;
		default:
			tempStr += "d(DEF)";
			break;
		}

		tempStr += " - " + name;

		text->SetLabel(tempStr);
		
		spin->SetMax(it->level);
		spin->SetMin(0);
		spin->SetValue(it->level);

		subSizer->Add(spin, 0);
		subSizer->Add(3, -1);
		subSizer->Add(text, 0);

		sizer->Add(-1, 5);
		sizer->Add(subSizer, 0, wxEXPAND | wxLEFT, 15);

		mainPagePanels.HitDie.push_back({ spin , text});
	}
	wxSize tempSize = std::get<0>(mainPagePanels.MaxHPBonus)->GetParent()->GetSize();
	tempSize.x = -1;
	//panel->SetMaxSize(tempSize);

	panel->SetSizer(sizer);
	//panel->Layout();

	return panel;
}

//------------------------------
///SPELL PANELS
//------------------------------
wxPanel* MainFrame::CreateSpellsDescPanel(wxWindow* parent)
{
	return CreateSpellDescription(static_cast<wxPanel*>(parent), *spellDesc.dispSpell, spellDesc.Title, spellDesc.Level, spellDesc.CastingTime, spellDesc.Range, spellDesc.Duration, spellDesc.Components, spellDesc.Type, spellDesc.AppClasses, spellDesc.Description, spellDesc.ColWidth);
}

wxPanel* MainFrame::CreateSpellTreeList(wxWindow* parent)
{
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize);
	//wxWrapSizer* mainSizer =  new wxWrapSizer(wxHORIZONTAL);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	spellDesc.spellList = new wxTreeCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_ROW_LINES | wxTR_SINGLE | wxTR_HIDE_ROOT);
	
	//wxTreeItemId AllSpellsId;
	wxTreeItemId spellRootId;
	
	spellRootId = spellDesc.spellList->AddRoot("Root");

	spellDesc.spellLevelId[0] = spellDesc.spellList->AppendItem(spellRootId, "Cantrips");
	spellDesc.spellLevelId[1] = spellDesc.spellList->AppendItem(spellRootId, "Spell Level 1");
	spellDesc.spellLevelId[2] = spellDesc.spellList->AppendItem(spellRootId, "Spell Level 2");
	spellDesc.spellLevelId[3] = spellDesc.spellList->AppendItem(spellRootId, "Spell Level 3");
	spellDesc.spellLevelId[4] = spellDesc.spellList->AppendItem(spellRootId, "Spell Level 4");
	spellDesc.spellLevelId[5] = spellDesc.spellList->AppendItem(spellRootId, "Spell Level 5");
	spellDesc.spellLevelId[6] = spellDesc.spellList->AppendItem(spellRootId, "Spell Level 6");
	spellDesc.spellLevelId[7] = spellDesc.spellList->AppendItem(spellRootId, "Spell Level 7");
	spellDesc.spellLevelId[8] = spellDesc.spellList->AppendItem(spellRootId, "Spell Level 8");
	spellDesc.spellLevelId[9] = spellDesc.spellList->AppendItem(spellRootId, "Spell Level 9");

	spellDesc.fillAllSpellTree(allSpells);

	//spellList->Expand(spellRootId);
	
	spellDesc.spellList->SetBackgroundColour(listColour.first);
	spellDesc.spellList->SetForegroundColour(listColour.second);

	setWindowColour(spellDesc.spellList, listColour);

	spellDesc.spellList->SetMinSize(wxSize(200, 100));
	spellDesc.spellList->SetFont(spellDesc.spellList->GetFont().MakeLarger());
	
	for (int i = 0; i < 10; ++i)
	{
		//spellDesc.spellList->SetItemFont(spellDesc.spellLevelId[i], spellDesc.spellList->GetFont().MakeBold());
		spellDesc.spellList->SetItemBold(spellDesc.spellLevelId[i], true);
	}

	spellDesc.spellSearch = new wxSearchCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200,-1));
	spellDesc.spellSearch->ShowCancelButton(true);
	spellDesc.spellSearch->ShowSearchButton(true);

	mainSizer->Add(spellDesc.spellSearch, 0, wxEXPAND| wxTOP | wxLEFT, 5);
	mainSizer->Add(spellDesc.spellList, 1, wxEXPAND | wxTOP | wxLEFT, 5);

	//spellDesc.fullSpellList = new wxTreeCtrl(spellDesc.spellList);
	//spellDesc.fullSpellList->Hide();

	panel->SetSizer(mainSizer);
	return panel;
}

//------------------------------
///KNOWN SPELLS PAGE PANELS
//------------------------------
wxPanel* MainFrame::CreateKnownSpellAbilityPanel(wxPanel* parent)
{
	wxFont curFont(wxFontInfo(wxSize(0,12)));
	curFont.MakeBold();

	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize);
	setWindowColour(panel, mainColour);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticLine* verBar = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	std::string tempStr;

	wxString choices[] = {"Intelligence", "Wisdom", "Charisma"};

	knownPagePanels.Combo_SpellCastSkill = new wxComboBox(panel, wxID_ANY, "Intelligence", wxDefaultPosition, wxDefaultSize, 3, choices, wxCB_READONLY);

	auto ProfBonus_Text = new wxStaticText(panel, wxID_ANY, "Profeciency\nBonus", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	knownPagePanels.ProfBonus_Val = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	knownPagePanels.ProfBonus_Val->SetLabel(std::to_string(character.getProfBonus()));

	auto SpellCastMod_Text = new wxStaticText(panel, wxID_ANY, "Spell Attack\nMod", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	knownPagePanels.SpellCastMod_Val = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);

	auto SpellSaveMod_Text = new wxStaticText(panel, wxID_ANY, "Spell Save\nMod", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	knownPagePanels.SpellSaveMod_Val = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);

	auto SelectedSpells_Text = new wxStaticText(panel, wxID_ANY, "Spells\nTicked", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	knownPagePanels.SelectedSpells_Val = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	
	auto SpellPoints_Text = new wxStaticText(panel, wxID_ANY, "Spell\nPoints", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	knownPagePanels.SpellPoints_Val = new wxTextCtrl(panel, wxID_ANY, std::to_string(character.getTotalSpellPoints()), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);

	ProfBonus_Text->SetFont(curFont);
	SpellCastMod_Text->SetFont(curFont);
	SpellSaveMod_Text->SetFont(curFont);
	SelectedSpells_Text->SetFont(curFont);
	SpellPoints_Text->SetFont(curFont);

	updateKnownSpellMods();

	int gapBig = 15, gapSmall = 5;

	sizer->Add(gapBig, -1);
	sizer->Add(knownPagePanels.Combo_SpellCastSkill, 0, wxALIGN_CENTER);
	
	sizer->Add(gapBig, -1);
	sizer->Add(verBar, 0, wxEXPAND);
	sizer->Add(gapBig, -1);
	
	sizer->Add(ProfBonus_Text, 0, wxALIGN_CENTER);
	sizer->Add(gapSmall, -1);
	sizer->Add(knownPagePanels.ProfBonus_Val, 0, wxALIGN_CENTER);
	
	sizer->Add(gapBig, -1);
	verBar = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	sizer->Add(verBar, 0, wxEXPAND);
	sizer->Add(gapBig, -1);

	sizer->Add(SpellCastMod_Text, 0, wxALIGN_CENTER);
	sizer->Add(gapSmall, -1);
	sizer->Add(knownPagePanels.SpellCastMod_Val, 0, wxALIGN_CENTER);
	
	sizer->Add(gapBig, -1);
	verBar = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	sizer->Add(verBar, 0, wxEXPAND);
	sizer->Add(gapBig, -1);
	
	sizer->Add(SpellSaveMod_Text, 0, wxALIGN_CENTER);
	sizer->Add(gapSmall, -1);
	sizer->Add(knownPagePanels.SpellSaveMod_Val, 0, wxALIGN_CENTER);

	sizer->Add(gapBig, -1);
	verBar = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	sizer->Add(verBar, 0, wxEXPAND);
	sizer->Add(gapBig, -1);

	sizer->Add(SelectedSpells_Text, 0, wxALIGN_CENTER);
	sizer->Add(gapSmall, -1);
	sizer->Add(knownPagePanels.SelectedSpells_Val, 0, wxALIGN_CENTER);

	sizer->Add(gapBig, -1);
	verBar = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	sizer->Add(verBar, 0, wxEXPAND);
	sizer->Add(gapBig, -1);

	//if (uses.SpellPoints)
	{
		sizer->Add(SpellPoints_Text, 0, wxALIGN_CENTER);
		sizer->Add(gapSmall, -1);
		sizer->Add(knownPagePanels.SpellPoints_Val, 0, wxALIGN_CENTER);

		sizer->Add(gapBig, -1);
		verBar = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
		sizer->Add(verBar, 0, wxEXPAND);
		sizer->Add(gapBig, -1);
	}	

	knownPagePanels.ProfBonus_Val->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
	knownPagePanels.SpellCastMod_Val->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
	knownPagePanels.SpellSaveMod_Val->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
	knownPagePanels.SelectedSpells_Val->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &knownPagePanels.selectedSpells));
	knownPagePanels.SpellPoints_Val->SetValidator(wxTextValidator(wxFILTER_NUMERIC));

	knownPagePanels.SelectedSpells_Val->SetLabel(knownPagePanels.selectedSpells);

	if (!uses.SpellPoints)
	{
		knownPagePanels.SpellPoints_Val->Hide();
		verBar->Hide();
	}

	calcCheckedSpells();
	
	setWindowColour(knownPagePanels.Combo_SpellCastSkill, ctrlColour);
	setWindowColour(knownPagePanels.ProfBonus_Val, ctrlColour);
	setWindowColour(knownPagePanels.SpellCastMod_Val, ctrlColour);
	setWindowColour(knownPagePanels.SpellSaveMod_Val, ctrlColour);
	setWindowColour(knownPagePanels.SelectedSpells_Val, ctrlColour);
	setWindowColour(knownPagePanels.SpellPoints_Val, ctrlColour);

	panel->SetSizerAndFit(sizer);
	panel->Layout();

	return panel;
}

wxPanel* MainFrame::CreateKnownSpellsAllLevels(wxPanel* parent)
{
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize);
	//panel->SetBackgroundColour(DescColor.first);
	setWindowColour(panel, mainColour);

	int gridLineThickness = 2;

	wxGridBagSizer* sizer = new wxGridBagSizer(gridLineThickness, gridLineThickness);

	std::vector<std::pair<wxGBPosition, wxGBSpan>> items =
	{
		//SPELL LIST LINES
		{{0,1},{9,1}},		//0
		{{9,1},{9,1}},		//1

		{{0,3},{9,1}},		//2
		{{9,3},{9,1}},		//3
		
		{{0,5},{6,1}},		//4
		{{6,5},{6,1}},		//5
		{{12,5},{6,1}},		//6
		
		{{0,7},{6,1}},		//7
		{{6,7},{6,1}},		//8
		{{12,7},{6,1}},		//9

		//STATIC LINES
		{{0,0},{18,1}},		//10
		{{0,2},{18,1}},		//11
		{{0,4},{18,1}},		//12
		{{0,6},{18,1}},		//13
		{{0,8},{18,1}},		//14

		{{18,0},{1,8}}		//15
	};
	
	std::pair<wxGBPosition, wxGBSpan> curItems;
	wxPanel* p;

	//LISTS
	for (int i = 0; i < 10; ++i)
	{
		curItems = items[i];
		p = CreateKnownSpells_SpellSlot(panel, i);
		sizer->Add(p, curItems.first, curItems.second, wxEXPAND | wxALIGN_CENTER);
	}
	
	//HORLINES
	for (int i = 10; i < 15; ++i)
	{
		curItems = items[i];
		wxStaticLine* line =  new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
		sizer->Add(line, curItems.first, curItems.second, wxEXPAND);
	}
	
	curItems = items[15];
	wxStaticLine* line = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	sizer->Add(line, curItems.first, curItems.second, wxEXPAND);


	panel->SetSizer(sizer);
	panel->Layout();

	return panel;
}

wxPanel* MainFrame::CreateKnownSpells_SpellSlot(wxPanel* parent, int spellLevel)
{
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, baseColSize);
	panel->SetBackgroundColour(mainColour.first);

	wxSize baseSize(250, -1);
	wxSize curSize = baseSize;
	curSize.x /= 4;
	curSize.x -= 20;
	wxSize listSize = baseSize;
	listSize.y = 200;

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* subSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* text = nullptr;
	wxSpinCtrl* slots = nullptr;
	wxButton* useButton = nullptr;
	wxButton* useSpellPoint = nullptr;
	wxCheckListBox* list = nullptr;
	wxButton* AddSpell = nullptr;
	wxButton* RemSpell = nullptr;

	std::tuple<wxStaticText*, wxSpinCtrl*, wxButton*, wxCheckListBox*> SpellSlotBox;

	std::string LevelTitle = "";

	if (spellLevel == 0)
		LevelTitle = "Cantrips";
	else if (spellLevel > 0 || spellLevel < 10)
		LevelTitle = "Level " + std::to_string(spellLevel);
	else
	{
		LevelTitle = "Level Error";
		return panel;
	}

	wxStaticLine* verBar = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);

	text = new wxStaticText(panel, wxID_ANY, LevelTitle, wxDefaultPosition, baseSize, wxALIGN_CENTER_HORIZONTAL);
	slots = new wxSpinCtrl(panel, wxID_ANY, "0", wxDefaultPosition, curSize, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
	useButton = new wxButton(panel, wxID_ANY, "Use", wxDefaultPosition, curSize);
	std::string SP_String = "Use " + std::to_string(convSlotToPoint(spellLevel)) + " SP";
	useSpellPoint = new wxButton(panel, wxID_ANY, SP_String, wxDefaultPosition, wxSize(curSize.x*1.5, curSize.y));
	list = new wxCheckListBox(panel, wxID_ANY, wxDefaultPosition, baseSize);

	AddSpell = new wxButton(panel, wxID_ANY, "", wxDefaultPosition, wxSize(curSize.x, buttonSize.y));
	RemSpell = new wxButton(panel, wxID_ANY, "", wxDefaultPosition, wxSize(curSize.x, buttonSize.y));

	wxArtProvider prov;
	AddSpell->SetBitmap(prov.GetBitmap(wxART_PLUS, wxART_BUTTON));
	RemSpell->SetBitmap(prov.GetBitmap(wxART_MINUS, wxART_BUTTON));

	SpellSlotBox = { text, slots, useButton, list };
	knownPagePanels.SpellSlotLevelList.push_back(SpellSlotBox);
	knownPagePanels.AddSpellSlot_Button.push_back(AddSpell);
	knownPagePanels.RemSpellSlot_Button.push_back(RemSpell);
	knownPagePanels.spellPointButton.push_back(useSpellPoint);

	list->SetMinSize(listSize);
	if (spellLevel < 4)
	{
		int a = text->GetSize().y;
		int b = AddSpell->GetSize().y;
		int y = listSize.y * 1.45 + a + b;
		list->SetMinSize(wxSize(listSize.x, y));
	}

	if (!overrides.spellPoint)
	{
		slots->SetMin(0);
		auto& curSlots = character.getSpellSlots();
		int numSlots = curSlots.slots[spellLevel].first;
		if (numSlots == -1)
			slots->SetMax(0);
		else if (numSlots == -4)
			slots->SetMax(100);
		else
			slots->SetMax(numSlots);

		slots->SetValue(slots->GetMax());
	}

	text->SetFont(BigFont1);

	setWindowColour(text, ctrlColour);
	setWindowColour(list, descColour);
	//setWindowColour(slots, ctrlColor);

	AddSpell->SetMaxSize(curSize);
	std::get<1>(SpellSlotBox)->SetMaxSize(curSize);
	std::get<2>(SpellSlotBox)->SetMaxSize(curSize);

	subSizer->Add(RemSpell, 0, wxEXPAND);
	subSizer->Add(2, -1);
	subSizer->Add(AddSpell, 0, wxEXPAND);
	subSizer->Add(2, -1);

	if (spellLevel == 0)
	{
		//delete slots;
		//delete useButton;
		//delete useSpellPoint;
		slots->Hide();
		useButton->Hide();
		useSpellPoint->Hide();
	}
	else
	{
		if (!uses.SpellSlots)
		{
			slots->Hide();
			useButton->Hide();
		}

		if (!uses.SpellPoints)
			useSpellPoint->Hide();
	}
	

	//if (spellLevel != 0 && uses.SpellSlots)
	{
		subSizer->Add(5, -1);
		subSizer->Add(slots, 0, wxEXPAND);
		subSizer->Add(2, -1);
		subSizer->Add(useButton, 0, wxEXPAND);
	}

	//if (spellLevel != 0 && uses.SpellPoints)
	{
		subSizer->Add(2, -1);
		subSizer->Add(useSpellPoint, 0, wxEXPAND);
	}

	sizer->Add(verBar, 0, wxEXPAND);
	sizer->Add(text, 0, wxEXPAND);
	verBar = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	sizer->Add(verBar, 0, wxEXPAND);
	sizer->Add(-1, 5);
	sizer->Add(subSizer, 0, wxALIGN_RIGHT | wxRIGHT, 5);
	sizer->Add(-1, 5);
	sizer->Add(list, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT, 3);

	panel->SetSizerAndFit(sizer);
	panel->Layout();
	calcCheckedSpells();

	return panel;
}

//------------------------------
///KNOWN SPELLS PAGE PANELS
//------------------------------
wxPanel* MainFrame::CreateSpellDescription(wxPanel* parent, Spell& dispSpell, wxStaticText*& Title, wxStaticText*& Level,
	wxStaticText*& CastingTime, wxStaticText*& Range, wxStaticText*& Duration, wxStaticText*& Components, wxStaticText*& Type,
	wxStaticText*& AppClasses, wxHtmlWindow*& Description, wxSize size)
{
	wxSize DescColWidth = size;
	wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, DescColWidth);
	panel->SetBackgroundColour(mainColour.first);
	//wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	int borderSize = 5;
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* p;
	std::string tempStr = "";
	std::string spacerStr = "  ";
	Spell curSpell = dispSpell;
	int margin = 2;
	int sizerFlag = wxEXPAND | wxLEFT | wxTOP;

	wxFont titleFont(wxFont(wxFontInfo(wxSize(0, 18))));
	titleFont.MakeBold();

	wxFont curFont(wxFont(wxFontInfo(wxSize(0, 13))));
	curFont.SetWeight(wxFontWeight::wxFONTWEIGHT_BOLD);

	//SPELL TITLE
	p = new wxPanel(panel, wxID_ANY, wxDefaultPosition, DescColWidth);
	setWindowColour(p, descColour);
	tempStr = curSpell.getName();
	Title = new wxStaticText(p, wxID_ANY, tempStr);
	Title->SetFont(titleFont);
	Title->Wrap(DescColWidth.x);
	mainSizer->Add(p, 0, sizerFlag, margin);

	//SPELL LEVEL
	p = new wxPanel(panel, wxID_ANY, wxDefaultPosition, DescColWidth);
	setWindowColour(p, descColour);
	tempStr = std::to_string(curSpell.getLevel());
	Level = new wxStaticText(p, wxID_ANY, spacerStr + "Level: " + tempStr);
	Level->SetFont(curFont);
	Level->Wrap(DescColWidth.x);
	mainSizer->Add(p, 0, sizerFlag, margin);

	//CASTING TIME
	p = new wxPanel(panel, wxID_ANY, wxDefaultPosition, DescColWidth);
	setWindowColour(p, descColour);
	tempStr = curSpell.getCastingTime();
	CastingTime = new wxStaticText(p, wxID_ANY, spacerStr + "Casting Time: " + tempStr);
	CastingTime->SetFont(curFont);
	CastingTime->Wrap(DescColWidth.x);
	mainSizer->Add(p, 0, sizerFlag, margin);

	//RANGE
	p = new wxPanel(panel, wxID_ANY, wxDefaultPosition, DescColWidth);
	setWindowColour(p, descColour);
	tempStr = curSpell.getRange();
	Range = new wxStaticText(p, wxID_ANY, spacerStr + "Range: " + tempStr);
	Range->SetFont(curFont);
	Range->Wrap(DescColWidth.x);
	mainSizer->Add(p, 0, sizerFlag, margin);

	//DURATION
	p = new wxPanel(panel, wxID_ANY, wxDefaultPosition, DescColWidth);
	setWindowColour(p, descColour);
	tempStr = curSpell.getDuration();
	Duration = new wxStaticText(p, wxID_ANY, spacerStr + "Duration: " + tempStr);
	Duration->SetFont(curFont);
	Duration->Wrap(DescColWidth.x);
	mainSizer->Add(p, 0, sizerFlag, margin);

	//COMPONENTS
	p = new wxPanel(panel, wxID_ANY, wxDefaultPosition, DescColWidth);
	setWindowColour(p, descColour);
	tempStr = comp_to_str(curSpell.getComps()) + curSpell.getMaterialsFromDesc();
	Components = new wxStaticText(p, wxID_ANY, spacerStr + "Components: " + tempStr);
	Components->SetFont(curFont);
	Components->Wrap(DescColWidth.x);
	mainSizer->Add(p, 0, sizerFlag, margin);

	//SPELL TYPE
	p = new wxPanel(panel, wxID_ANY, wxDefaultPosition, DescColWidth);
	setWindowColour(p, descColour);
	tempStr = curSpell.getSpellType();
	Type = new wxStaticText(p, wxID_ANY, spacerStr + "Type: " + tempStr);
	Type->SetFont(curFont);
	Type->Wrap(DescColWidth.x);
	mainSizer->Add(p, 0, sizerFlag, margin);

	//APPLICABLE CLASSES
	p = new wxPanel(panel, wxID_ANY, wxDefaultPosition, DescColWidth);
	setWindowColour(p, descColour);
	tempStr = curSpell.getAppClasses();
	AppClasses = new wxStaticText(p, wxID_ANY, spacerStr + "Classes: " + tempStr);
	AppClasses->SetFont(curFont);
	AppClasses->Wrap(DescColWidth.x);
	mainSizer->Add(p, 0, sizerFlag, margin);

	//DESCRIPTION
	tempStr = curSpell.getDescription();
	std::string htmlFormatOpen = "<body><b><span style:'color:white;'>";
	std::string htmlFormatClose = "</span></b></body>";
	Description = new wxHtmlWindow(panel, wxID_ANY, wxDefaultPosition, wxSize(DescColWidth.x, 100), wxHW_SCROLLBAR_AUTO);
	Description->SetPage(htmlFormatOpen + tempStr + htmlFormatClose);
	mainSizer->Add(Description, 1, sizerFlag, margin);
	
	setWindowColour(Description, descColour);

	panel->SetSizer(mainSizer);
	panel->Layout();

	return panel;
}

//------------------------------
///NOTEBOOK
//------------------------------
void MainFrame::CreatePages(wxNotebook* parent)
{
	wxWindow* mainPage = CreateMainPage(parent);
	parent->AddPage(mainPage, "Character");

	wxWindow* allSpellsPage = CreateSpellsPage(parent);
	parent->AddPage(allSpellsPage, "All Spells");

	wxWindow* knownSpellsPage = CreateKnownSpellsPage(parent);
	//knownSpellsPage->SetBackgroundColour(wxColour(0x99,0x77,0x77));
	parent->AddPage(knownSpellsPage, "Known Spells", true);

	wxWindow* SlotsTablePage = CreateSpellSlotsTable(parent);
	SlotsTablePage->SetBackgroundColour(slotsColour.first);
	SlotsTablePage->SetForegroundColour(slotsColour.second);
	parent->AddPage(SlotsTablePage, "SpellSlotTable");

	wxWindow* Inventory = CreateInventoryPage(parent);
	Inventory->SetBackgroundColour(wxColour(0x19, 0x19, 0x22));
	parent->AddPage(Inventory, "Inventory");

	wxWindow* TestPage = CreateTestPanel(parent);
	TestPage->SetBackgroundColour(wxColour(0x19, 0x19, 0x22));
	parent->AddPage(TestPage, "Test");

}

//------------------------------
///UPDATES AND INITS
//------------------------------
void MainFrame::updateDispSpell(std::string str)
{
	std::string name;

	for (auto i = str.begin(); i != str.end(); ++i)
	{
		*i = std::tolower(*i);
	}

	for (auto it = allSpells.begin(); it != allSpells.end(); ++it)
	{
		name = it->getName();

		for (auto i = name.begin(); i != name.end(); ++i)
		{
			*i = std::tolower(*i);
		}

		if (name == str)
		{
			spellDesc.dispSpell = &(*it);
			return;
		}

	}
}

void MainFrame::updateSpellDesc()
{
	int wrapWidth = spellDesc.Title->GetParent()->GetParent()->GetSize().x;
	//int wrapWidth = spellDesc.ColWidth.x - 10;
	std::string spacerStr = "  ";
	std::string tempStr = "";

	spellDesc.AppClasses->SetLabel(spacerStr + "Classes: " + spellDesc.dispSpell->getAppClasses());
	spellDesc.AppClasses->Wrap(wrapWidth);

	spellDesc.CastingTime->SetLabel(spacerStr + "Casting Time: " + spellDesc.dispSpell->getCastingTime());
	spellDesc.CastingTime->Wrap(wrapWidth);

	tempStr = comp_to_str(spellDesc.dispSpell->getComps());
	tempStr += " ";
	tempStr += spellDesc.dispSpell->getMaterialsFromDesc();
	spellDesc.Components->SetLabel(spacerStr + "Components: " + tempStr);
	spellDesc.Components->Wrap(wrapWidth);

	std::string htmlFormatOpen = "<body><b>";
	std::string htmlFormatClose = "</b></body>";	
	spellDesc.Description->SetPage(htmlFormatOpen + spellDesc.dispSpell->getDescription() + htmlFormatClose);
	setWindowColour(spellDesc.Description, descColour);

	spellDesc.Duration->SetLabel(spacerStr + "Duration: " + spellDesc.dispSpell->getDuration());
	spellDesc.Duration->Wrap(wrapWidth);

	spellDesc.Level->SetLabel(spacerStr + "Level: " + std::to_string(spellDesc.dispSpell->getLevel()));
	spellDesc.Level->Wrap(wrapWidth);

	spellDesc.Range->SetLabel(spacerStr + "Range: " + spellDesc.dispSpell->getRange());
	spellDesc.Range->Wrap(wrapWidth);

	spellDesc.Title->SetLabel(spellDesc.dispSpell->getName());
	spellDesc.Title->Wrap(wrapWidth);

	spellDesc.Type->SetLabel(spacerStr + "Type: " + spellDesc.dispSpell->getSpellType());
	spellDesc.Type->Wrap(wrapWidth);

	spellDesc.Title->GetParent()->GetParent()->Layout();

}

void MainFrame::updateKnownSpellDesc()
{
	int wrapWidth = knownPagePanels.Title->GetParent()->GetParent()->GetSize().x;
	//int wrapWidth = knownPagePanels.ColWidth.x - 10;
	std::string spacerStr = "  ";
	std::string tempStr = "";

	knownPagePanels.AppClasses->SetLabel(spacerStr + "Classes: " + knownPagePanels.dispSpell->getAppClasses());
	knownPagePanels.AppClasses->Wrap(wrapWidth);

	knownPagePanels.CastingTime->SetLabel(spacerStr + "Casting Time: " + knownPagePanels.dispSpell->getCastingTime());
	knownPagePanels.CastingTime->Wrap(wrapWidth);

	tempStr = comp_to_str(knownPagePanels.dispSpell->getComps());
	tempStr += " ";
	tempStr += knownPagePanels.dispSpell->getMaterialsFromDesc();
	knownPagePanels.Components->SetLabel(spacerStr + "Components: " + tempStr);
	knownPagePanels.Components->Wrap(wrapWidth);

	std::string htmlFormatOpen = "<body><b>";
	std::string htmlFormatClose = "</b></body>";
	knownPagePanels.Description->SetPage(htmlFormatOpen + knownPagePanels.dispSpell->getDescription() + htmlFormatClose);

	knownPagePanels.Duration->SetLabel(spacerStr + "Duration: " + knownPagePanels.dispSpell->getDuration());
	knownPagePanels.Duration->Wrap(wrapWidth);

	knownPagePanels.Level->SetLabel(spacerStr + "Level: " + std::to_string(knownPagePanels.dispSpell->getLevel()));
	knownPagePanels.Level->Wrap(wrapWidth);

	knownPagePanels.Range->SetLabel(spacerStr + "Range: " + knownPagePanels.dispSpell->getRange());
	knownPagePanels.Range->Wrap(wrapWidth);

	knownPagePanels.Title->SetLabel(knownPagePanels.dispSpell->getName());
	knownPagePanels.Title->Wrap(wrapWidth);

	knownPagePanels.Type->SetLabel(spacerStr + "Type: " + knownPagePanels.dispSpell->getSpellType());
	knownPagePanels.Type->Wrap(wrapWidth);

	setWindowColour(knownPagePanels.Description, descColour);

	knownPagePanels.Title->GetParent()->GetParent()->Layout();
}

void MainFrame::updateKnownDispSpell(std::string str)
{
	std::string name;

	auto& knownSpell = character.getKnownSpells();

	for (auto i = str.begin(); i != str.end(); ++i)
	{
		*i = std::tolower(*i);
	}

	for (auto it = knownSpell.begin(); it != knownSpell.end(); ++it)
	{
		name = it->getName();

		for (auto i = name.begin(); i != name.end(); ++i)
		{
			*i = std::tolower(*i);
		}

		if (name == str)
		{
			knownPagePanels.dispSpell = &(*it);
			return;
		}

	}
}

void MainFrame::updateFeaturesList()
{
	std::vector<feature>& features = character.getFeatures();
	for (auto it = features.begin(); it != features.end(); ++it)
	{
		wxListBox* list = mainPagePanels.Feature_FullList;
		int i = list->FindString(it->title);

		if (i == wxNOT_FOUND)
		{
			list->Append(it->title);
		}
	}	
}

void MainFrame::updateKnownSpellMods()
{
	int spellMod, spellSaveMod;
	Skills tempSkill;

	std::string tempStr = knownPagePanels.Combo_SpellCastSkill->GetValue().ToStdString();
	if (tempStr == "Intelligence")
		tempSkill = Skills::Intelligence;
	else if (tempStr == "Wisdom")
		tempSkill = Skills::Wisdom;
	else if (tempStr == "Charisma")
		tempSkill = Skills::Charisma;
	
	knownPagePanels.ProfBonus_Val->SetLabel(std::to_string(character.getProfBonus()));
	
	spellMod = character.getProfBonus() + character.getSkillMod(tempSkill);
	knownPagePanels.SpellCastMod_Val->SetLabel(std::to_string(spellMod));

	spellSaveMod = 8 + spellMod;
	knownPagePanels.SpellSaveMod_Val->SetLabel(std::to_string(spellSaveMod));
}

void MainFrame::updateKnownSpellsLists()
{
	for (int i = 1; i < 10; ++i)
	{
		auto slotSpin = std::get<1>(knownPagePanels.SpellSlotLevelList[i]);
		auto& spSlots = character.getSpellSlots();
		int x = spSlots.slots[i].first;
		if (x == -1)
			slotSpin->SetMax(0);

		if (spSlots.slots[i].second != -1)
			x += spSlots.slots[i].second;
		
		if (x > 0)
		{
			slotSpin->SetMax(x);
			slotSpin->SetValue(slotSpin->GetMax());
		}		

		x = character.getTotalSpellPoints();
		knownPagePanels.SpellPoints_Val->SetValue(std::to_string(x));

		if (uses.SpellSlots)
		{
			slotSpin->Show();
			std::get<2>(knownPagePanels.SpellSlotLevelList[i])->Show();
		}

		if (!uses.SpellSlots)
		{
			slotSpin->Hide();
			std::get<2>(knownPagePanels.SpellSlotLevelList[i])->Hide();
		}

		if (uses.SpellPoints)
		{
			knownPagePanels.SpellPoints_Val->Show();
			knownPagePanels.spellPointButton[i]->Show();
		}

		if (!uses.SpellPoints)
		{
			knownPagePanels.SpellPoints_Val->Hide();
			knownPagePanels.spellPointButton[i]->Hide();
		}
		
		slotSpin->GetParent()->GetParent()->Layout();
		Layout();
	}
}

void MainFrame::updateMoneyCtrls()
{
	int money[] = { 
		character.getPlatinum(),
		character.getGold(),
		character.getSilver(),
		character.getCopper() };
	
	for (int i = 0; i < 4; i++)
		mainPagePanels.moneyVals[i]->SetValue(money[i]);
}

void MainFrame::updateHP()
{
	std::string str = "HP | " + std::to_string(character.getTotHP());
	int x = std::get<1>(mainPagePanels.MaxHPBonus)->GetValue();

	character.setTotHPBonus(x);

	if (x != 0)
		str += " (" + std::to_string(character.getModTotHP()) + ")";

	mainPagePanels.HPText->SetLabel(str);
	mainPagePanels.HPText->GetParent()->Layout();
	mainPagePanels.HP->SetMax(character.getModTotHP());
}

void MainFrame::updatePlayerConds()
{
	playerConditions.clear();
	int cond = character.getConditions();
	for (auto it = allConditions.begin(); it != allConditions.end(); ++it)
	{
		if (it->first == Conditions::NoCondition)
			continue;

		if (it->first == Conditions::Incapacitated)
			continue;

		if (it->first & cond)
			playerConditions.push_back(*it);
	}
}

void MainFrame::updateStats()
{
	auto stat = character.getStats();
	int statsArray[6] = { stat->Str,stat->Dex, stat->Con, stat->Int, stat->Wis, stat->Cha };
	Skills skillNames[6] = { Skills::Strength, Skills::Dexterity, Skills::Constitution, Skills::Intelligence, Skills::Wisdom, Skills::Charisma };

	
	for (int i = 0; i < 6; ++i)
	{
		mainPagePanels.Stat_TextCtrls[i].first->SetValue(std::to_string(statsArray[i]));
		mainPagePanels.Stat_TextCtrls[i].second->SetValue(std::to_string(character.getSkillMod(skillNames[i])));
	}
	
	wxMessageBox("HP is not updated. Please update manually");
	updateInitiative();
	updateKnownSpellMods();
	updateSavingThrows();
	updateSkills();
}

void MainFrame::updateInitiative()
{
	int init = character.getInitiative();
	mainPagePanels.InitMod.first->SetValue(std::to_string(init));
	mainPagePanels.InitMod.second->SetValue(std::to_string(character.getInitMod()));
}

void MainFrame::updateSavingThrows()
{
	std::vector<Skills> tempSkills =
	{
		Skills::Strength, Skills::Dexterity, Skills::Constitution, Skills::Intelligence, Skills::Wisdom, Skills::Charisma
	};

	for (int i = 0; i < 6; ++i)
	{
		int x = character.getSkillMod(tempSkills[i], true);
		mainPagePanels.SavingThrows[i].second->SetValue(std::to_string(x));

		bool isProficient = character.checkProf(tempSkills[i]);
		bool isExpert = character.checkExpert(tempSkills[i]);
		
		if (isExpert)
		{
			mainPagePanels.SavingThrows[i].first->SetForegroundColour(expertiseColour);
		}

		else if (isProficient)
		{
			mainPagePanels.SavingThrows[i].first->SetForegroundColour(proficientColour);
			mainPagePanels.SavingThrows[i].first->SetFont(SkillFont.Underlined());
		}		

		else
		{
			mainPagePanels.SavingThrows[i].first->SetForegroundColour(panelColour.second);
			mainPagePanels.SavingThrows[i].first->SetFont(SkillFont);
		}
	}
}

void MainFrame::updateSkills()
{
	std::vector<Skills> curSkill =
	{
		Skills::Acrobatics,
		Skills::Animal_Handling,
		Skills::Arcana,
		Skills::Athletics,
		Skills::Deception,
		Skills::History,
		Skills::Insight,
		Skills::Intimidation,
		Skills::Investigation,
		Skills::Medicine,
		Skills::Nature,
		Skills::Perception,
		Skills::Performance,
		Skills::Persuasion,
		Skills::Religion,
		Skills::Sleight_of_Hand,
		Skills::Stealth,
		Skills::Survival
	};

	
	

	for (int i = 0; i < curSkill.size(); ++i)
	{
		int x = character.getSkillMod(curSkill[i], false);
		mainPagePanels.Skills[i].second->SetValue(std::to_string(x));

		bool isProficient = character.checkProf(curSkill[i]);
		bool isExpert = character.checkExpert(curSkill[i]);

		mainPagePanels.Skills[i].first->SetForegroundColour(panelColour.second);
		mainPagePanels.Skills[i].first->SetFont(SkillFont);

		if (isExpert)
		{
			mainPagePanels.Skills[i].first->SetForegroundColour(expertiseColour);
			mainPagePanels.Skills[i].first->SetFont(SkillFont.Underlined());
		}

		else if (isProficient)
		{
			mainPagePanels.Skills[i].first->SetForegroundColour(proficientColour);
			mainPagePanels.Skills[i].first->SetFont(SkillFont.Underlined());
		}
	}
}

void MainFrame::SpellDesc::fillAllSpellTree(std::vector<Spell>& allSpells)
{
	for (auto it = allSpells.begin(); it != allSpells.end(); ++it)
	{
		int lev = it->getLevel();
		switch (lev)
		{
		case 0:
			spellList->AppendItem(spellLevelId[0], it->getName());
			break;
		case 1:
			spellList->AppendItem(spellLevelId[1], it->getName());
			break;
		case 2:
			spellList->AppendItem(spellLevelId[2], it->getName());
			break;
		case 3:
			spellList->AppendItem(spellLevelId[3], it->getName());
			break;
		case 4:
			spellList->AppendItem(spellLevelId[4], it->getName());
			break;
		case 5:
			spellList->AppendItem(spellLevelId[5], it->getName());
			break;
		case 6:
			spellList->AppendItem(spellLevelId[6], it->getName());
			break;
		case 7:
			spellList->AppendItem(spellLevelId[7], it->getName());
			break;
		case 8:
			spellList->AppendItem(spellLevelId[8], it->getName());
			break;
		case 9:
			spellList->AppendItem(spellLevelId[9], it->getName());
			break;
		default:
			break;
		}
	}
}

void MainFrame::calcCheckedSpells()
{
	int x = 0;
	int len = knownPagePanels.SpellSlotLevelList.size();
	if (len == 0)
		return;
	for (int i = 1; i < len; ++i)
	{
		wxCheckListBox* list = std::get<3>(knownPagePanels.SpellSlotLevelList[i]);
		int numItems = list->GetCount();
		
		for (int j = 0; j < numItems; j++)
		{
			if (list->IsChecked(j))
				x++;
		}
	}
	
	knownPagePanels.selectedSpells = std::to_string(x);
	TransferDataToWindow();

}

void MainFrame::DrawMain()
{
	
}

void MainFrame::loadSpells()
{
	//LOADING SPELLS INTO MEMORY
	std::string path = "D:\\C++ Programmes\\CharacerSheet\\";
	std::string fileName = "allSpells.csv";

	wxFile spellFile(path + fileName, wxFile::read);

	if (!spellFile.IsOpened())
		wxMessageBox("File not Opened", "Caption", wxOK | wxCENTRE | wxICON_ERROR, this);

	wxFileInputStream fileStream(spellFile);
	wxTextInputStream fileTextStream(fileStream);

	fileTextStream.SetStringSeparators(wxString());

	wxString line;

	bool firstLine = true;
	std::vector<std::string> spellParts;
	while (!fileStream.Eof())
	{
		if (!firstLine)
		{
			spellParts.clear();
			line = fileTextStream.ReadLine();
			wxStringTokenizer lineTokenizer(line, ";");
			while (lineTokenizer.HasMoreTokens())
			{
				wxString token = lineTokenizer.GetNextToken();
				spellParts.push_back(token.ToStdString());
			}

			//Name;Level;Type;Casting time;Range;Components;Duration;Description;Applicable classes
			int spellLevel = stoi(spellParts[1]);
			spellSchool schoolStr = string_to_School(spellParts[2], (spellLevel == 0) ? true : false);
			if (spellParts.size() == 9)
			{
				Spell tempSpell(
					spellParts[0],						//Name
					spellLevel,							//Level
					schoolStr,							//School
					spellParts[3],						//Cast Time
					spellParts[4],						//Range
					string_to_Comp(spellParts[5]),		//Components
					spellParts[6],						//Duration
					spellParts[7],						//Description
					spellParts[8]						//Classes

				);
				
				allSpells.push_back(tempSpell);
			}

		}

		if (firstLine)
			fileTextStream.ReadLine();

		firstLine = false;
	}
}

void MainFrame::fillLevelSpells()
{
	for (auto it = allSpells.begin(); it != allSpells.end(); ++it)
	{
		for (int j = 0; j < 10; j++)
		{
			if (it->getLevel() == j)
				allSpellsLevel[j].push_back(&(*it));
		}
	}
}

int MainFrame::getNumSpells(int level)
{
	int count = 0;
	if (level < 0 || level > 9)
		return -1;

	return allSpellsLevel[level].size();
}

void MainFrame::makeSavingThrowPair(wxStaticText* savingThrowName, wxTextCtrl* savingThrowValue, Skills curSkill)
{
	std::wstring tempString = L"";
	bool isProficient = false;
	
	tempString = Util::to_wstring(getSkillStr(curSkill));

	wxFont tempFont(wxFontInfo(wxSize(0, 12)));
	tempFont.MakeBold();

	savingThrowName->SetLabel(tempString);
	tempString = std::to_wstring(character.getSkillMod(curSkill, true));
	savingThrowValue->SetLabel(tempString);

	isProficient = character.checkProf(curSkill);
	
	savingThrowName->SetFont(tempFont);

	if (isProficient)
	{
		savingThrowName->SetForegroundColour(proficientColour);
		savingThrowName->SetFont(tempFont.Underlined());
	}

}

void MainFrame::makeSkillPair(wxStaticText* skillName, wxTextCtrl* skillValue, Skills curSkill)
{
	std::wstring tempString = L"";
	bool isProficient = false;

	tempString = Util::to_wstring(getSkillStr(curSkill));

	wxFont tempFont(wxFontInfo(wxSize(0, 12)));
	tempFont.MakeBold();

	skillName->SetLabel(tempString);
	tempString = std::to_wstring(character.getSkillMod(curSkill));
	skillValue->SetLabel(tempString);

	skillName->SetFont(tempFont);

	isProficient = character.checkProf(curSkill);
	if (isProficient)
	{
		skillName->SetForegroundColour(proficientColour);
		skillName->SetFont(tempFont.Underlined());
	}

}

void MainFrame::makeAddRemList(wxStaticText*& title, wxButton*& add, wxButton*& rem, wxListBox*& list, wxSizer* sizer, wxPanel* parent)
{
	add = new wxButton(parent, wxID_ANY, "", wxDefaultPosition, buttonSize);
	rem = new wxButton(parent, wxID_ANY, "", wxDefaultPosition, buttonSize);
	list = new wxListBox(parent, wxID_ANY);

	add->SetBitmap(wxArtProvider().GetBitmap(wxART_PLUS, wxART_BUTTON));
	rem->SetBitmap(wxArtProvider().GetBitmap(wxART_MINUS, wxART_BUTTON));

	auto titleSizer = new wxGridSizer(1, 2, 0, 0);
	auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	
	buttonSizer->Add(rem);
	buttonSizer->Add(4, -1);
	buttonSizer->Add(add);

	titleSizer->Add(title, 0, wxALIGN_LEFT | wxALIGN_BOTTOM);
	titleSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALIGN_BOTTOM);
	
	sizer->Add(titleSizer, 0, wxEXPAND);
	sizer->Add(-1, 5);
	sizer->Add(list, 1, wxEXPAND);
}

//------------------------------
///EVENT HANDLERS
//------------------------------

/// FOR TESTING PAGE
void MainFrame::onTest(wxCommandEvent& event)
{
	SliderDialog dialog(this);

	dialog.ShowModal();
}


/// ACTUAL HANDLERS

void MainFrame::onSpellSearchType(wxCommandEvent& event)
{
	wxObject* obj = event.GetEventObject();

	//spellDesc.curatedSpellList = new wxTreeCtrl(spellList);

	if (obj == spellDesc.spellSearch)
	{
		std::string str = spellDesc.spellSearch->GetValue().ToStdString();
		auto& list = spellDesc.spellList;
		auto& spellLevelId = spellDesc.spellLevelId;

		SetEvtHandlerEnabled(false);
		list->Freeze();

		for (int i = 0; i < 10; ++i)
			list->DeleteChildren(spellLevelId[i]);

		if (str == "")
			spellDesc.fillAllSpellTree(allSpells);

		else
		{
			Util::toLowerString(str);
						
			for (auto it = allSpells.begin(); it != allSpells.end(); ++it)
			{
				std::string spellName = it->getName();
				Util::toLowerString(spellName);

				if (spellName.find(str) != std::string::npos)
				{
					int level = it->getLevel();

					list->AppendItem(spellLevelId[level], it->getName());
				}
			}
			list->ExpandAll();
		}

		list->Thaw();
		SetEvtHandlerEnabled(true);
	}
}

void MainFrame::onSpellSearch(wxCommandEvent& event)
{
	TransferDataFromWindow();
	std::string str = event.GetString().ToStdString();
	updateDispSpell(str);
	updateSpellDesc();
	return;	
}

void MainFrame::onSpellTreeSelect(wxTreeEvent& event)
{
	TransferDataFromWindow();
	wxTreeItemId tempID = spellDesc.spellList->GetSelection();

	std::string selectedSpell = spellDesc.spellList->GetItemText(tempID).ToStdString();
	updateDispSpell(selectedSpell);

	updateSpellDesc();
}

void MainFrame::onACCheckBoxTick(wxCommandEvent& event)
{
	TransferDataFromWindow();
	int newAC = character.getAC();
	
	if (mainPagePanels.AC_MageArmor->IsChecked())
		newAC = 13 + character.getSkillMod(Skills::Dexterity);

	if (mainPagePanels.AC_Shield->IsChecked())
		newAC = newAC + 2;

	mainPagePanels.AC->SetLabel(std::to_string(newAC));

}

void MainFrame::onGiveTempHPButton(wxCommandEvent& event)
{
	TransferDataFromWindow();
	int x = wxGetNumberFromUser("Enter temporary hitpoints:", "", "", 0L, 0L, 100L);
	mainPagePanels.TempHP->SetValue(x);
	character.giveTempHP(x);
}

void MainFrame::onSetArmorButton(wxCommandEvent& event)
{
	//wxObject* eventObject = event.GetEventObject();

	//if (eventObject == mainPagePanels.AC_AddArmor)
	//{
	//	int Base = wxGetNumberFromUser("Enter Armor Base:", "", "", 0L, 0L, 100L);
	//	int Mod = wxGetNumberFromUser("Enter Mod:", "", "", 0L, 0L, 10L);
	//	
	//	int (*func)() = [](int) { return Base + Mod; };
	//	character.addAC_Func( );
	//}
}

void MainFrame::onHealToButton(wxCommandEvent& event)
{
	auto& spin = std::get<1>(mainPagePanels.HealToX);
	auto& HP = mainPagePanels.HP;
	float perc = static_cast<float>(spin->GetValue()) / 100.f;

	int healedHP = perc * (character.getModTotHP());
	int curHP = character.getCurHP();

	if (curHP < healedHP)
	{
		character.setCurHP(healedHP);
		HP->SetValue(healedHP);
	}
}

void MainFrame::onHPSpin(wxSpinEvent& event)
{
	int x = mainPagePanels.HP->GetValue();
	character.setCurHP(x);
}

void MainFrame::onTempHPSpin(wxSpinEvent& event)
{
	int x = mainPagePanels.TempHP->GetValue();
	character.giveTempHP(x);
}

void MainFrame::onInitMod(wxCommandEvent& event)
{
	int x = 0;
	std::string temp = mainPagePanels.InitMod.second->GetValue().ToStdString();
	if (temp == "")
	{
		x = 0;
		mainPagePanels.InitMod.first->SetValue("0");
	}
	else
		x = std::stoi(temp);

	character.setInitMod(x);
	mainPagePanels.InitMod.first->SetValue(std::to_string(character.getInitiative()));
}

void MainFrame::onMaxHPChange(wxCommandEvent& event)
{
	updateHP();
}

void MainFrame::onAddRemSlider(wxCommandEvent& event)
{
	auto obj = event.GetEventObject();
	//ADD
	if (obj == mainPagePanels.SlidersButtons.second)
	{
		auto panel = mainPagePanels.SlidersButtons.second->GetParent();
		auto sizer = panel->GetSizer();
		
		SliderDialog dialog(this);

		int release = dialog.ShowModal();
		if (release != wxID_CANCEL)
		{
			auto title = dialog.getTitle();
			int min = dialog.getMin();
			int max = dialog.getMax();
			int def = dialog.getDef();
			auto slider = new wxSlider(panel, wxID_ANY, def, min, max, wxDefaultPosition, wxDefaultSize, wxSL_MIN_MAX_LABELS | wxSL_TICKS);

			auto text = new wxStaticText(panel, wxID_ANY, title, wxDefaultPosition, wxDefaultSize);
			auto horLine = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(1.5*baseColSize.x,-1)));

			auto value = new wxTextCtrl(panel, wxID_ANY, std::to_string(def), wxDefaultPosition, FromDIP(wxSize(50, -1)), wxALIGN_CENTER_HORIZONTAL);

			value->SetValidator(wxTextValidator(wxFILTER_NUMERIC));

			text->SetForegroundColour(panelColour.second);
			slider->SetForegroundColour(mainColour.second);
			setWindowColour(value, ctrlColour);

			auto font = text->GetFont();
			font.MakeLarger();
			font.MakeBold();
			font.MakeUnderlined();
			text->SetFont(font);

			value->SetFont(value->GetFont().MakeBold().MakeLarger());

			auto subSizer = new wxBoxSizer(wxHORIZONTAL);

			auto sizeFlag = wxALIGN_CENTER;

			subSizer->Add(text, 0, sizeFlag);
			subSizer->Add(value, 0, sizeFlag | wxLEFT, 5);
			subSizer->Add(slider, 1, wxEXPAND | wxLEFT, 10);
			//subSizer->Add(-1,5);

			sizer->Add(-1, 3);
			sizer->Add(subSizer, 0, wxEXPAND | wxRIGHT, 20);
			sizer->Add(horLine, 0, wxEXPAND);
			sizer->Add(-1, 3);

			mainPagePanels.Sliders.push_back({text, slider});
			mainPagePanels.sliderLine.push_back(horLine);
			mainPagePanels.SliderVal.push_back(value);

			character.addSlider({title, slider->GetValue()});

			panel->FitInside();
			panel->Layout();

			slider->Bind(wxEVT_SLIDER, &MainFrame::onSliderChange, this);
			value->Bind(wxEVT_TEXT, &MainFrame::onSliderValChange, this);
		}

	}

	//REM
	if (obj == mainPagePanels.SlidersButtons.first)
	{
		SliderRemDialog dialog(this, wxID_ANY, mainPagePanels.Sliders);

		int release = dialog.ShowModal();
		if (release != wxID_CANCEL)
		{
			int x = dialog.getSelected();
			if (x != -1)
			{
				mainPagePanels.Sliders[x].second->Unbind(wxEVT_SLIDER, &MainFrame::onSliderChange, this);
				delete mainPagePanels.Sliders[x].first;
				delete mainPagePanels.Sliders[x].second;
				delete mainPagePanels.sliderLine[x];
				delete mainPagePanels.SliderVal[x];
				
				mainPagePanels.Sliders.erase(mainPagePanels.Sliders.begin() + x);
				mainPagePanels.sliderLine.erase(mainPagePanels.sliderLine.begin() + x);
				mainPagePanels.SliderVal.erase(mainPagePanels.SliderVal.begin() + x);
				character.remSlider(x);

				auto panel = mainPagePanels.SlidersButtons.second->GetParent();

				panel->FitInside();
				panel->Layout();
			}
		}
	}
}

void MainFrame::onSliderChange(wxCommandEvent& event)
{
	auto obj = event.GetEventObject();
	int sliderNum = 0;
	
	for (auto it = mainPagePanels.Sliders.begin(); it != mainPagePanels.Sliders.end(); ++it)
	{
		if (obj == it->second)
			break;
		sliderNum++;
	}

	auto& slider = mainPagePanels.Sliders[sliderNum].second;
	int val = slider->GetValue();

	character.updateSlider(mainPagePanels.Sliders[sliderNum].first->GetLabel().ToStdString(), val);
	mainPagePanels.SliderVal[sliderNum]->SetValue(std::to_string(val));
}

void MainFrame::onSliderValChange(wxCommandEvent& event)
{
	auto obj = event.GetEventObject();
	int sliderNum = 0;
	for (auto it = mainPagePanels.SliderVal.begin(); it != mainPagePanels.SliderVal.end(); ++it)
	{
		if (obj == *it)
			break;
		sliderNum++;
	}

	auto& slider = mainPagePanels.Sliders[sliderNum].second;
	auto& value = mainPagePanels.SliderVal[sliderNum];
	auto str = value->GetValue().ToStdString();
	int num = 0;
	if (str != "" && str != "-")
	{
		num = std::stoi(str);

		if (num < slider->GetMin())
		{
			num = slider->GetMin();
			value->SetValue(std::to_string(num));
		}
		else if (num > slider->GetMax())
		{
			num = slider->GetMax();
			value->SetValue(std::to_string(num));

		}
	}

	if (str == "")
		num = slider->GetMin();

	if (str == "-")
		num = 0;


	slider->SetValue(num);
	character.updateSlider(mainPagePanels.Sliders[sliderNum].first->GetLabel().ToStdString(), num);


}

void MainFrame::onFeatureButton(wxCommandEvent& event)
{
	TransferDataFromWindow();
	wxObject* eventObject = event.GetEventObject();

	if (eventObject == mainPagePanels.Feature_AddButton)
	{
		wxString tempString;
		feature tempFeature;
		int release = wxID_OK;

		//wxTextEntryDialog* dialog = new wxTextEntryDialog(this, "Enter feature title:", "Add Feature");
		FeatureDialog* dialog = new FeatureDialog(this);
		release = dialog->ShowModal();
		
		if (release != wxID_CANCEL)
		{
			tempString = dialog->getTitle();
			tempFeature.title = tempString;

			tempString = dialog->getDescription();
			tempFeature.description = tempString;

			std::vector<feature>& allFeatures = character.getFeatures();
			for (auto it = allFeatures.begin(); it != allFeatures.end(); ++it)
			{
				if (*it == tempFeature)
				{
					wxMessageBox("Feature already exists", "Error");
					return;
				}
			}

			character.addFeature(tempFeature);

			updateFeaturesList();
		}

		dialog->Destroy();
		return;
	}

	if (eventObject == mainPagePanels.Feature_RemoveButton)
	{
		wxListBox* list = mainPagePanels.Feature_FullList;

		int i = list->GetSelection();
		if (i == wxNOT_FOUND)
			return;
		else
		{
			std::vector<feature>& features = character.getFeatures();

			for (auto it = features.begin(); it != features.end(); ++it)
			{
				std::string str = list->GetString(i).ToStdString();
				if (str == it->title)
				{
					it = features.erase(it);
					break;
				}
			}

			list->Deselect(i);
			list->Delete(i);
		}

		mainPagePanels.Feature_Title->SetLabel("");
		mainPagePanels.Feature_Desc->SetPage("");
		setWindowColour(mainPagePanels.Feature_Desc, descColour);
		updateFeaturesList();
		return;
	}
}

void MainFrame::onFeatureSelect(wxCommandEvent& event)
{
	TransferDataFromWindow();
	wxObject* eventObject = event.GetEventObject();
	
	wxListBox* list = mainPagePanels.Feature_FullList;
	wxStaticText* title = mainPagePanels.Feature_Title;
	wxHtmlWindow* desc = mainPagePanels.Feature_Desc;

	if (eventObject == list)
	{
		int i = list->GetSelection();
		if (i == wxNOT_FOUND)
		{
			title->SetLabel("Feature Name");
			desc->SetPage("Feature Description");
			setWindowColour(desc, descColour);
		}
		else
		{
			wxString str = list->GetString(i);
			std::vector<feature>& features = character.getFeatures();

			for (auto& i : features)
			{
				if (i.title == str)
				{
					title->SetLabel("  " + i.title);
					desc->SetPage("<b>" + i.description + "</b>");
					setWindowColour(desc, descColour);
					return;
				}
			}
		}
	}

	updateFeaturesList();

}

void MainFrame::onSpellSplitterResize(wxSplitterEvent& event)
{
	TransferDataFromWindow();
	if (event.GetEventType() == wxEVT_SPLITTER_SASH_POS_CHANGING)
	{ }
	if (event.GetEventType() == wxEVT_SPLITTER_SASH_POS_CHANGED)
	{
		updateSpellDesc();
	}

	if (event.GetEventType() == wxEVT_SPLITTER_DOUBLECLICKED)
	{

	}
}

void MainFrame::onKnownSpellsEvents(wxCommandEvent& event)
{
	TransferDataFromWindow();
	if (event.GetEventType() == wxEVT_COMBOBOX)
	{
		updateKnownSpellMods();
	}
}

void MainFrame::onKnownSpellsSelectSpell(wxCommandEvent& event)
{
	wxObject* obj = event.GetEventObject();
	for (int i = 0; i < knownPagePanels.SpellSlotLevelList.size(); ++i)
	{
		auto& list = std::get<3>(knownPagePanels.SpellSlotLevelList[i]);

		if (obj != list)
			list->DeselectAll();

		if (obj == list)
		{
			std::string tempStr = list->GetString(list->GetSelection()).ToStdString();
			updateKnownDispSpell(tempStr);
			updateKnownSpellDesc();
		}

	}

}

void MainFrame::onKnownSpellsAddRemSpell(wxCommandEvent& event)
{
	TransferDataFromWindow();
	wxObject* obj = event.GetEventObject();
	bool evtHandled = false;
	wxString tempString;

	for (int i = 0; i < knownPagePanels.AddSpellSlot_Button.size(); ++i)
	{
		wxButton* tempButton = knownPagePanels.AddSpellSlot_Button[i];
		wxListBox* list = std::get<3>(knownPagePanels.SpellSlotLevelList[i]);

		if (obj == tempButton)
		{
			evtHandled = true;
			int num = getNumSpells(i);
			wxString* choices = new wxString[num];

			for (int j = 0; j < num; j++)
			{
				choices[j] = allSpellsLevel[i][j]->getName();
			}

			//wxSingleChoiceDialog* dialog = new wxSingleChoiceDialog(this, "Select Spell", "Add Spell", num, choices);
			auto dialog = new SpellSelectDialog(this, wxID_ANY, num, choices);

			int release = dialog->ShowModal();
			if (release != wxID_CANCEL)
			{
				tempString = dialog->getSelection();
				if (tempString != "")
				{
					list->Append(tempString);

					for (auto& i : allSpells)
					{
						if (i.getName() == tempString)
						{
							character.addSpell(i);
							break;
						}
					}
					calcCheckedSpells();
					break;
				}
			}

			//knownPagePanels.selectedSpells;
			delete dialog;
			return;
		}	
	}

	for (int i = 0; i < knownPagePanels.RemSpellSlot_Button.size(); ++i)
	{
		wxButton* tempButton = knownPagePanels.RemSpellSlot_Button[i];
		wxListBox* list = std::get<3>(knownPagePanels.SpellSlotLevelList[i]);

		if (obj == tempButton)
		{
			evtHandled = true;
			int item = list->GetSelection();
			
			if (item == -1)
				return;

			list->Delete(item);

			calcCheckedSpells();

			return;
		}
	}

	event.Skip();
}

void MainFrame::onTickSpellKnownSpells(wxCommandEvent& event)
{
	calcCheckedSpells();
}

void MainFrame::onKnownSpellsUseSpell(wxCommandEvent& event)
{
	wxObject* obj = event.GetEventObject();

	for (int i = 0; i < knownPagePanels.AddSpellSlot_Button.size(); ++i)
	{
		auto& curButton = std::get<2>(knownPagePanels.SpellSlotLevelList[i]);
		auto& spin = std::get<1>(knownPagePanels.SpellSlotLevelList[i]);
		if (obj == curButton)
		{
			int curVal = spin->GetValue();
			if (curVal != 0)
				spin->SetValue(curVal - 1);
			
			return;
		}
	}

	event.Skip();
}

void MainFrame::onKnownSpellsUseSpellPoint(wxCommandEvent& event)
{
	wxObject* obj = event.GetEventObject();

	for (int i = 0; i < knownPagePanels.AddSpellSlot_Button.size(); ++i)
	{
		auto& curButton = knownPagePanels.spellPointButton[i];
		auto& val = knownPagePanels.SpellPoints_Val;
		if (obj == curButton)
		{
			int curVal = std::stoi(val->GetValue().ToStdString());
			int newVal = curVal - convSlotToPoint(i);
			if (newVal >= 0)
				val->SetValue(std::to_string(newVal));

			return;
		}
	}

	event.Skip();
}

void MainFrame::onSetMenuEvents(wxCommandEvent& event)
{
	auto obj = event.GetId();

	if (obj == menuBarItems.SetMaxHP->GetId())
	{
		std::string title = "Enter Max HP";
		int x = wxGetNumberFromUser(title, "", "Max HP", 10, 6, 1e3);
		character.setTotHP(x);
		updateHP();
	}

	if (obj == menuBarItems.SetInitMod->GetId())
	{
		std::string title = "Enter Inititiaite Mod";
		int x = wxGetNumberFromUser(title, "", "Initiative Modifier", 0, -1e3, 1e3);
		character.setInitMod(x);
		
		updateInitiative();
	}

	if (obj == menuBarItems.SetName->GetId())
	{
		auto name = wxGetTextFromUser("Enter character name");
		character.setName(name.ToStdString());
		mainPagePanels.PlayerName->SetLabel(name);
	}

	if (obj == menuBarItems.SetSpeed->GetId())
	{
		int x = wxGetNumberFromUser("Enter Speed", "", "Speed", 30, 0, 240);
		character.setSpeed(x);
		mainPagePanels.Speed->SetValue(std::to_string(x));
	}

	if (obj == menuBarItems.SetStats->GetId())
	{
		StatsDialog dialog(this);

		int release = dialog.ShowModal();

		if (release != wxID_CANCEL)
		{
			auto stat = dialog.getStats();
			character.setStats(Stats(stat[0], stat[1], stat[2], stat[3], stat[4], stat[5]));
			updateStats();
		}
	}

	if (obj == menuBarItems.SetSP->GetId())
	{
		std::string title = "Enter number of Spell Points (def ";
		title += std::to_string(character.getTotalSpellPoints());
		title += ")";
		int max = character.getTotalSpellPoints();
		int x = wxGetNumberFromUser(title, "", "Spell Points", max, 0, 10*max);
		knownPagePanels.SpellPoints_Val->SetValue(std::to_string(x));
	}

	if (obj == menuBarItems.SetSavingThrows->GetId())
	{
		std::vector<Skills> skills =
		{
			Skills::Strength,
			Skills::Dexterity,
			Skills::Constitution,
			Skills::Intelligence,
			Skills::Wisdom,
			Skills::Charisma
		};

		SkillProfDialog dialog(this, wxID_ANY, skills, character.getSavingThrow());

		int release = dialog.ShowModal();
		if (release != wxID_CANCEL)
		{
			auto list = dialog.getList();
			int num = list->GetCount();
			int st = 0;

			for (int i = 0; i < num; ++i)
			{
				if (list->IsChecked(i))
				{
					st |= skills[i];
				}
			}

			character.setSavingThrowProfs(st);
			updateSavingThrows();
		}
	}

	if (obj == menuBarItems.SetSkillProfs->GetId())
	{
		std::vector<Skills> skills =
		{
			Skills::Acrobatics,
			Skills::Animal_Handling,
			Skills::Arcana,
			Skills::Athletics,
			Skills::Deception,
			Skills::History,
			Skills::Insight,
			Skills::Intimidation,
			Skills::Investigation,
			Skills::Medicine,
			Skills::Nature,
			Skills::Perception,
			Skills::Performance,
			Skills::Persuasion,
			Skills::Religion,
			Skills::Sleight_of_Hand,
			Skills::Stealth,
			Skills::Survival
		};

		SkillProfDialog dialog(this, wxID_ANY, skills, character.getSkillsProfs());

		int release = dialog.ShowModal();
		if (release != wxID_CANCEL)
		{
			auto list = dialog.getList();
			int num = list->GetCount();
			int sk = 0;

			for (int i = 0; i < num; ++i)
			{
				if (list->IsChecked(i))
				{
					sk |= skills[i];
				}
			}

			character.setSkillProfs(sk);
			updateSkills();
		}
	}

	if (obj == menuBarItems.SetExpertises->GetId())
	{
		std::vector<Skills> allSkills =
		{
			Skills::Acrobatics,
			Skills::Animal_Handling,
			Skills::Arcana,
			Skills::Athletics,
			Skills::Deception,
			Skills::History,
			Skills::Insight,
			Skills::Intimidation,
			Skills::Investigation,
			Skills::Medicine,
			Skills::Nature,
			Skills::Perception,
			Skills::Performance,
			Skills::Persuasion,
			Skills::Religion,
			Skills::Sleight_of_Hand,
			Skills::Stealth,
			Skills::Survival
		};

		std::vector<Skills> skills;
		for (auto it = allSkills.begin(); it != allSkills.end(); ++it)
		{
			if (*it & character.getSkillsProfs())
				skills.push_back(*it);
		}

		SkillProfDialog dialog(this, wxID_ANY, skills, character.getExpertises());

		int release = dialog.ShowModal();
		if (release != wxID_CANCEL)
		{
			auto list = dialog.getList();
			int num = list->GetCount();
			int sk = 0;

			for (int i = 0; i < num; ++i)
			{
				if (list->IsChecked(i))
				{
					sk |= skills[i];
				}
			}

			character.setExpertises(sk);
			updateSkills();
		}
		
	}

	if (obj == menuBarItems.SetSlots->GetId())
	{
		auto dialog = new SpellSlotDialog(this);
		int release = dialog->ShowModal();
		if (release != wxID_CANCEL)
		{
			overrides.spellSlot = true;
			uses.SpellSlots = false;

			SpellSlot newSlots;
			SpellSlot oldSlots = character.getSpellSlots();

			for (int i = 0; i < 10; ++i)
			{
				if (i == 0)
				{
					newSlots.slots.push_back({ oldSlots.slots[i].first, oldSlots.slots[i].second });
					continue;
				}
				
				int numSlots = dialog->getSlots(i);

				if (numSlots == -1)
				{
					wxMessageBox("Slots index out of bounds");
					return;
				}

				if (numSlots > 0)
					uses.SpellSlots = true;

				newSlots.slots.push_back({numSlots, oldSlots.slots[i].second});
			}

			if (uses.SpellSlots)
				character.setSpellSlots(newSlots);

			updateKnownSpellsLists();
		}
	}

	if (obj == menuBarItems.SetWarlockSlots->GetId())
	{
		auto dialog = new WarlockSlotsDialog(this);
		int release = dialog->ShowModal();
		if (release != wxID_CANCEL)
		{
			overrides.spellSlot = true;
			uses.SpellSlots = false;

			SpellSlot newSlots;
			SpellSlot oldSlots = character.getSpellSlots();
			
			for (int i = 0; i < 10; ++i)
			{
				if (i == 0)
				{
					newSlots.slots.push_back({ oldSlots.slots[i].first, oldSlots.slots[i].second });
					continue;
				}

				int numSlots = dialog->getNumSlots();
				int level = dialog->getLevel();
				
				if (numSlots != 0)
					uses.SpellSlots = true;

				if (i == level)
				{
					newSlots.slots.push_back({ oldSlots.slots[i].first , numSlots});
				}

				else
				{
					newSlots.slots.push_back({ oldSlots.slots[i].first , -1 });
				}

				if (uses.SpellSlots)
					character.setSpellSlots(newSlots);
			}

			updateKnownSpellsLists();
		}
		
	}
}

void MainFrame::onResetMenuEvents(wxCommandEvent& event)
{
	auto obj = event.GetId();

	if (obj == menuBarItems.ResetSP->GetId())
	{
		knownPagePanels.SpellPoints_Val->SetLabel(std::to_string(character.getTotalSpellPoints()));
	}

	if (obj == menuBarItems.ResetSlots->GetId())
	{
		
	}

	if (obj == menuBarItems.ResetUncheckSpells->GetId())
	{

	}

	if (obj == menuBarItems.ResetDeleteSpells->GetId())
	{

	}

}

void MainFrame::onConditionMenuEvents(wxCommandEvent& event)
{
	auto obj = event.GetId();

	if (obj == menuBarItems.ConditionsAll->GetId())
	{
		ConditionDialog* dialog = new ConditionDialog(this, wxID_ANY, allConditions.size(), &allConditions, "Conditions", wxDefaultPosition, FromDIP(wxSize(500, -1)), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
		dialog->Show();
	}

	if (obj == menuBarItems.ConditionsPlayer->GetId())
	{
		updatePlayerConds();

		ConditionDialog* dialog = new ConditionDialog(this, wxID_ANY, playerConditions.size(), &playerConditions, "Conditions", wxDefaultPosition, FromDIP(wxSize(500, -1)), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
		dialog->Show();
	}

	if (obj == menuBarItems.ConditionsAdd->GetId())
	{
		not_playerConditions.clear();
		int cond = character.getConditions();
		for (auto it = allConditions.begin(); it != allConditions.end(); ++it)
		{
			if (it->first == Conditions::NoCondition)
				continue;
		
			if (it->first == Conditions::Incapacitated)
				continue;
			
			bool has = it->first & cond;
			has = !has;
			if (has)
			{
				not_playerConditions.push_back(*it);
			}
		}

		AddCondDialog dialog(this, wxID_ANY, not_playerConditions.size(), &not_playerConditions);

		auto release = dialog.ShowModal();

		if (release != wxID_CANCEL)
		{
			auto list = dialog.getList();
			for (int i = 0; i < not_playerConditions.size(); ++i)
			{
				if (list->IsChecked(i))
				{
					character.addCondition(not_playerConditions[i].first);
					int count = mainPagePanels.EL_Conditions->GetListCtrl()->GetItemCount();
					mainPagePanels.EL_Conditions->GetListCtrl()->InsertItem(count-1, not_playerConditions[i].second.title);
				}
			}
		}
	}

	if (obj == menuBarItems.ConditionsRemove->GetId())
	{
		updatePlayerConds();

		AddCondDialog dialog(this, wxID_ANY, playerConditions.size(), &playerConditions);

		auto release = dialog.ShowModal();

		if (release != wxID_CANCEL)
		{
			auto list = dialog.getList();
			for (int i = 0; i < playerConditions.size(); ++i)
			{
				if (list->IsChecked(i))
				{
					character.remCondition(playerConditions[i].first);
					int count = mainPagePanels.EL_Conditions->GetListCtrl()->GetItemCount();
					for (int j = 0; j < count-1; ++j)
					{
						std::string str = mainPagePanels.EL_Conditions->GetListCtrl()->GetItemText(j).ToStdString();
						if (str == playerConditions[i].second.title)
						{
							mainPagePanels.EL_Conditions->GetListCtrl()->DeleteItem(j);
							break;
						}
					}
				}
			}
		}
	}
}

void MainFrame::onDiceMenuEvents(wxCommandEvent& event)
{
	DiceRollerDialog* dialog = new DiceRollerDialog(this, wxID_ANY, "Roll Dice", wxDefaultPosition, FromDIP(wxSize(600,500)), 
		wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE);
	dialog->Show();
}

void MainFrame::onToolProfecsSelect(wxListEvent& event)
{

	//wxListCtrl* list = std::get<3>(mainPagePanels.ToolProf);
	//if (event.GetEventType() == wxEVT_LIST_ITEM_FOCUSED)
	//{
	//	int items = list->GetSelectedItemCount();
	//	long item = -1;
	//	for (;; )
	//	{
	//		item = list->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	//		if (item == -1)
	//			break;

	//		// this item is selected - do whatever is needed with it
	//		//std::string str = list->GetItemText(item).ToStdString();
	//		//wxMessageBox(str);
	//	}
	//}	
}

void MainFrame::onToolProfecs(wxCommandEvent& event)
{
	auto obj = event.GetEventObject();
	auto& list = std::get<3>(mainPagePanels.ToolProf);
	
	if (obj == std::get<1>(mainPagePanels.ToolProf))
	{
		wxString str = "";
		str = wxGetTextFromUser("Enter profecient item:", "Tools", "Default Tool");
		if (str == "")
			return;
		else
		{
			list->Append(str);
			character.addTool(str.ToStdString());
		}
	}

	if (obj == std::get<2>(mainPagePanels.ToolProf))
	{
		int i = list->GetSelection();
		character.remTool(list->GetString(i).ToStdString());
		list->Delete(i);
		
	}
}

void MainFrame::onLangProfecs(wxCommandEvent& event)
{
	auto obj = event.GetEventObject();
	auto& list = std::get<3>(mainPagePanels.LangProf);

	if (obj == std::get<1>(mainPagePanels.LangProf))
	{
		wxString str = "";
		str = wxGetTextFromUser("Enter Language:", "Languages", "Common");
		if (str == "")
			return;
		else
		{
			list->Append(str);
			character.addLanguage(str.ToStdString());
		}
	}

	if (obj == std::get<2>(mainPagePanels.LangProf))
	{
		int i = list->GetSelection();
		character.remLanguage(list->GetString(i).ToStdString());
		list->Delete(i);
	}
}

void MainFrame::onAddRemMoney(wxCommandEvent& event)
{
	auto obj = event.GetEventObject();

	MoneyDialog dialog(this);

	if (dialog.ShowModal() == wxID_OK)
	{
		int p = dialog.getPlatinnum();
		int g = dialog.getGold();
		int s = dialog.getSilver();
		int c = dialog.getCopper();

		if (obj == mainPagePanels.moneyButtons.first)
		{
			if (p > character.getPlatinum())
				wxMessageBox("You don't have that much platinum");
			else if (g > character.getGold())
				wxMessageBox("You don't have that much gold");
			else if (s > character.getSilver())
				wxMessageBox("You don't have that much silver");
			else if (c > character.getCopper())
				wxMessageBox("You don't have that much copper");
			else
				character.addMoney(-p, -g, -s, -c);

		}

		if (obj == mainPagePanels.moneyButtons.second)
			character.addMoney(p, g, s, c);
	}

	updateMoneyCtrls();
}

void MainFrame::onConditionListDClick(wxCommandEvent& event)
{
	updatePlayerConds();
	
	auto list = mainPagePanels.EL_Conditions->GetListCtrl();
	int items = list->GetSelectedItemCount();
	int totCount = list->GetItemCount();
	if (totCount == 1)
		return;
	long item = -1;
	
	ConditionDialog* dialog = new ConditionDialog(this, wxID_ANY, playerConditions.size(), &playerConditions, "Conditions", wxDefaultPosition, FromDIP(wxSize(500, -1)), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

	for (;; )
	{
		item = list->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (item == -1)
			break;

		// this item is selected - do whatever is needed with it
		std::string str = list->GetItemText(item).ToStdString();
		dialog->setSelection(str);
	}
	
	dialog->Show();
}

//------------------------------
///CONDITIONS
//------------------------------

void MainFrame::initConditions()
{
	Conditions curCond;
	feature curFeature;

	curCond = Conditions::NoCondition;
	curFeature.title = "No Conditions";
	curFeature.description = "No Effect";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Blinded;
	curFeature.title = "Blinded";
	curFeature.description = "A blinded creature canft see and automatically fails any ability check that requires sight.\n\n";
	curFeature.description += "Attack rolls against the creature have advantage, and the creaturefs attack rolls have disadvantage.\n\n";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Charmed;
	curFeature.title = "Charmed";
	curFeature.description = "A charmed creature canft attack the charmer or target the charmer with harmful abilities or magical effects.\n\n";
	curFeature.description += "The charmer has advantage on any ability check to interact socially with the creature.";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Deafened;
	curFeature.title = "Deafened";
	curFeature.description = "A deafened creature canft hear and automatically fails any ability check that requires hearing.";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Frightened;
	curFeature.title = "Frightened";
	curFeature.description = "A frightened creature has disadvantage on ability checks and attack rolls while the source of its fear is within line of sight.\n\n";
	curFeature.description += "The creature canft willingly move closer to the source of its fear.";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Graplled;
	curFeature.title = "Grappled";
	curFeature.description = "A grappled creaturefs speed becomes 0, and it canft benefit from any bonus to its speed.\n\n";
	curFeature.description += "The condition ends if the grappler is incapacitated.\n\n";
	curFeature.description += "The condition also ends if an effect removes the grappled creature from the reach of the grappler or grappling effect, such as when a creature is hurled away by the thunderwave spell.";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Incapacitated;
	curFeature.title = "Incapacitated";
	curFeature.description = "An incapacitated creature canft take actions or reactions.";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Invisible;
	curFeature.title = "Invisible";
	curFeature.description = "An invisible creature is impossible to see without the aid of magic or a special sense. For the purpose of hiding, the creature is heavily obscured. The creaturefs location can be detected by any noise it makes or any tracks it leaves.\n\n";
	curFeature.description += "Attack rolls against the creature have disadvantage, and the creaturefs attack rolls have advantage.";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Paralyzed;
	curFeature.title = "Paralyzed";
	curFeature.description = "A paralyzed creature is incapacitated and canft move or speak.\n\n";
	curFeature.description += "The creature automatically fails Strength and Dexterity saving throws.\n\n";
	curFeature.description += "Attack rolls against the creature have advantage.\n\n";
	curFeature.description += "Any attack that hits the creature is a critical hit if the attacker is within 5 feet of the creature.";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Petrified;
	curFeature.title = "Petrified";
	curFeature.description = "A petrified creature is transformed, along with any nonmagical object it is wearing or carrying, into a solid inanimate substance (usually stone). Its weight increases by a factor of ten, and it ceases aging.\n\n";
	curFeature.description += "The creature is incapacitated (see the condition), canft move or speak, and is unaware of its surroundings.\n\n";
	curFeature.description += "Attack rolls against the creature have advantage.\n\n";
	curFeature.description += "The creature automatically fails Strength and Dexterity saving throws.\n\n";
	curFeature.description += "The creature automatically fails Strength and Dexterity saving throws.\n\n";
	curFeature.description += "The creature is immune to poison and disease, although a poison or disease already in its system is suspended, not neutralized.";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Poisoned;
	curFeature.title = "Poisoned";
	curFeature.description = "A poisoned creature has disadvantage on attack rolls and ability checks.";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Prone;
	curFeature.title = "Prone";
	curFeature.description = "A prone creaturefs only movement option is to crawl, unless it stands up and thereby ends the condition.\n\n";
	curFeature.description += "The creature has disadvantage on attack rolls.\n\n";
	curFeature.description += "An attack roll against the creature has advantage if the attacker is within 5 feet of the creature. Otherwise, the attack roll has disadvantage.\n\n";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Restrained;
	curFeature.title = "Restrained";
	curFeature.description = "A restrained creaturefs speed becomes 0, and it canft benefit from any bonus to its speed.\n\n";
	curFeature.description += "Attack rolls against the creature have advantage, and the creaturefs attack rolls have disadvantage.\n\n";
	curFeature.description += "The creature has disadvantage on Dexterity saving throws.\n\n";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Stunned;
	curFeature.title = "Stunned";
	curFeature.description = "A stunned creature is incapacitated (see the condition), canft move, and can speak only falteringly.\n\n";
	curFeature.description += "The creature automatically fails Strength and Dexterity saving throws.\n\n";
	curFeature.description += "Attack rolls against the creature have advantage.\n\n";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Unconscious;
	curFeature.title = "Unconscious";
	curFeature.description = "An unconscious creature is incapacitated (see the condition), canft move or speak, and is unaware of its surroundings\n\n";
	curFeature.description += "The creature drops whatever itfs holding and falls prone.\n\n";
	curFeature.description += "The creature automatically fails Strength and Dexterity saving throws.\n\n";
	curFeature.description += "Attack rolls against the creature have advantage.\n\n";
	curFeature.description += "Any attack that hits the creature is a critical hit if the attacker is within 5 feet of the creature.";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Exhaustion;
	curFeature.title = "Exhaustion";
	curFeature.description = "Level\tEffect\n";
	curFeature.description += "1    \tDisadvantage on ability checks\n";
	curFeature.description += "2    \tSpeed halved\n";
	curFeature.description += "3    \tDisadvantage on attack rolls and saving throws\n";
	curFeature.description += "4    \tHit point maximum halved\n";
	curFeature.description += "5    \tSpeed reduced to 0\n";
	curFeature.description += "6    \tDeath\n";
	curFeature.description += "\nIf an already exhausted creature suffers another effect that causes exhaustion, its current level of exhaustion increases by the amount specified in the effectfs description.\n";
	curFeature.description += "\nA creature suffers the effect of its current level of exhaustion as well as all lower levels. For example, a creature suffering level 2 exhaustion has its speed halved and has disadvantage on ability checks.\n";
	curFeature.description += "\nAn effect that removes exhaustion reduces its level as specified in the effectfs description, with all exhaustion effects ending if a creaturefs exhaustion level is reduced below 1.\n";
	curFeature.description += "\nFinishing a long rest reduces a creaturefs exhaustion level by 1, provided that the creature has also ingested some food and drink.\n";
	allConditions.push_back({ curCond, curFeature });

	curCond = Conditions::Fatigued;
	curFeature.title = "Fatigued";
	curFeature.description = "Fatigue can have levels upto 10. Things that add levels tell how many are added. A Long Rest removes all levels of fatigue while short rest removes only one.\n\n";
	curFeature.description += "Lesser Restoration can remove one level of Fatigue. Greater restoration removes all levels of Fatigue.\n\n";
	curFeature.description += "For each level of fatigue, you have a -1 on all ability checks, saving throws and attack rolls.";
	allConditions.push_back({ curCond, curFeature });
}

void MainFrame::setWindowColour(wxWindow* window, ColourPair colour)
{
	window->SetBackgroundColour(colour.first);
	window->SetForegroundColour(colour.second);
}

