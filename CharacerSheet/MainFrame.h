#pragma once

#include "AttackControl.h"
#include "Character.h"
#include "Spells.h"
#include "FileLogger.h"
#include "InventoryControl.h"

using ColourPair = std::pair<wxColour, wxColour>;

class MainFrame : public wxFrame
{
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& curInitiative;
		
		ar& shortRestSliders;
		ar& longRestSliders;
		ar& uses.SpellPoints;
		ar& uses.SpellSlots;

		ar& overrides.spellPoint;
		ar& overrides.spellSlot;

		ar& healToPercentageValue;

		ar& fileName;

		ar& notesPanels.pages;
		ar& notesPanels.pageNames;

		ar& character;
		
		ar& mainPagePanels.attackPanelAttacks;
	}

	std::string saveFolder = "Character Saves";
	std::string fileName = "";
	int healToPercentageValue = 100;
	int firstPage = 0;

	FileLogger healthLog;
	bool isLoggingHealth = false;

	std::vector<std::pair<Conditions, feature>> allConditions;
	std::vector<std::pair<Conditions, feature>> playerConditions;
	std::vector<std::pair<Conditions, feature>> not_playerConditions;
	int curInitiative;

	std::vector<int> shortRestSliders;
	std::vector<int> longRestSliders;

	std::vector<Spell> allSpells;
	std::vector<Spell*> allSpellsLevel[10];
	Character character;
	
	wxFont BigFont1;
	wxFont BigFont2;
	wxFont SkillFont;

	ColourPair ctrlColour;
	ColourPair descColour;
	ColourPair listColour;
	ColourPair mainColour;
	ColourPair panelColour;
	ColourPair slotsColour;
	wxColour proficientColour;
	wxColour expertiseColour;

	wxSize baseColSize;
	wxSize buttonSize;
	double acColSizeMod = 1.2;
	double hpColSizeMod = 1.3;
	double invColSizeMod = 1.4;

	wxSizer* masterSizer;
	wxPanel* masterPanel;
	wxNotebook* mainNotebook;

	struct Uses
	{
		bool SpellPoints = false;
		bool SpellSlots = true;
	}uses;	

	struct Overrides
	{
		bool spellSlot = false;
		bool spellPoint = false;
	} overrides;
	
	//
	struct MenuBarItems
	{
		wxMenuItem* FileHealthLogger;
		wxMenuItem* FileLevelUp;

		wxMenuItem* SetName;
		wxMenuItem* SetStats;
		wxMenuItem* SetSkillProfs;
		wxMenuItem* SetExpertises;
		wxMenuItem* SetSavingThrows;
		wxMenuItem* SetMaxHP;
		wxMenuItem* SetSpeed;
		wxMenuItem* SetInitMod;
		wxMenuItem* SetSP;
		wxMenuItem* SetSlots;
		wxMenuItem* SetWarlockSlots;
		wxMenuItem* SetSkillModifiers;
		wxMenuItem* SetSavingThrowModifier;
		

		wxMenuItem* ResetSP;
		wxMenuItem* ResetSlots;
		wxMenuItem* ResetUncheckSpells;
		wxMenuItem* ResetDeleteSpells;
		
		wxMenuItem* RestLong;
		wxMenuItem* RestShort;
		wxMenuItem* RestShortWithHitDice;

		wxMenuItem* RestAddSliderToShort;
		wxMenuItem* RestAddSliderToLong;
		wxMenuItem* RestRemSliderToShort;
		wxMenuItem* RestRemSliderToLong;

		wxMenuItem* ConditionsAll;
		wxMenuItem* ConditionsPlayer;
		wxMenuItem* ConditionsAdd;
		wxMenuItem* ConditionsRemove;

		wxMenuItem* NotesFont;
		wxMenuItem* NotesColour;
		wxMenuItem* NotesDefBGColour;
		
		wxMenuItem* DiceRoll;
		wxMenuItem* DiceAdv;

	}menuBarItems;

	//CONTROLS IN SPELLS PAGE
	struct SpellDesc
	{
		Spell* dispSpell = nullptr;

		wxSize ColWidth = wxSize(350,-1);

		wxStaticText* Title = nullptr;
		wxStaticText* Level = nullptr;
		wxStaticText* CastingTime = nullptr;
		wxStaticText* Range = nullptr;
		wxStaticText* Duration = nullptr;
		wxStaticText* Components = nullptr;
		wxStaticText* Type = nullptr;
		wxStaticText* AppClasses = nullptr;
		wxHtmlWindow* Description = nullptr;

		//SpellDesc(wxSize size = wxSize(350,-1));
		
		wxTreeCtrl* curatedSpellList = nullptr;
		wxTreeCtrl* fullSpellList = nullptr;
		wxTreeCtrl* spellList = nullptr;
		wxTreeItemId spellLevelId[10] = {};
		
		wxSearchCtrl* spellSearch = nullptr;

		wxSplitterWindow* spellSplitter = nullptr;

		void fillAllSpellTree(std::vector<Spell>& allSpells);

	} spellDesc;


	//CONTROLS IN MAIN PAGE
	struct MainPagePanels
	{
		//wxTextCtrl* PlayerName = nullptr;
		wxTextCtrl* PlayerName = nullptr;
		wxTextCtrl* Classes = nullptr;

		std::vector<std::pair<wxStaticText*, wxTextCtrl*>> SavingThrows;
		std::vector<std::pair<wxStaticText*, wxTextCtrl*>> Skills;

		wxStaticText* HPText = nullptr;
		wxSpinCtrl* HP = nullptr;
		wxTextCtrl* Speed = nullptr;

		wxButton* GiveTempHPButton = nullptr;
		wxSpinCtrl* TempHP = nullptr;

		//AC Stuff
		wxTextCtrl* AC_Base = nullptr;
		wxTextCtrl* AC_Mod = nullptr;
		wxTextCtrl* AC = nullptr;
		wxCheckBox* AC_Shield = nullptr;
		wxCheckBox* AC_MageArmor = nullptr;
		wxButton* AC_AddArmor = nullptr;

		wxStaticText* Feature_Title = nullptr;
		wxHtmlWindow* Feature_Desc = nullptr;
		wxListBox* Feature_FullList = nullptr;

		wxButton* Feature_AddButton = nullptr;
		wxButton* Feature_RemoveButton = nullptr;

		std::tuple<wxStaticText*, wxButton*, wxButton*, wxListBox*> ToolProf = {nullptr, nullptr, nullptr, nullptr};
		std::tuple<wxStaticText*, wxButton*, wxButton*, wxListBox*> LangProf = {nullptr, nullptr, nullptr, nullptr};
		wxEditableListBox* EL_Conditions = nullptr;

		wxTextCtrl* profBonus = nullptr;
		std::vector<std::pair<wxTextCtrl*, wxTextCtrl*>> Stat_TextCtrls;

		std::vector<wxSpinCtrl*> moneyVals;
		std::pair<wxButton*, wxButton*> moneyButtons = { nullptr, nullptr };

		std::tuple<wxButton*, wxButton*, wxSpinCtrl*, wxButton*> HealDamage;
		std::tuple<wxStaticText* ,wxSpinCtrl*, wxButton*> HealToX;
		std::tuple<wxStaticText*, wxSpinCtrl*> MaxHPBonus;

		std::pair<wxTextCtrl*, wxTextCtrl*> InitMod;
		wxSpinCtrl* curInit;

		std::vector<std::tuple<wxSpinCtrl*, wxStaticText*>> HitDie;

		std::pair<wxButton*, wxButton*> SlidersButtons;
		std::vector<wxTextCtrl*> SliderVal;
		std::vector<std::pair<wxStaticText* ,wxSlider*>> Sliders;
		std::vector<int> SliderDefaults;
		std::vector<wxStaticLine*> sliderLine;
		//MainPagePanels();

		AttackControl* AttackPanel = nullptr;
		std::vector<AttackControl::ListItem> attackPanelAttacks;

		InventoryControl* InventoryPanel = nullptr;

	}mainPagePanels;


	//CONTROLS IN THE KNOWN PAGE SPELLS
	struct KnownPagePanels
	{
		wxString selectedSpells ="0";
		Spell* dispSpell = nullptr;

		wxComboBox* Combo_SpellCastSkill = nullptr;
		
		wxTextCtrl* ProfBonus_Val = nullptr;
		wxTextCtrl* SpellCastMod_Val = nullptr;
		wxTextCtrl* SpellSaveMod_Val = nullptr;
		wxTextCtrl* SelectedSpells_Val = nullptr;
		
		wxStaticText* SpellPoints_Text = nullptr;
		wxTextCtrl* SpellPoints_Val = nullptr;
		std::vector<wxButton*> spellPointButton;

		std::vector<std::tuple<wxStaticText*, wxSpinCtrl*, wxButton*, wxCheckListBox*>> SpellSlotLevelList;
		std::vector<wxButton*> AddSpellSlot_Button;
		std::vector<wxButton*> RemSpellSlot_Button;

		//Spell Details
		wxStaticText* Title = nullptr;
		wxStaticText* Level = nullptr;
		wxStaticText* CastingTime = nullptr;
		wxStaticText* Range = nullptr;
		wxStaticText* Duration = nullptr;
		wxStaticText* Components = nullptr;
		wxStaticText* Type = nullptr;
		wxStaticText* AppClasses = nullptr;
		wxHtmlWindow* Description = nullptr;

	}knownPagePanels;

	struct NotesPanels
	{
		std::vector<std::string> pages;
		std::vector<std::string> pageNames;
		wxButton* AddPage = nullptr;
		wxButton* RemPage = nullptr;
		wxListBox* PageList = nullptr;
		wxTextCtrl* PageText = nullptr;

		int curPageNum = 0;

	}notesPanels;

public:
	MainFrame(const wxString& title, const Character& pChar);

	void CreateMenuBar();

	wxScrolled<wxPanel>* CreateMainPage(wxNotebook* parent);
	wxScrolled<wxPanel>* CreateSpellsPage(wxNotebook* parent);
	wxScrolled<wxPanel>* CreateKnownSpellsPage(wxNotebook* parent);
	wxScrolled<wxPanel>* CreateSpellSlotsTable(wxNotebook* parent);
	wxScrolled<wxPanel>* createInventoryListPage(wxNotebook* parent);
	wxScrolled<wxPanel>* CreateInventoryPage(wxNotebook* parent);
	wxScrolled<wxPanel>* CreateAnimalCardsPage(wxNotebook* parent);
	wxScrolled<wxPanel>* CreateNotesPage(wxNotebook* parent);
	wxScrolled<wxPanel>* CreateTestPanel(wxNotebook* parent);


	void BindControls();

	//----------------------
	//Main Page Functions
	//----------------------
	wxPanel* CreateNamePanel(wxPanel* parent);
	wxPanel* CreateHPPanel(wxPanel* parent);
	wxPanel* CreateTempHPPanel(wxPanel* parent);
	wxPanel* CreateSpeedPanel(wxPanel* parent);
	wxPanel* CreateACPanel(wxPanel* parent);
	wxPanel* CreateStats(wxPanel* parent);
	wxPanel* CreateSavingThrows(wxPanel* parent);
	wxPanel* CreateSkillProf(wxPanel* parent);
	wxPanel* CreateFeaturesPanel(wxPanel* parent);
	wxPanel* CreateToolProficiencies(wxPanel* parent);
	wxPanel* CreateLangProficiencies(wxPanel* parent);
	wxPanel* CreatePlayerConditions(wxPanel* parent);
	wxPanel* CreateMoney(wxPanel* parent);
	wxPanel* CreateSubHPPanel(wxPanel* parent);
	wxPanel* CreateInitiativePanel(wxPanel* parent);
	wxPanel* CreateDeathSavesPanel(wxPanel* parent);
	wxScrolled<wxPanel>* CreateSliderPanel(wxPanel* parent);
	wxScrolled<wxPanel>* CreateHitDiePanel(wxPanel* parent);


	//----------------------
	//Spells Page Functions
	//----------------------
	wxPanel* CreateSpellsDescPanel(wxWindow* parent);
	wxPanel* CreateSpellTreeList(wxWindow* parent);

	//----------------------
	//Known Spells Page Functions
	//----------------------
	wxPanel* CreateKnownSpellAbilityPanel(wxPanel* parent);
	wxPanel* CreateKnownSpellsAllLevels(wxPanel* parent);
	wxPanel* CreateKnownSpells_SpellSlot(wxPanel* parent, int spellLevel);


	//----------------------
	//Other Functions
	//----------------------

	wxPanel* CreateSpellDescription(wxPanel* parent, Spell& dispSpell, wxStaticText*& Title, wxStaticText*& Level, 
		wxStaticText*& CastingTime, wxStaticText*& Range, wxStaticText*& Duration, wxStaticText*& Components, wxStaticText*& Type, 
		wxStaticText*& AppClasses, wxHtmlWindow*& Description, wxSize size = wxSize(350, -1));

	void CreatePages(wxNotebook* parent);

	void updateDispSpell(std::string str);
	void updateSpellDesc();

	void updateKnownSpellDesc();
	void updateKnownDispSpell(std::string str);
	void updateKnownSpellMods();
	void updateKnownSpellsLists();

	void updateAC();
	void updateName();
	void updateTempHP();
	void updateHP();
	void updateProficiencies();
	void updateStats();
	void updateInitiative();
	void updateSavingThrows();
	void updateSkills();
	void updateSpeed();

	void updateMoneyCtrls();
	void updatePlayerConds();
	void updateFeaturesList();
	void updateNotes();
	void updateSliders();
	void updateLangProfs();
	void updateToolProfs();
	void updateConditions();
	void updateHitDie();

	void updateAll();

	void calcCheckedSpells();

	void DrawMain();

	void rollHitDice();

	void makeSlider(std::string title, int val, int min, int max, int def);
	void makeSavingThrowPair(wxStaticText* savingThrowName, wxTextCtrl* savingThrowValue, Skills curSkill);
	void makeSkillPair(wxStaticText* skillName, wxTextCtrl* skillValue, Skills curSkill);
	void makeAddRemList(wxStaticText*& title, wxButton*& add, wxButton*& rem, wxListBox*& list, wxSizer* sizer, wxPanel* parent);

	void calcAC();
	void HealToPerc();
	void FillRegularSlots();
	void FillWarlockSlots();

	void DefaultLongSliders();
	void DefaultShortSliders();

	void callSave(bool saveAs = false);

	void AttackPanelSave();
	void AttackPanelLoad();

	//----------------------
	//Event Handlers
	//----------------------

	void onSpellSearchType(wxCommandEvent& event);
	void onSpellSearch(wxCommandEvent& event);
	void onSpellTreeSelect(wxTreeEvent& event);
	
	void onACValChange(wxCommandEvent& event);
	void onACCheckBoxTick(wxCommandEvent& event);
	void onGiveTempHPButton(wxCommandEvent& event);
	void onSetArmorButton(wxCommandEvent& event);
	void onHealDamageButton(wxCommandEvent& event);
	void onHealToButton(wxCommandEvent& event);
	void onHealToSpin(wxCommandEvent& event);
	void onHPSpin(wxSpinEvent& event);
	void onTempHPSpin(wxSpinEvent& event);
	void onInitMod(wxCommandEvent& event);
	void onMaxHPChange(wxCommandEvent& event);
	void onAddRemSlider(wxCommandEvent& event);
	void onSliderChange(wxCommandEvent& event);
	void onSliderValChange(wxCommandEvent& event);
	void onFeatureButton(wxCommandEvent& event);
	void onFeatureSelect(wxCommandEvent& event);
	void onToolProfecsSelect(wxListEvent& event);
	void onToolProfecs(wxCommandEvent& event);
	void onLangProfecs(wxCommandEvent& event);
	void onAddRemMoney(wxCommandEvent& event);
	void onSpinMoney(wxSpinEvent& event);
	void onConditionListDClick(wxCommandEvent& event);
	void onCurInitiativeChange(wxSpinEvent& event);

	void onSpellSplitterResize(wxSplitterEvent& event);

	void onKnownSpellsEvents(wxCommandEvent& event);
	void onKnownSpellsSelectSpell(wxCommandEvent& event);
	void onKnownSpellsAddRemSpell(wxCommandEvent& event);
	void onTickSpellKnownSpells(wxCommandEvent& event);
	void onKnownSpellsUseSpell(wxCommandEvent& event);
	void onKnownSpellsUseSpellPoint(wxCommandEvent& event);
	
	void onNotesType(wxCommandEvent& event);
	void onNotesAddRem(wxCommandEvent& event);
	void onNotesSelect(wxCommandEvent& event);
	void onNotesDClick(wxCommandEvent& event);

	void onFileMenuEvents(wxCommandEvent& event);
	void onSetMenuEvents(wxCommandEvent& event);
	void onResetMenuEvents(wxCommandEvent& event);
	void onRestMenuEvents(wxCommandEvent& event);
	void onConditionMenuEvents(wxCommandEvent& event);
	void onDiceMenuEvents(wxCommandEvent& event);
	void onNotesMenuEvents(wxCommandEvent& event);

	void onTest(wxCommandEvent& event);

	void onExit(wxCloseEvent& event);

	void loadSpells();
	void fillLevelSpells();
	int getNumSpells(int level);
	void initConditions();

	void setWindowColour(wxWindow* window, ColourPair colour);
};

