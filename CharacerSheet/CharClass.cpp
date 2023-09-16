#include "PreCompiledHeader.h"

#include "CharClass.h"

std::string CharClass::getClassName()
{
	switch (classType)
	{
	case ClassType::Barbarian:
		return "Barbarian";
		break;
	case ClassType::Bard:
		return "Bard";
		break;
	case ClassType::Cleric:
		return "Cleric";
		break;
	case ClassType::Druid:
		return "Druid";
		break;
	case ClassType::Fighter:
		return "Fighter";
		break;
	case ClassType::Monk:
		return "Monk";
		break;
	case ClassType::Paladin:
		return "Paladin";
		break;
	case ClassType::Ranger:
		return "Ranger";
		break;
	case ClassType::Rogue:
		return "Rogue";
		break;
	case ClassType::Sorcerer:
		return "Sorcerer";
		break;
	case ClassType::Warlock:
		return "Warlock";
		break;
	case ClassType::Wizard:
		return "Wizard";
		break;
	case ClassType::Artificer:
		return "Artificer";
		break;
	default:
		return "Error";
		break;
	}
	return std::string();
}

DieType CharClass::getHitDie()
{
	switch (classType)
	{
	case ClassType::Barbarian:
		return DieType::d12;
		break;
	case ClassType::Bard:
		return DieType::d8;
		break;
	case ClassType::Cleric:
		return DieType::d8;
		break;
	case ClassType::Druid:
		return DieType::d8;
		break;
	case ClassType::Fighter:
		return DieType::d10;
		break;
	case ClassType::Monk:
		return DieType::d8;
		break;
	case ClassType::Paladin:
		return DieType::d10;
		break;
	case ClassType::Ranger:
		return DieType::d10;
		break;
	case ClassType::Rogue:
		return DieType::d8;
		break;
	case ClassType::Sorcerer:
		return DieType::d6;
		break;
	case ClassType::Warlock:
		return DieType::d8;
		break;
	case ClassType::Wizard:
		return DieType::d6;
		break;
	case ClassType::Artificer:
		return DieType::d8;
		break;
	default:
		return DieType::error;
		break;
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
