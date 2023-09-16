#pragma once

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
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& usesSpellPoint;
		ar& casterType;
		ar& classType;
		ar& level;
		ar& hitDie;
	}

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

	CharClass(ClassType pClass = ClassType::none, int pLevel = 1, CasterType type = CasterType::None);

	bool operator== (CharClass& c2);
};
