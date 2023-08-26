#include "Spells.h"

#include <cmath>

//Name;Level;Type;Casting time;Range;Components;Duration;Description;Applicable classes

Spell::Spell(std::string pName, int pLevel, spellSchool pSchool, std::string pTime, std::string pRange, spellComp pComponent, std::string pDuration, std::string pDesc, std::string pAppClass):
	Name(pName),
	level(pLevel),
	school(pSchool),
	castingTime(pTime),
	duration(pDuration),
	range(pRange),
	range_feet(0),
	components(pComponent),
	description(pDesc),
	applicableClases(pAppClass),
	matsLength(0)
{
	if (range == "Self")
		range_feet = 0;

	if (range == "Special")
		range_feet = -1;

	if (range == "Unlimited")
		range_feet = -2;

	size_t len = range.size();
	std::string tempStr = range;
	tempStr.erase(0, len - 4);
	if (tempStr == "feet")
	{
		tempStr = range;
		range_feet = std::stoi(tempStr.erase(len-4));
	}

	if (tempStr == "mile")
	{
		tempStr = range;
		range_feet = std::stoi(tempStr.erase(len - 4))*5840;
	}

	if (description[0] != '(')
		matsLength = 0;
	else
	{
		for (int i = 0; i < description.length(); i++)
		{
			matsLength = i;
			if (description[i] == ')')
				break;
		}
	}
}

int Spell::rangeSquare(int sqSize)
{
	float temp = static_cast<float>(range_feet);
	temp /= sqSize;
	temp = std::round(temp);

	return static_cast<int>(temp);
}

std::string Spell::getSpellType()
{
	std::string tempStr = "";
	if (level == 0)
	{
		tempStr = school_to_str(school) + " cantrip";
	}
	else
	{
		if (level == 1)
			tempStr = "1st";

		else if (level == 2)
			tempStr = "2nd";

		else if (level == 3)
			tempStr = "3rd";
		else
		{
			tempStr = std::to_string(level);
			tempStr += "th";
		}

		tempStr += " level ";
		tempStr += school_to_str(school);
	}	

	return tempStr;
}

std::string Spell::getDescription()
{
	std::string str = description;
	
	if (matsLength != 0)
		str = str.substr(matsLength + 1, str.length());

	return str;
}

bool Spell::operator==(const Spell& spell)
{
	if (spell.Name == Name)
		if (spell.level == level)
			if (spell.school == school)
				if (spell.castingTime == castingTime)
					if (spell.range_feet == range_feet)
						if (spell.components == components)
							if (spell.duration == duration)
								if (spell.description == description)
									if (spell.applicableClases == applicableClases)
										return true;
									
	return false;
}

std::string Spell::getMaterialsFromDesc()
{
	if (matsLength != 0)
		return description.substr(0, matsLength + 1);
	else
		return "";
}




//---------------------
//CONVERSION FUNCITONS
//---------------------	

std::string school_to_str(spellSchool spellSchool)
{
	switch (spellSchool)
	{
	case spellSchool::none:
		return "School Error: none";
		break;
	case spellSchool::Abjuration:
		return "Abjuration";
		break;
	case spellSchool::Conjuration:
		return "Conjuration";
		break;
	case spellSchool::Divination:
		return "Divination";
		break;
	case spellSchool::Dunamancy:
		return "Dunamancy";
		break;
	case spellSchool::Enchantment:
		return "Enchantment";
		break;
	case spellSchool::Evocation:
		return "Evocation";
		break;
	case spellSchool::Illusion:
		return "Illusion";
		break;
	case spellSchool::Necromancy:
		return "Necromancy";
		break;
	case spellSchool::Transmutation:
		return "Transmutation";
		break;
	default:
		return "School Error: default";
		break;
	}
}

spellSchool string_to_School(std::string schoolString, bool isCantrip)
{
	if (isCantrip)
	{
		schoolString.erase(schoolString.length()-8);
	}
	else
	{
		schoolString.erase(0, 10);
	}

	for (auto i = schoolString.begin(); i != schoolString.end(); ++i)
	{
		*i = std::tolower(*i);
	}
	

	if (schoolString == "abjuration")
		return spellSchool::Abjuration;

	else if (schoolString == "conjuration")
		return spellSchool::Conjuration;

	else if (schoolString == "divination")
		return spellSchool::Divination;

	else if (schoolString == "dunamancy")
		return spellSchool::Dunamancy;

	else if (schoolString == "enchantment")
		return spellSchool::Enchantment;

	else if (schoolString == "evocation")
		return spellSchool::Evocation;

	else if (schoolString == "illusion")
		return spellSchool::Illusion;

	else if (schoolString == "necromancy")
		return spellSchool::Necromancy;

	else if (schoolString == "transmutation")
		return spellSchool::Transmutation;

	else
		return spellSchool::none;

}

std::string comp_to_str(spellComp comp)
{
	switch (comp)
	{
	case spellComp::none:
		return "Component error: none";
	case spellComp::S:
		return "S";
	case spellComp::V:
		return "V";
	case spellComp::M:
		return "M";
	case spellComp::VS:
		return "V, S";
	case spellComp::SM:
		return "S, M";
	case spellComp::VM:
		return "V, M";
	case spellComp::VSM:
		return "V, S, M";
	default:
		return "Component error: default";
	}
}

spellComp string_to_Comp(std::string compStr)
{
	if (compStr == "S")
		return spellComp::S;

	else if (compStr == "V")
		return spellComp::V;

	else if (compStr == "V")
		return spellComp::V;

	else if (compStr == "M")
		return spellComp::M;

	else if (compStr == "V, S")
		return spellComp::VS;

	else if (compStr == "S, M")
		return spellComp::SM;

	else if (compStr == "V, M")
		return spellComp::VM;

	else if (compStr == "V, S, M")
		return spellComp::VSM;

	else
		return spellComp::none;

	return spellComp();
}