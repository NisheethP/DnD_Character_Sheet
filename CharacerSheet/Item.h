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
		ar& tags;
	}

	std::string itemName;
	std::string itemDescription;
	bool reqAttunement;
	float weight;
	int count;
	std::vector<std::string> tags;

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
	void setWeight(float wt) { weight = wt; }
	
	void setCount(int ct) { count = ct; }
	int incCount(int x);
	int decCount(int x);
	
	void addTag(std::string pTag);
	void addTag(std::vector<std::string> pTag);
	void setTag(std::vector<std::string> pTag);

	std::string getName()		{ return itemName; }
	std::string getDescription(){ return itemDescription; }
	bool getAttunement() const { return reqAttunement; }
	int getWeight() const { return weight; }
	int getTotalWeight() const { return weight * count; }
	int getCount() const { return count; }
	std::vector<std::string>& getTags() { return tags; }

	bool hasTag(std::string pTag);

};

