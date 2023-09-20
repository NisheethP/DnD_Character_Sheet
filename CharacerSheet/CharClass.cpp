#include "PreCompiledHeader.h"

#include "CharClass.h"

std::string CharClass::getClassName()
{
	return getStringFromClass(classType);
}

DieType CharClass::getHitDie()
{
	switch (classType)
	{
	case ClassType::Barbarian:
		return DieType::d12;
	case ClassType::Bard:
		return DieType::d8;
	case ClassType::Cleric:
		return DieType::d8;
	case ClassType::Druid:
		return DieType::d8;
	case ClassType::Fighter:
		return DieType::d10;
	case ClassType::Monk:
		return DieType::d8;
	case ClassType::Paladin:
		return DieType::d10;
	case ClassType::Ranger:
		return DieType::d10;
	case ClassType::Rogue:
		return DieType::d8;
	case ClassType::Sorcerer:
		return DieType::d6;
	case ClassType::Warlock:
		return DieType::d8;
	case ClassType::Wizard:
		return DieType::d6;
	case ClassType::Artificer:
		return DieType::d8;
	default:
		return DieType::error;
	}
}

CharClass::CharClass(ClassType pClass, int pLevel, CasterType type):
	classType(pClass),
	level(pLevel),
	hitDie(getHitDie()),
	casterType(type)
{

}

bool CharClass::operator==(CharClass& c2)
{
	bool p = this->casterType == c2.casterType;
	p = p && this->classType == c2.classType;
	p = p && this->hitDie == c2.hitDie;
	
	return p;
}

std::string getStringFromClass(ClassType type)
{
	switch (type)
	{
	case ClassType::Barbarian:
		return "Barbarian";
	case ClassType::Bard:
		return "Bard";
	case ClassType::Cleric:
		return "Cleric";
	case ClassType::Druid:
		return "Druid";
	case ClassType::Fighter:
		return "Fighter";
	case ClassType::Monk:
		return "Monk";
	case ClassType::Paladin:
		return "Paladin";
	case ClassType::Ranger:
		return "Ranger";
	case ClassType::Rogue:
		return "Rogue";
	case ClassType::Sorcerer:
		return "Sorcerer";
	case ClassType::Warlock:
		return "Warlock";
	case ClassType::Wizard:
		return "Wizard";
	case ClassType::Artificer:
		return "Artificer";
	default:
		return "Error";
	}
	return "SWITCH FAIL";
}