#pragma once

class Item
{
	std::string itemName;
	std::string itemDescription;
	bool reqAttunement;
	int weight;

public:

	Item();
	Item(std::string pName, std::string pDesc, bool pAtt, int pWt);
};

