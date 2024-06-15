#include "PreCompiledHeader.h"

#include "Character.h"
#include "Util.h"

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
	skillExpertises(pExp),
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
	updateStatMods();

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

	curCharSlots = charSlots;
	curSpellPoints = SpellPoints;
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
	charSlots.slots.clear();
	int castLevel = getCasterLevel();
	int totWarlock = 0;

	for (auto it = charClass.begin(); it != charClass.end(); ++it)
	{
		if (it->classType == ClassType::Warlock)
			totWarlock += it->level;
	}

	for (int i = 0; i <= 9; ++i)
	{
		int reg = charSlots.getNumSlots(i, castLevel);
		int war = charSlots.getNumWarlockSlots(i, totWarlock);
		
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

void Character::updateStatMods()
{
	charStatMods.Str = getStatMod(charStats.Str);
	charStatMods.Dex = getStatMod(charStats.Dex);
	charStatMods.Con = getStatMod(charStats.Con);
	charStatMods.Int = getStatMod(charStats.Int);
	charStatMods.Wis = getStatMod(charStats.Wis);
	charStatMods.Cha = getStatMod(charStats.Cha);

}

int Character::getSkillMod(const Skills& skill, bool isSavingThrow)
{
	int mod = getStatfromSkill(skill, charStatMods);
	
	if (checkProf(skill))
	{
		if (skill & Strength || skill & Dexterity || skill & Constitution || skill & Intelligence || skill & Wisdom || skill & Charisma)
		{
			if (isSavingThrow)
				return mod + profBonus + getSkillModifier(skill);
			else
				return mod;
		}
	}

	mod += getSkillModifier(skill);

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
		if (i == pSpell.getName())
			return true;
	}
	return false;
}

int Character::getDefaultSpellPoints()
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
	updateStatMods();
}

void Character::setStats(int str, int dex, int con, int pInt, int wis, int cha)
{
	charStats.Str = str;
	charStats.Dex = dex;
	charStats.Con = con;
	charStats.Int = pInt;
	charStats.Wis = wis;
	charStats.Cha = cha;
	updateStatMods();
}

void Character::setCurHP(int x)
{
	if (x > getModTotHP())
		curHP = getModTotHP();
	else
		curHP = x;
}

void Character::giveClass(ClassType pClassType, int pLevel, CharClass::CasterType type, bool updateSlots)
{
	CharClass tempClass(pClassType, pLevel, type);
	giveClass(tempClass, updateSlots);
}

void Character::giveClass(CharClass pClass, bool updateSlots)
{

	bool existingClass = false;
	for (auto i = charClass.begin(); i != charClass.end(); ++i)
	{
		if (pClass == *i)
		{
			i->level += pClass.level;
			existingClass = true;
			break;
		}
	}

	if (existingClass == false)
		charClass.push_back(pClass);

	calcTotLevel();

	if (updateSlots)
	{
		initSlots();
		curCharSlots = charSlots;
		SpellPoints = getDefaultSpellPoints();
		curSpellPoints = SpellPoints;
	}

	profBonus = getProficiency(totalLevel);
}

void Character::setSkillProfs(int x)
{
	int ST = 0;
	if (skillProficiencies & Strength)
		ST |= Strength;
	if (skillProficiencies & Dexterity)
		ST |= Dexterity;
	if (skillProficiencies & Constitution)
		ST |= Constitution;
	if (skillProficiencies & Intelligence)
		ST |= Intelligence;
	if (skillProficiencies & Wisdom)
		ST |= Wisdom;
	if (skillProficiencies & Charisma)
		ST |= Charisma;

	skillProficiencies &= ~ST;

	skillProficiencies = x | ST;
}

void Character::setSavingThrowProfs(int x)
{
	if (skillProficiencies & Strength)
		skillProficiencies &= ~Strength;
	if (skillProficiencies & Dexterity)
		skillProficiencies &= ~Dexterity;
	if (skillProficiencies & Constitution)
		skillProficiencies &= ~Constitution;
	if (skillProficiencies & Intelligence)
		skillProficiencies &= ~Intelligence;
	if (skillProficiencies & Wisdom)
		skillProficiencies &= ~Wisdom;
	if (skillProficiencies & Charisma)
		skillProficiencies &= ~Charisma;

	skillProficiencies |= x;
}

void Character::addLanguage(std::string str)
{
	languages.push_back(str);
}

bool Character::modifyLanguage(std::string oldTool, std::string newTool)
{
	Util::toLowerString(oldTool);

	for (auto it = languages.begin(); it != languages.end(); ++it)
	{
		auto cur = *it;
		Util::toLowerString(cur);
		if (cur == oldTool)
		{
			*it = newTool;
			return true;
		}
	}

	return false;
}

bool Character::remLanguage(std::string str)
{
	Util::toLowerString(str);

	for (auto it = languages.begin(); it != languages.end(); ++it)
	{
		auto cur = *it;
		Util::toLowerString(cur);
		if (cur == str)
		{
			it = languages.erase(it);
			return true;
		}
	}

	return false;
}

void Character::addTool(std::string str)
{
	toolProf.push_back(str);
}

bool Character::modifyTool(std::string oldTool, std::string newTool)
{
	Util::toLowerString(oldTool);

	for (auto it = toolProf.begin(); it != toolProf.end(); ++it)
	{
		auto cur = *it;
		Util::toLowerString(cur);
		if (cur == oldTool)
		{
			*it = newTool;
			return true;
		}
	}

	return false;
}

bool Character::remTool(std::string str)
{
	Util::toLowerString(str);
	for (auto it = toolProf.begin(); it != toolProf.end(); ++it)
	{
		auto cur = *it;
		Util::toLowerString(cur);
		if (cur == str)
		{
			toolProf.erase(it);
			return true;
		}
	}

	return false;
}

void Character::addSpell(Spell pSpell)
{
	if (checkSpellKnown(pSpell))
		return;
	else
	{
		knownSpells.push_back(pSpell.getName());
	}
}

bool Character::remSpell(std::string spellName)
{
	Util::toLowerString(spellName);
	for (auto it = knownSpells.begin(); it != knownSpells.end(); ++it)
	{
		if (*it == spellName)
		{
			knownSpells.erase(it);
			return true;
		}
	}

	return false;
}

void Character::setMoney(int p, int g, int s, int c)
{
	platinum = p;
	gold = g;
	silver = s;
	copper = c;
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

void Character::addSlider(Slider param)
{
	sliders.push_back(param);
}

bool Character::updateSlider(std::string str, int val)
{
	for (auto it = sliders.begin(); it != sliders.end(); ++it)
	{
		if (str == it->name)
		{
			it->value = val;
			return true;
		}
	}
	return false;
}

void Character::remSlider(int index)
{
	sliders.erase(sliders.begin() + index);
}

/// <summary>
/// Adds condition. Use only single condition for ease
/// </summary>
/// <param name="cond"></param>
/// <returns>
///  0 : Successful allocation |
/// -1 : cond had NoCondition |
/// -2 : cond overlap
/// </returns>
int Character::addCondition(int cond)
{
	if (cond & Conditions::NoCondition)
	{
		charConditions = Conditions::NoCondition;
		return -1;
	}

	if (charConditions & cond)
		return -2;
	
	if (charConditions == Conditions::NoCondition)
	{
		charConditions = cond;
		return 0;
	}
	
	charConditions |= cond;
	return 0;
}

/// <summary>
/// Removes Conditions. Use only single condition for ease
/// </summary>
/// <param name="cond"></param>
/// <returns>
/// -1 : cond has NoCondition |
/// -2 : cond not part of charConditon |
/// 0 : cond removed successfully
/// </returns>
int Character::remCondition(int cond)
{
	if (cond & Conditions::NoCondition)
		return -1;

	if (!(charConditions & cond))
		return -2;

	charConditions = charConditions & ~cond;
	if (charConditions == 0)
		charConditions = Conditions::NoCondition;
	return 0;
}

void Character::addInventory(Item entry)
{
	for (auto it = inventory.begin(); it != inventory.end(); ++it)
	{
		if (entry == *it)
			return;
	}

	inventory.push_back(entry);
}

void Character::removeInventory(Item entry)
{
	for (auto it = inventory.begin(); it != inventory.end(); ++it)
	{
		if (entry == *it)
			it = inventory.erase(it);
	}
}

void Character::emptyInventory()
{
	inventory.clear();
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

std::vector<CharClass> Character::getCombinedClasses()
{
	std::vector<CharClass> tempClasses;

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

		if (!classExists)
			tempClasses.push_back(*i);
	}

	return tempClasses;
}

int Character::getSavingThrow()
{
	int ST = 0;
	if (skillProficiencies & Strength)
		ST |= Strength;
	if (skillProficiencies & Dexterity)
		ST |= Dexterity;
	if (skillProficiencies & Constitution)
		ST |= Constitution;
	if (skillProficiencies & Intelligence)
		ST |= Intelligence;
	if (skillProficiencies & Wisdom)
		ST |= Wisdom;
	if (skillProficiencies & Charisma)
		ST |= Charisma;

	return ST;
}

int Character::getSkillsProfs()
{
	int ST = 0;
	if (skillProficiencies & Strength)
		ST |= Strength;
	if (skillProficiencies & Dexterity)
		ST |= Dexterity;
	if (skillProficiencies & Constitution)
		ST |= Constitution;
	if (skillProficiencies & Intelligence)
		ST |= Intelligence;
	if (skillProficiencies & Wisdom)
		ST |= Wisdom;
	if (skillProficiencies & Charisma)
		ST |= Charisma;

	return skillProficiencies & ~ST;
}

Slider Character::getSlider(std::string name)
{
	for (auto& curSlider : sliders)
	{
		if (Util::toLowerStringRet(curSlider.name) == Util::toLowerStringRet(name))
			return curSlider;
	}
	
	return { "", 0,0,0,0 };
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

std::vector<Skills> getSavingThrowVector()
{
	static std::vector<Skills> SavingThrows =
	{
		Skills::Strength, 
		Skills::Dexterity, 
		Skills::Constitution, 
		Skills::Intelligence, 
		Skills::Wisdom, 
		Skills::Charisma
	};

	return SavingThrows;
}

std::vector<Skills> getSkillsVector()
{
	static std::vector<Skills> Skill =
	{
		Skills::Acrobatics,
		Skills::Animal_Handling,
		Skills::Arcana,
		Skills::Athletics,
		Skills::Deception,
		Skills::History,
		Skills::Insight,
		Skills::Intimidation,
		Skills::Investigation,
		Skills::Medicine,
		Skills::Nature,
		Skills::Perception,
		Skills::Performance,
		Skills::Persuasion,
		Skills::Religion,
		Skills::Sleight_of_Hand,
		Skills::Stealth,
		Skills::Survival
	};
	return Skill;
}

std::string getSkillStr(const Skills& skill)
{
	switch (skill)
	{
	case none:
		return "Skill: none\n";
	case Acrobatics:
		return "Acrobatics\n";
	case Animal_Handling:
		return "Animal Handling\n";
	case Arcana:
		return "Arcana\n";
	case Athletics:
		return "Athletics\n";
	case Deception:
		return "Deception\n";
	case History:
		return "History\n";
	case Insight:
		return "Insight\n";
	case Intimidation:
		return "Intimidation\n";
	case Investigation:
		return "Investigation\n";
	case Medicine:
		return "Medicine\n";
	case Nature:
		return "Nature\n";
	case Perception:
		return "Perception\n";
	case Performance:
		return "Performance\n";
	case Persuasion:
		return "Persuasion\n";
	case Religion:
		return "Religion\n";
	case Sleight_of_Hand:
		return "Sleight of Hand\n";
	case Stealth:
		return "Stealth\n";
	case Survival:
		return "Survival\n";
	case Strength:
		return "Strength\n";
	case Dexterity:
		return "Dexterity\n";
	case Constitution:
		return "Constitution\n";
	case Intelligence:
		return "Intelligence\n";
	case Wisdom:
		return "Wisdom\n";
	case Charisma:
		return "Charisma\n";
	default:
		return "Error\n";
	}
}

Skills getSkillfromStr(const std::string& str)
{
	std::string lstr = Util::toLowerStringRet(str);
	if (lstr == "acrobatics")
		return Skills::Acrobatics;

	if (lstr == "animal handling")
		return Skills::Animal_Handling;

	if (lstr == "arcana")
		return Skills::Arcana;

	if (lstr == "athletics")
		return Skills::Athletics;

	if (lstr == "charisma")
		return Skills::Charisma;

	if (lstr == "constitution")
		return Skills::Constitution;

	if (lstr == "deception")
		return Skills::Deception;

	if (lstr == "dexterity")
		return Skills::Dexterity;

	if (lstr == "history")
		return Skills::History;

	if (lstr == "insight")
		return Skills::Insight;

	if (lstr == "intelligence")
		return Skills::Intelligence;

	if (lstr == "intimidation")
		return Skills::Intimidation;

	if (lstr == "investigation")
		return Skills::Investigation;

	if (lstr == "medicine")
		return Skills::Medicine;
	
	if (lstr == "nature")
		return Skills::Nature;

	if (lstr == "perception")
		return Skills::Perception;

	if (lstr == "performance")
		return Skills::Performance;

	if (lstr == "persuasion")
		return Skills::Persuasion;

	if (lstr == "religion")
		return Skills::Religion;

	if (lstr == "sleight of hand")
		return Skills::Sleight_of_Hand;

	if (lstr == "stealth")
		return Skills::Stealth;

	if (lstr == "strength")
		return Skills::Strength;

	if (lstr == "survival")
		return Skills::Survival;

	if (lstr == "wisdom")
		return Skills::Wisdom;

	return Skills::none;
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

std::string getDieTypeStr(DieType die)
{
	switch (die)
	{
	case DieType::d4:
		return "d4";
	case DieType::d6:
		return "d6";
	case DieType::d8:
		return "d8";
	case DieType::d10:
		return "d10";
	case DieType::d12:
		return "d12";
	case DieType::d20:
		return "d20";
	case DieType::d100:
		return "d100";
	case DieType::error:
		return "Die:ERROR";
	default:
		return "Die:EMPTY";
		break;
	}
	return std::string();
}

bool feature::operator==(const feature& rhs)
{
	if (title == rhs.title && description == rhs.description)
		return true;

	return false;
}

bool feature::operator!=(const feature& rhs)
{
	return !(*this == rhs);
}
