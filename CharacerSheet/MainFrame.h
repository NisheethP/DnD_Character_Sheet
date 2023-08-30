#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/notebook.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/listbox.h>
#include <wx/editlbox.h>
#include <wx/statline.h>
#include <wx/html/htmlwin.h>
#include <wx/splitter.h>

#include "Character.h"
#include <vector>
#include <tuple>
#include "Spells.h"


using ColourPair = std::pair<wxColour, wxColour>;

class MainFrame : public wxFrame
{
	int firstPage = 0;

	std::vector<std::pair<Conditions, feature>> allConditions;
	std::vector<std::pair<Conditions, feature>> playerConditions;
	std::vector<std::pair<Conditions, feature>> not_playerConditions;

	std::vector<Spell> allSpells;
	std::vector<Spell*> allSpellsLevel[10];
	Character character;
	wxFont BigFont1;
	wxFont BigFont2;

	ColourPair ctrlColour;
	ColourPair descColour;
	ColourPair listColour;
	ColourPair mainColour;
	ColourPair panelColour;
	ColourPair slotsColour;
	wxColour profecientColour;

	wxSize baseColSize;

	wxSizer* masterSizer;
	wxPanel* masterPanel;
	wxNotebook* mainNotebook;

	struct Uses
	{
		bool SpellPoints = true;
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
		wxMenuItem* SetName;
		wxMenuItem* SetStats;
		wxMenuItem* SetSkillProfs;
		wxMenuItem* SetSavingThrows;
		wxMenuItem* SetMaxHP;
		wxMenuItem* SetSpeed;
		wxMenuItem* SetSP;
		
		wxMenuItem* ResetSP;

		wxMenuItem* ConditionsAll;
		wxMenuItem* ConditionsPlayer;
		wxMenuItem* ConditionsAdd;
		wxMenuItem* ConditionsRemove;
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
		wxStaticText* PlayerName = nullptr;
		wxStaticText* Classes = nullptr;

		std::vector<std::pair<wxStaticText*, wxTextCtrl*>> SavingThrows;
		std::vector<std::pair<wxStaticText*, wxTextCtrl*>> Skills;

		wxStaticText* HPText = nullptr;
		wxSpinCtrl* HP = nullptr;
		wxTextCtrl* Speed = nullptr;

		wxButton* GiveTempHPButton = nullptr;
		wxSpinCtrl* TempHP = nullptr;

		//AC Stuff
		wxTextCtrl* AC = nullptr;
		wxCheckBox* AC_Shield = nullptr;
		wxCheckBox* AC_MageArmor = nullptr;
		wxCheckListBox* AC_Mod = nullptr;
		wxButton* AC_AddArmor = nullptr;

		wxStaticText* Feature_Title = nullptr;
		wxHtmlWindow* Feature_Desc = nullptr;
		wxListBox* Feature_FullList = nullptr;

		wxButton* Feature_AddButton = nullptr;
		wxButton* Feature_RemoveButton = nullptr;
		wxButton* Feature_MoveUpButton = nullptr;
		wxButton* Feature_MoveDownButton = nullptr;

		wxEditableListBox* EL_ToolProf = nullptr;
		wxEditableListBox* EL_LangProf = nullptr;
		wxEditableListBox* EL_Conditions = nullptr;

		std::vector<std::pair<wxTextCtrl*, wxTextCtrl*>> Stat_TextCtrls;

		std::vector<wxSpinCtrl*> moneyVals;
		std::pair<wxButton*, wxButton*> moneyButtons = { nullptr, nullptr };

		std::tuple<wxStaticText* ,wxSpinCtrl*, wxButton*> HealToX;
		std::tuple<wxStaticText*, wxSpinCtrl*> MaxHPBonus;

		std::pair<wxTextCtrl*, wxTextCtrl*> InitMod;

		std::vector<std::tuple<wxSpinCtrl*, wxStaticText*>> HitDie;

		std::pair<wxButton*, wxButton*> SlidersButtons;
		std::vector<wxTextCtrl*> SliderVal;
		std::vector<std::pair<wxStaticText* ,wxSlider*>> Sliders;
		std::vector<wxStaticLine*> sliderLine;
		//MainPagePanels();

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
		wxTextCtrl* SpellPoints_Val = nullptr;

		wxTextCtrl* spellPoints;
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

		
public:
	MainFrame(const wxString& title, const Character& pChar);

	void CreateMenuBar();

	wxScrolled<wxPanel>* CreateMainPage(wxNotebook* parent);
	wxScrolled<wxPanel>* CreateSpellsPage(wxNotebook* parent);
	wxScrolled<wxPanel>* CreateKnownSpellsPage(wxNotebook* parent);
	wxScrolled<wxPanel>* CreateSpellSlotsTable(wxNotebook* parent);
	wxScrolled<wxPanel>* CreateInveontoryPage(wxNotebook* parent);
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
	wxPanel* CreateToolProfeciencies(wxPanel* parent);
	wxPanel* CreateLangProfeciencies(wxPanel* parent);
	wxPanel* CreatePlayerConditions(wxPanel* parent);
	wxPanel* CreateMoney(wxPanel* parent);
	wxPanel* CreateSubHPPanel(wxPanel* parent);
	wxPanel* CreateInitiativePanel(wxPanel* parent);
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
	wxPanel* CreateSpellPoints(wxPanel* parent);
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
	void updateFeaturesList();
	void updateKnownSpellMods();
	void updateMoneyCtrls();
	void updateHP();
	void updatePlayerConds();

	void calcCheckedSpells();

	void DrawMain();

	void makeSavingThrowPair(wxStaticText* savingThrowName, wxTextCtrl* savingThrowValue, Skills curSkill);
	void makeSkillPair(wxStaticText* skillName, wxTextCtrl* skillValue, Skills curSkill);

	//----------------------
	//Event Handlers
	//----------------------

	void onSpellSearchType(wxCommandEvent& event);
	void onSpellSearch(wxCommandEvent& event);
	void onSpellTreeSelect(wxTreeEvent& event);
	
	void onACCheckBoxTick(wxCommandEvent& event);
	void onGiveTempHPButton(wxCommandEvent& event);
	void onSetArmorButton(wxCommandEvent& event);
	void onHealToButton(wxCommandEvent& event);
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
	void onToolProfecs(wxListEvent& event);
	void onLangProfecs(wxListEvent& event);
	void onAddRemMoney(wxCommandEvent& event);
	void onConditionListDClick(wxCommandEvent& event);

	void onSpellSplitterResize(wxSplitterEvent& event);

	void onKnownSpellsEvents(wxCommandEvent& event);
	void onKnownSpellsSelectSpell(wxCommandEvent& event);
	void onKnownSpellsAddRemSpell(wxCommandEvent& event);
	void onTickSpellKnownSpells(wxCommandEvent& event);
	void onKnownSpellsUseSpell(wxCommandEvent& event);
	void onKnownSpellsUseSpellPoint(wxCommandEvent& event);
	
	void onResetSpellPoints(wxCommandEvent& event);
	void onSetSpellPoints(wxCommandEvent& event);
	void onSetMenuEvents(wxCommandEvent& event);
	void onConditionMenuEvents(wxCommandEvent& event);

	void onTest(wxCommandEvent& event);

	void loadSpells();
	void fillLevelSpells();
	int getNumSpells(int level);
	void initConditions();
	feature getConditionDescription(const Conditions& cond);

	void setWindowColour(wxWindow* window, ColourPair colour);
};

void toLowerString(std::string& str);