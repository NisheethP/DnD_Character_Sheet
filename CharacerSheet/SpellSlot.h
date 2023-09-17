#pragma once

#include "CharClass.h"

///=================
///RETURN CODES
/// -1 = NO SLOTS
/// -2 = SLOT LEVEL OUT OF RANGE
/// -3 = CHAR LEVEL OUT OF RANGE
/// -4 = INF
/// -5 = OTHER ERROR
///=================

class SpellSlot
{
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		/*try
		{
			ar& slots;
		}
		catch (const boost::archive::archive_exception& x)
		{
			x.code;
			wxMessageBox("Slots");
		}
		*/
		ar& AllWarlockSlots;
		ar& AllSlots;
	}

	using vector_int_2d = std::vector<std::vector<int>>;
	
	///[CHAR LEVEL] <NUMBER,LEVEL>
	std::vector<std::pair <int, int>> AllWarlockSlots;
	
	/// [CHAR LEVEL][SPELL LEVEL]
	vector_int_2d AllSlots;

public:
	enum class slotType
	{
		NoSlot,
		Cantrip,
		RegularSlot,
		WarlockSlot
	};
	
	/// [SLOT LEVEL]<REGULAR  WARLOCK>
	std::vector<std::pair<int, int>> slots;

	SpellSlot();

	void initAllSlots();
	void initAllWarlock();
	int getNumSlots(int spell_Level, int char_Level);
	int getNumWarlockSlots(int spellLevel, int charLevel);

	int getSlotsByType(int level, slotType type);
};

int convSlotToPoint(int spell_Level);
int convPointToSlot(int points);

