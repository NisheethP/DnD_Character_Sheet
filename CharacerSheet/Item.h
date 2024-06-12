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
		ar& count;
	}

	std::string itemName;
	std::string itemDescription;
	bool reqAttunement;
	int weight;
	int count;

public:

	Item();
	Item(std::string pName, std::string pDesc, bool pAtt, int pWt, int pCount);
	bool operator== (Item& item)
	{
		if (itemName == item.itemName
			&& itemDescription == item.itemDescription
			&& reqAttunement == item.reqAttunement
			&& weight == item.weight
			)
			return true;

		return false;
	}

	void setItem(std::string name, std::string desc, bool att, int wt, int ct);
	void setName(std::string name) { itemName = name; }
	void setDescription(std::string desc) { itemDescription = desc; }
	void setAttunement(bool att) { reqAttunement = att; }
	void setWeight(int wt) { weight = wt; }
	void setCount(int ct) { count = ct; }
	int incCount(int x);
	int decCount(int x);

	std::string getName()		{ return itemName; }
	std::string getDescription(){ return itemDescription; }
	bool getAttunement()		{ return reqAttunement; }
	int getWeight()				{ return weight; }
	int getTotalWeight()		{ return weight * count; }
	int getCount()				{ return count; }

};

