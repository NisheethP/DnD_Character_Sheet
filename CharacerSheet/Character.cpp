#include "Character.h"

#include <cmath>

Stats::Stats(int pStr, int pDex, int pCon, int pInt, int pWis, int pCha):
	Str(pStr),
	Dex(pDex),
	Con(pCon),
	Int(pInt),
	Wis(pWis),
	Cha(pCha)
{}

Stats::Stats():
	Str(0),
	Dex(0),
	Con(0),
	Int(0),
	Wis(0),
	Cha(0)
{
}

int Stats::getMod(Skills s)
{
	switch (s)
	{
	case none:
		return -1;
		break;
	case Acrobatics:
	case Animal_Handling:
	case Arcana:
	case Athletics:
	case Deception:
	case History:
	case Insight:
	case Intimidation:
	case Investigation:
	case Medicine:
	case Nature:
	case Perception:
	case Performance:
	case Persuasion:
	case Religion:
	case Sleight_of_Hand:
	case Stealth:
	case Survival:
		return -2;
		break;
	case Strength:
		return static_cast<int>(std::floor((Str - 10.f) * 0.5));
		break;
	case Dexterity:
		return static_cast<int>(std::floor((Dex - 10.f) * 0.5));
		break;
	case Constitution:
		return static_cast<int>(std::floor((Con - 10.f) * 0.5));
		break;
	case Intelligence:
		return static_cast<int>(std::floor((Int - 10.f) * 0.5));
		break;
	case Wisdom:
		return static_cast<int>(std::floor((Wis - 10.f) * 0.5));
		break;
	case Charisma:
		return static_cast<int>(std::floor((Cha - 10.f) * 0.5));
		break;
	default:
		return -3;
		break;
	}
	return 0;
}

Character::Character(std::string pName, std::vector<CharClass>& pCharClass, Stats pStats, int pHP,
	int pSpeed, unsigned  int pProf, unsigned  int pExp):
	characterName(pName),
	characterClassStr(""),
	totalLevel(0),
	charStats(pStats),
	skillProficiencies(pProf),
	totHP(pHP),
	totHPMod(0),
	curHP(pHP),
	speed(pSpeed),
	AC(10 + pStats.Dex),
	profBonus(2),
	tempHP(0),
	charClass(pCharClass),
	copper(0),
	silver(0),
	gold(0),
	platinum(0),
	charConditions(Conditions::NoCondition),
	charSlots(),
	initMod(0)
{
	charStatMods.Str = getStatMod(charStats.Str);
	charStatMods.Dex = getStatMod(charStats.Dex);
	charStatMods.Con = getStatMod(charStats.Con);
	charStatMods.Int = getStatMod(charStats.Int);
	charStatMods.Wis = getStatMod(charStats.Wis);
	charStatMods.Cha = getStatMod(charStats.Cha);

	characterClassStr = "";

	for (auto i = charClass.begin(); i != charClass.end(); ++i)
	{
		characterClassStr += i->getClassName();
		characterClassStr += " " + std::to_string(i->level);
		
		totalLevel += i->level;

		if (i + 1 != charClass.end())
			characterClassStr += "\n";
	}

	initSlots();

	languages.push_back("Common");

	profBonus = getProficiency(totalLevel);
}

std::string Character::getClass()
{
	std::vector<CharClass> tempClasses;
	characterClassStr = "";

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

		if(!classExists)
			tempClasses.push_back(*i);
	}

	for (auto i = tempClasses.begin(); i != tempClasses.end(); ++i)
	{
		characterClassStr += std::to_string(i->level);
		characterClassStr += " - " + i->getClassName();

		if (i + 1 != tempClasses.end())
			characterClassStr += "\n";
	}

	return characterClassStr;
}

bool Character::checkProf(const Skills& skill)
{
	return skillProficiencies & skill;
}

bool Character::checkExpert(const Skills& skill)
{
	return skillExpertises & skill;
}

void Character::initSlots()
{
	int castLevel = getCasterLevel();
	int totWarlock = 0;

	for (auto it = charClass.begin(); it != charClass.end(); ++it)
	{
		if (it->classType == ClassType::Warlock)
			totWarlock += it->level;
	}

	for (int i = 0; i <= 9; ++i)
	{
		int reg = charSlots.getNumSlots(castLevel, i);
		int war = charSlots.getNumWarlockSlots(totWarlock, i);
		
		charSlots.slots.push_back({ reg, war });
	}

	curCharSlots = charSlots;
}

void Character::calcTotLevel()
{
	totalLevel = 0;
	for (auto i = charClass.begin(); i != charClass.end(); ++i)
	{
		totalLevel += i->level;
	}
}

int Character::getSkillMod(const Skills& skill, bool isSavingThrow)
{
	int mod = getStatfromSkill(skill, charStatMods);
	
	if (checkProf(skill))
	{
		if (skill & Strength || skill & Dexterity || skill & Constitution || skill & Intelligence || skill & Wisdom || skill & Charisma)
		{
			if (isSavingThrow)
				return mod + profBonus;
			else
				return mod;
		}
	}

	if (checkExpert(skill))
		mod += 2 * profBonus;
	else if (checkProf(skill))
		mod += profBonus;
	
	return mod;
}

bool Character::checkSpellKnown(Spell& pSpell)
{
	for (auto& i : knownSpells)
	{
		if (i == pSpell)
			return true;
	}
	return false;
}

int Character::getTotalSpellPoints()
{
	int x = 0;

	for (int i = 0; i < charSlots.slots.size(); ++i)
	{
		int y;
		y = charSlots.getSlotsByType(i, SpellSlot::slotType::RegularSlot);
		y *= convSlotToPoint(i);
		x += y;
	}

	return x;
}

int Character::multiclassGetSorcererSlots(int spellLevel)
{
	std::vector<CharClass> tempClass;
	for (auto it = charClass.begin(); it != charClass.end(); ++it)
	{

	}
	Character tempChar(*this);

	return 0;
}

void Character::setStats(Stats stat)
{
	this->charStats = stat;
}

void Character::setStats(int str, int dex, int con, int pInt, int wis, int cha)
{
	charStats.Str = str;
	charStats.Dex = dex;
	charStats.Con = con;
	charStats.Int = pInt;
	charStats.Wis = wis;
	charStats.Cha = cha;
}

void Character::setCurHP(int x)
{
	if (x > totHP)
		curHP = totHP;
	else
		curHP = x;
}

void Character::giveClass(ClassType pClassType, int pLevel, CharClass::CasterType type)
{
	for (auto i = charClass.begin(); i != charClass.end(); ++i)
	{
		if (pClassType == i->classType)
		{
			i->level = pLevel;
			calcTotLevel();
			return;
		}
	}

	charClass.push_back(CharClass(pClassType, pLevel, type));
	calcTotLevel();
}

void Character::giveClass(CharClass pClass)
{
	for (auto i = charClass.begin(); i != charClass.end(); ++i)
	{
		if (pClass == *i)
		{
			i->level += pClass.level;
			calcTotLevel();

			return;
		}
	}

	charClass.push_back(pClass);
	calcTotLevel();
}

void Character::addLanguage(std::string str)
{
	languages.push_back(str);
}

void Character::addTool(std::string str)
{
	toolProf.push_back(str);
}

void Character::addSpell(Spell pSpell)
{
	if (checkSpellKnown(pSpell))
		return;
	else
	{
		knownSpells.push_back(pSpell);
	}
}

void Character::addMoney(int p, int g, int s, int c)
{
	platinum += p;
	gold += g;
	silver += s;
	copper += c;
}

void Character::addFeature(feature pFeat)
{
	features.push_back(pFeat);
}

void Character::removeFeature(feature pFeat)
{
	for (auto it = features.begin(); it != features.end(); ++it)
	{
		if (pFeat == *it)
			it = features.erase(it);
	}
}

void Character::addSlider(std::pair<std::string, int> param)
{
	sliders.push_back(param);
}

bool Character::updateSlider(std::string str, int val)
{
	for (auto it = sliders.begin(); it != sliders.end(); ++it)
	{
		if (str == it->first)
		{
			it->second = val;
			return true;
		}
	}
	return false;
}

void Character::remSlider(int index)
{
	sliders.erase(sliders.begin() + index);
}

int Character::getCasterLevel()
{
	float castLevel = 0.f, tempLevel = 0.f;
	for (auto& it : charClass)
	{
		tempLevel = 0.f;
		switch (it.casterType)
		{
		case CharClass::CasterType::None:
			tempLevel += 0.f;
			break;
		case CharClass::CasterType::Third:
			tempLevel += 0.33333f;
			break;
		case CharClass::CasterType::Half:
			tempLevel += 0.5f;
			break;
		case CharClass::CasterType::Full:
			tempLevel += 1.f;
			break;
		default:
			break;
		}
		tempLevel *= it.level;
		castLevel += tempLevel;
	}
	castLevel = std::floor(castLevel);
	return static_cast<int>(castLevel);
}

void Character::temp()
{
}

//=================================
// EXTRA FUNCTIONS
//=================================


int getStatMod(int x)
{
	float y = 0.f;
	y = static_cast<float>(x) - 10.f;
	y /= 2;
	y = std::floor(y);
	return static_cast<int>(y);
}

int getProficiency(int level)
{
	int bonus = -1;
	switch (level)
	{
	case 1:
	case 2:
	case 3:
	case 4:
		bonus = 2;
		break;
	case 5:
	case 6:
	case 7:
	case 8:
		bonus = 3;
		break;
	case 9:
	case 10:
	case 11:
	case 12:
		bonus = 4;
		break;
	case 13:
	case 14:
	case 15:
	case 16:
		bonus = 5;
		break;
	case 17:
	case 18:
	case 19:
	case 20:
		bonus = 6;
		break;
	default:
		bonus = -1;
		break;
	}
	
	return bonus;
	
}

int getStatfromSkill(const Skills& skill, const Stats& stat)
{
	switch (skill)
	{
	case Strength:
	case Athletics:
		return stat.Str;
	
	case Dexterity:
	case Acrobatics:
	case Sleight_of_Hand:
	case Stealth:
		return stat.Dex;

	case Constitution:
		return stat.Con;

	case Intelligence:
	case Arcana:
	case History:
	case Investigation:
	case Nature:
	case Religion:
		return stat.Int;

	case Wisdom:
	case Animal_Handling:
	case Insight:
	case Medicine:
	case Perception:
	case Survival:
		return stat.Wis;

	case Charisma:
	case Deception:
	case Intimidation:
	case Performance:
	case Persuasion:
		return stat.Cha;

	default:
		return -10;
		break;
	}
}

std::string getNumStr(int x)
{
	std::string str = "";
	if (x < 0)
	{
		x = -x;
		str = "-";
	}

	str += std::to_string(x);

	return str;
}

bool feature::operator==(const feature& rhs)
{
	if (title == rhs.title && description == rhs.description)
		return true;

	return false;
}