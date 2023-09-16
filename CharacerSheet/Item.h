#pragma once

class Item
{
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& itemName;
		ar& itemDescription;
		ar& reqAttunement;
		ar& weight;
	}

	std::string itemName;
	std::string itemDescription;
	bool reqAttunement;
	int weight;

public:

	Item();
	Item(std::string pName, std::string pDesc, bool pAtt, int pWt);
};

