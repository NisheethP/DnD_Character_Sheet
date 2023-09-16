#include "PreCompiledHeader.h"

#include "Item.h"

Item::Item() :
	itemName(""),
	itemDescription(""),
	reqAttunement(false),
	weight(0)
{
}

Item::Item(std::string pName, std::string pDesc, bool pAtt, int pWt):
	itemName(pName),
	itemDescription(pDesc),
	reqAttunement(pAtt),
	weight(pWt)
{
}
