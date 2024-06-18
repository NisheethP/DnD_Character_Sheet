#pragma once

#include "CharClass.h"
#include "Item.h"
#include "Spells.h"
#include "SpellSlot.h"

class feature
{
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& title;
		ar& description;
	}
public:
	std::string title;
	std::string description;
	
	bool operator==(const feature& rhs);
	bool operator!=(const feature& rhs);
};

enum Skills
{
	none				= 1 << 0,
	Acrobatics			= 1 << 1,
	Animal_Handling		= 1 << 2,
	Arcana				= 1 << 3,
	Athletics			= 1 << 4,
	Deception			= 1 << 5,
	History				= 1 << 6,
	Insight				= 1 << 7,
	Intimidation		= 1 << 8,
	Investigation		= 1 << 9,
	Medicine			= 1 << 10,
	Nature				= 1 << 11,
	Perception			= 1 << 12,
	Performance			= 1 << 13,
	Persuasion			= 1 << 14,
	Religion			= 1 << 15,
	Sleight_of_Hand		= 1 << 16,
	Stealth				= 1 << 17,
	Survival			= 1 << 18,
	Strength			= 1 << 19,
	Dexterity			= 1 << 20,
	Constitution		= 1 << 21,
	Intelligence		= 1 << 22,
	Wisdom				= 1 << 23,
	Charisma			= 1 << 24
};

class Stats
{
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& Str;
		ar& Dex;
		ar& Con;
		ar& Int;
		ar& Wis;
		ar& Cha;
	}
public:
	int Str;
	int Dex;
	int Con;
	int Int;
	int Wis;
	int Cha;

	Stats(int pStr, int pDex, int pCon, int pInt, int pWis, int pCha);
	//Stats(Stats&);
	Stats();

	int getMod(Skills);
};

enum Conditions
{
	NoCondition			= 1 << 0,
	Blinded				= 1 << 1,
	Charmed				= 1 << 2,
	Deafened			= 1 << 3,
	Frightened			= 1 << 4,
	Graplled			= 1 << 5,
	Incapacitated		= 1 << 6,
	Invisible			= 1 << 7,
	Paralyzed			= 1 << 8,
	Petrified			= 1 << 9,
	Poisoned			= 1 << 10,
	Prone				= 1 << 11,
	Restrained			= 1 << 12,
	Stunned				= 1 << 13,
	Unconscious			= 1 << 14,
	Exhaustion			= 1 << 15,
	Fatigued			= 1 << 16
};

struct Slider
{
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& name;
		ar& value;
		ar& min;
		ar& max;
		ar& defaultValue;
	}

	std::string name;
	int value;
	int min;
	int max;
	int defaultValue;
};

class Character
{
private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version);

	unsigned int charConditions;

	std::string characterName;
	std::string characterClassStr;

	std::vector<CharClass> charClass;
	std::vector<std::pair<ClassType, int>> hitDice;

	std::vector<std::string> languages;
	std::vector<std::string> toolProf;
	std::vector<feature> features;
	std::vector<Item> inventory;

	std::vector<Slider> sliders;

	std::vector<std::string> knownSpells;
	
	int totalLevel;
	Stats charStats;
	Stats charStatMods;
	unsigned int skillProficiencies;
	unsigned int skillExpertises;

	std::map<Skills, int> SkillModifiers;

	SpellSlot charSlots;
	SpellSlot curCharSlots;
	int SpellPoints;
	int curSpellPoints;

	int AC;
	int AC_base;
	int AC_mod;
	int initMod;
	int totHP;
	int totHPMod;
	int curHP;
	int profBonus;
	int speed;

	int platinum;
	int gold;
	int silver;
	int copper;

	int tempHP;

public:
	Character(std::string pName, std::vector<CharClass>& pCharClass, Stats pStats, int pHP,
		int pSpeed, unsigned  int pProf, unsigned  int pExp = Skills::none);

	//Character();

	//-------------
	//GET FUNCTIONS
	//-------------
	std::string& getName() { return characterName; }
	std::string getClass(); 

	std::vector<CharClass>& getCharClass() { return charClass; }
	
	int& getLevel()		{ return totalLevel; }
	int& getAC()		{ return AC; }
	int& getAC_Base()	{ return AC_base; }
	int& getAC_Mod()	{ return AC_mod; }
	int& getTotHP()		{ return totHP; }
	int& getCurHP()		{ return curHP; }
	int& getProfBonus()	{ return profBonus; }
	int& getSpeed()		{ return speed; }

	unsigned int getConditions() {return charConditions;}
	int getInitiative() { return initMod + getSkillMod(Dexterity); }
	int getInitMod()	{ return initMod; }
	int getHPBonus()	{ return totHPMod; }
	int getModTotHP()	{ return totHP + totHPMod; }
	int getTempHP()		{ return tempHP; }

	Stats* getStats()			{ return &charStats; }
	Stats getStatsMods()		{ return charStatMods; }
	size_t getNumberSpells()	{ return knownSpells.size(); }

	std::vector<std::string>& getLanguages()	{ return languages; }
	std::vector<std::string>& getToolProf()		{ return toolProf; }
	std::vector<feature>& getFeatures()			{ return features; }
	std::vector<std::string>& getKnownSpells()	{ return knownSpells; }
	std::vector<Item>& getInventory()			{ return inventory; }

	SpellSlot& getSpellSlots()		{ return charSlots; }
	SpellSlot& getCurSpellSlots()	{ return curCharSlots; }
	int& getSpellPoints()			{ return SpellPoints; }
	int& getCurSpellPoints()		{ return curSpellPoints; }

	int& getPlatinum()	{ return platinum; }
	int& getGold()		{ return gold; }
	int& getSilver()	{ return silver; }
	int& getCopper()	{ return copper; }

	int getCasterLevel();
	std::vector<CharClass> getCombinedClasses();

	int getSavingThrow();
	int getSkillsProfs();
	int getExpertises() { return skillExpertises; }

	int getSkillModifier(const Skills& skill) { return SkillModifiers[skill]; }
	const std::vector<Slider>& getSliders() const { return sliders; }
	Slider getSlider(int index) { return sliders[index]; }
	Slider getSlider(std::string name);

	//-------------
	//SET FUNCTIONS
	//-------------
	void setTotHP(int x) { totHP = x; }
	void setName(std::string name) { characterName = name; }
	void setSpeed(int x) { speed = x; }
	void setStats(Stats stat);
	void setStats(int str = 10, int dex = 10, int con = 10, int pInt = 10, int wis = 10, int cha = 10);
	void setCurHP(int x);
	void addTotHP(int x) { totHP += x; }
	void setTotHPBonus(int x) { totHPMod = x; }
	void setAC(int x) { AC = x; }
	void setAC(int base, int mod) { AC_base = base; AC_mod = mod; AC = AC_base + AC_mod; }
	void giveClass(ClassType pClassType, int pLevel = 1, CharClass::CasterType type = CharClass::CasterType::None, bool updateSlots = false);
	void giveClass(CharClass pClass, bool updateSlots = false);
	void setInitMod(int x) { initMod = x; }

	void setSpellSlots(SpellSlot slots) { charSlots = slots; curCharSlots = slots; }
	void setSkillProfs(int x);
	void setSavingThrowProfs(int x);
	void setExpertises(int x) { skillExpertises = x; }
	void setSkillModifier(const Skills& skill, int modifier) { SkillModifiers[skill] = modifier; }

	void setSpellPoints(int x) { SpellPoints = x; curSpellPoints = x; }
	void setCurSpellPoints(int x) { curSpellPoints = x; }

	void addLanguage(std::string str);
	bool modifyLanguage(std::string oldTool, std::string newTool);
	bool remLanguage(std::string str);
	
	void addTool(std::string str);
	bool modifyTool(std::string oldTool, std::string newTool);
	bool remTool(std::string str);

	void addSpell(Spell pSpell);
	bool remSpell(std::string spellName);

	void setMoney(int p, int g, int s, int c);
	void addMoney(int p, int g, int s, int c);

	void addFeature(feature pFeat);
	void removeFeature(feature pFeat);
	
	void addSlider(Slider param);
	bool updateSlider(std::string str, int val);
	void remSlider(int index);

	int addCondition(int cond);
	int remCondition(int cond);

	void addInventory(Item entry);
	void removeInventory(Item entry);
	void emptyInventory();
	//-------------
	//OTHER FUNCTIONS
	//-------------
	bool checkProf(const Skills& skill);
	bool checkExpert(const Skills& skill);
	
	bool hasSpellSlots();

	void initSlots();

	void calcTotLevel();
	void updateStatMods();
	int getSkillMod(const Skills& skill, bool isSavingThrow = false);

	void giveTempHP(int x) { tempHP = x; }

	bool checkSpellKnown(Spell& pSpell);

	int getDefaultSpellPoints();

	int multiclassGetSorcererSlots(int spellLevel);

	void temp();
};

int getStatMod(int x);
int getProficiency(int level);
int getStatfromSkill(const Skills& skill, const Stats& stat);

std::vector<Skills> getSavingThrowVector();
std::vector<Skills> getSkillsVector();
std::string getSkillStr(const Skills& skill);
Skills getSkillfromStr(const std::string& str);
std::string getNumStr(int x);

std::string getDieTypeStr(DieType die);

template<class Archive>
inline void Character::serialize(Archive& ar, const unsigned int version)
{

	ar& charConditions;
	ar& characterName;
	ar& characterClassStr;

	ar& charClass;
	ar& hitDice;

	ar& languages;
	ar& toolProf;
	ar& features;
	ar& inventory;
	
	ar& knownSpells;

	ar& totalLevel;
	ar& charStats;
	ar& charStatMods;
	ar& skillProficiencies;
	ar& skillExpertises;
	
	ar& SpellPoints;
	ar& curSpellPoints;
	
	ar& AC;
	ar& AC_base;
	ar& AC_mod;
	ar& initMod;
	ar& totHP;
	ar& totHPMod;
	ar& curHP;
	ar& profBonus;
	ar& speed;
	
	ar& platinum;
	ar& gold;
	ar& silver;
	ar& copper;

	ar& tempHP;

	ar& charSlots;
	ar& curCharSlots;

	ar& sliders;

	ar& SkillModifiers;
}
