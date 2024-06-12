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

Item::Item(std::string pName, std::string pDesc, bool pAtt, int pWt, int pCount):
	itemName(pName),
	itemDescription(pDesc),
	reqAttunement(pAtt),
	weight(pWt),
	count(pCount)
{
}

void Item::setItem(std::string name, std::string desc, bool att, int wt, int ct)
{
	*this = Item(name, desc, att, wt, ct);
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
