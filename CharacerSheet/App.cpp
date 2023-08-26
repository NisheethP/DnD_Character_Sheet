#include "App.h"
#include "MainFrame.h"

#include <vector>
#include <sstream>
#include <fstream>

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	std::vector<CharClass> characterClass;
	characterClass.push_back(CharClass(ClassType::Sorcerer, 11, CharClass::CasterType::Full));
	//characterClass.push_back(CharClass(ClassType::Fighter, 3, CharClass::CasterType::Full));
	//characterClass.push_back(CharClass(ClassType::Wizard, 2, CharClass::CasterType::Full));
	//characterClass.push_back(CharClass(ClassType::Sorcerer, 5, CharClass::CasterType::Full));
	//characterClass.push_back(CharClass(ClassType::Artificer, 5, CharClass::CasterType::Full));
	//characterClass.push_back(CharClass(ClassType::Barbarian, 5, CharClass::CasterType::Full));
	//characterClass.push_back(CharClass(ClassType::Bard, 5, CharClass::CasterType::Full));
	//characterClass.push_back(CharClass(ClassType::Wizard, 3));

	Character Eli(
		"Eli",																					// Name
		characterClass,																			// Class and Levels
		Stats(9, 16, 16, 14, 12, 20),															// Stats
		78,																						// HP
		30,																						// Speed
		Arcana | History | Insight | Religion |													// Skill Proficiencies
		Constitution | Charisma																	// Saving Throw Proficiencies
	);

	Eli.setAC(13);
	
	int castLev = Eli.getCasterLevel();
	int x[] = {
		Eli.getSpellSlots().getNumSlots(1, castLev),
		Eli.getSpellSlots().getNumSlots(2, castLev),
		Eli.getSpellSlots().getNumSlots(3, castLev),
		Eli.getSpellSlots().getNumSlots(4, castLev),
		Eli.getSpellSlots().getNumSlots(5, castLev),
		Eli.getSpellSlots().getNumSlots(6, castLev),
		Eli.getSpellSlots().getNumSlots(7, castLev),
	};

	MainFrame* mainframe = new MainFrame(Eli.getName(), Eli);
	
	mainframe->Show(true);

	return true;
}