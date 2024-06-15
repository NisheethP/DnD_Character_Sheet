#include "PreCompiledHeader.h"

#include "Item.h"

Item::Item() :
	itemName(""),
	itemDescription(""),
	reqAttunement(false),
	weight(0),
	count(0)
{
}

Item::Item(std::string pName, std::string pDesc, bool pAtt, double pWt, int pCount):
	itemName(pName),
	itemDescription(pDesc),
	reqAttunement(pAtt),
	weight(pWt),
	count(pCount)
{
}

void Item::setItem(std::string name, std::string desc, bool att, double wt, int ct)
{
	*this = Item(name, desc, att, wt, ct);
}

void Item::addTag(std::string pTag)
{
	if (pTag == "")
		return;

	if (hasTag(pTag))
		return;

	tags.push_back(pTag);
}

void Item::addTag(std::vector<std::string> pTag)
{
	for (auto& i : pTag)
	{
		if (hasTag(i))
			continue;

		tags.push_back(i);
	}
}

void Item::setTag(std::vector<std::string> pTag)
{
	tags = pTag;
}

int Item::incCount(int x)
{
	count += x;
	return count;
}

int Item::decCount(int x)
{
	count -= x;
	if (count < 0)
		count = 0;
	
	return count;
}

bool Item::hasTag(std::string pTag)
{
	for (auto& i : tags)
	{
		if (i == pTag)
			return true;
	}

	return false;
}
