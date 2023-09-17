#include "PreCompiledHeader.h"
#include "SpellSlot.h"

SpellSlot::vector_int_2d SpellSlot::AllSlots;
std::vector<std::pair <int, int>> SpellSlot::AllWarlockSlots;



SpellSlot::SpellSlot()
{
	initAllSlots();
	initAllWarlock();
}

void SpellSlot::initAllSlots()
{
	for (int i = 0; i < 20; ++i)
	{
		AllSlots.push_back(std::vector<int>(9, -1));
		//slots[i][j] = -1;
	}

	//LVL 1
	AllSlots[0][0] = 2;
	AllSlots[1][0] = 3;
	for (int i = 2; i < 20; ++i)
		AllSlots[i][0] = 4;

	//LVL 2
	AllSlots[2][1] = 2;
	for (int i = 3; i < 20; ++i)
		AllSlots[i][1] = 3;

	//LVL 3
	AllSlots[4][2] = 2;
	for (int i = 5; i < 20; ++i)
		AllSlots[i][2] = 3;

	//LVL 4
	AllSlots[6][3] = 1;
	AllSlots[7][3] = 2;
	for (int i = 8; i < 20; ++i)
		AllSlots[i][3] = 3;

	//LVL 5
	AllSlots[8][4] = 1;
	for (int i = 9; i < 20; ++i)
		AllSlots[i][4] = 2;

	//LVL 6
	AllSlots[18][5] = 2;
	AllSlots[19][5] = 2;
	for (int i = 10; i < 18; ++i)
		AllSlots[i][5] = 1;

	//LVL 7
	AllSlots[19][6] = 2;
	for (int i = 12; i < 19; ++i)
		AllSlots[i][6] = 1;

	//LVL 8
	for (int i = 14; i < 20; ++i)
		AllSlots[i][7] = 1;

	//LVL 9
	for (int i = 16; i < 20; ++i)
		AllSlots[i][8] = 1;
}

void SpellSlot::initAllWarlock()
{
	AllWarlockSlots.push_back({1,1});	//1
	AllWarlockSlots.push_back({2,1});	//2
	AllWarlockSlots.push_back({2,2});	//3
	AllWarlockSlots.push_back({2,2});	//4
	AllWarlockSlots.push_back({2,3});	//5
	AllWarlockSlots.push_back({2,3});	//6
	AllWarlockSlots.push_back({2,4});	//7
	AllWarlockSlots.push_back({2,4});	//8
	AllWarlockSlots.push_back({2,5});	//9
	AllWarlockSlots.push_back({2,5});	//10
	AllWarlockSlots.push_back({3,5});	//11
	AllWarlockSlots.push_back({3,5});	//12
	AllWarlockSlots.push_back({3,5});	//13
	AllWarlockSlots.push_back({3,5});	//14
	AllWarlockSlots.push_back({3,5});	//15
	AllWarlockSlots.push_back({3,5});	//16
	AllWarlockSlots.push_back({4,5});	//17
	AllWarlockSlots.push_back({4,5});	//18
	AllWarlockSlots.push_back({4,5});	//19
	AllWarlockSlots.push_back({4,5});	//20
}

int SpellSlot::getNumSlots(int spell_Level, int char_Level)
{
	if (spell_Level == 0)
		return -4;

	if (spell_Level < 1 || spell_Level > 9)
		return -2;
	
	if (char_Level == 0)
		return -1;

	if (char_Level < 1 || char_Level > 20)
		return -3;

	return AllSlots[char_Level-1][spell_Level-1];
}

int SpellSlot::getNumWarlockSlots(int spellLevel, int charLevel)
{
	if (spellLevel == 0)
		return -4;

	if (spellLevel < 1 || spellLevel > 9)
		return -2;

	if (charLevel == 0)
		return -1;

	if (charLevel < 1 || charLevel > 20)
		return -3;

	int num = -1;
	
	if (spellLevel == AllWarlockSlots[charLevel-1].second)
		num = AllWarlockSlots[charLevel-1].first;
	
	return num;
}

int SpellSlot::getSlotsByType(int level, slotType type)
{
	if (level < 0 || level > slots.size())
		return -2;
	else
	{
		switch (type)
		{
		case SpellSlot::slotType::NoSlot:
			return 0;
		case SpellSlot::slotType::Cantrip:
			return -4;
		case SpellSlot::slotType::RegularSlot:
			return slots[level].first;
		case SpellSlot::slotType::WarlockSlot:
			return slots[level].second;
		default:
			return -5;
		}
	}
}

int convSlotToPoint(int spell_Level)
{
	switch (spell_Level)
	{
	case 0:
		return 0;
	case 1:
		return 2;
	case 2:
		return 3;
	case 3:
		return 5;
	case 4:
		return 6;
	case 5:
		return 7;
	case 6:
		return 9;
	case 7:
		return 10;
	case 8:
		return 11;
	case 9:
		return 13;
	default:
		return -1;
	}

	return -2;
}

int convPointToSlot(int points)
{
	switch (points)
	{
	case 2:
		return 1;
	case 3:
		return 2;
	case 5:
		return 3;
	case 6:
		return 4;
	case 7:
		return 5;
	case 9:
		return 6;
	case 10:
		return 7;
	case 11:
		return 8;
	case 13:
		return 9;

	default:
		return -1;
	}

	return -2;
}