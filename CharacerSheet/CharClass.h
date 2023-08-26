#pragma once

#include <string>


enum class ClassType
{
	Barbarian,
	Bard,
	Cleric,
	Druid,
	Fighter,
	Monk,
	Paladin,
	Ranger,
	Rogue,
	Sorcerer,
	Warlock,
	Wizard,
	Artificer,
	none
};

enum class DieType
{
	d4,
	d6,
	d8,
	d10,
	d12,
	d20,
	d100,
	error
};

class CharClass
{
	bool usesSpellPoint = false;

public:

	enum class CasterType
	{
		None,
		Third,
		Half,
		Full
	};

	CasterType casterType = CasterType::None;
	ClassType classType;
	int level;
	DieType hitDie;

	std::string getClassName();
	DieType getHitDie();

	void setSpellPoint(bool x) { usesSpellPoint = x; }

	CharClass(ClassType pClass, int pLevel, CasterType type = CasterType::None);

	bool operator== (CharClass& c2);
};
