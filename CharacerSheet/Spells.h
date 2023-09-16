#pragma once

enum class spellSchool
{
	none,
	Abjuration,
	Conjuration,
	Divination,
	Dunamancy,
	Enchantment,
	Evocation,
	Illusion,
	Necromancy,
	Transmutation
};

enum class spellComp
{
	none, S, V, M, VS, SM, VM, VSM
};

class Spell
{
	//Name;Level;Type;Casting time;Range;Components;Duration;Description;Applicable classes
	std::string Name;
	int level;
	spellSchool school;
	std::string castingTime;
	std::string range;
	int range_feet;
	spellComp components;
	std::string duration;
	std::string description;
	std::string applicableClases;

	int matsLength;

public:
	Spell(std::string pName = " ", int pLevel = 0, spellSchool pSchool = spellSchool::none, std::string pTime = " ", std::string pRange = 0,
		spellComp pComponent = spellComp::none, std::string pDuration = " ", std::string pDesc = " ", std::string pAppClass = " ");
	
	//----------------
	//OTHER FUNCITONS
	//----------------

	int rangeSquare(int sqSize = 5.f);


	//----------------
	//GET FUNCITONS
	//----------------
	std::string getName()				{ return Name; }
	int getLevel()						{ return level; }
	spellSchool getSchool()				{ return school; }
	std::string getCastingTime()		{ return castingTime; }
	std::string getRange()				{ return range; }
	int getRageFeet()					{ return range_feet; }
	spellComp getComps()				{ return components; }
	std::string getDuration()			{ return duration; }
	std::string getRawDescription()		{ return description; }
	std::string getAppClasses()			{ return applicableClases; }

	std::string getSpellType();
	std::string getDescription();


	//----------------
	//CHECK FUNCITONS
	//----------------

	bool checkName(std::string pName)	{ return Name == pName; }
	bool operator==(const Spell& spell);


	std::string getMaterialsFromDesc();
};

std::string school_to_str(spellSchool spellSchool);
spellSchool string_to_School(std::string schoolString, bool isCantrip = false);

std::string comp_to_str(spellComp comp);
spellComp string_to_Comp(std::string compStr);