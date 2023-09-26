#include "PreCompiledHeader.h"

#include "App.h"
#include "MainFrame.h"


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

	Character character(
		"Eli",															// Name
		characterClass,													// Class and Levels
		Stats(9, 16, 16, 14, 12, 20),									// Stats
		78,																// HP
		30,																// Speed
		Arcana | History | Insight | Religion |							// Skill Proficiencies
		Constitution | Charisma,										// Saving Throw Proficiencies
		0																// Expertises
	);

	character.setAC(13);
	
	MainFrame* mainframe = new MainFrame(character.getName(), character);

	
	
	mainframe->Show(true);

	return true;
}