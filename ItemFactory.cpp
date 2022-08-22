//---------------------------------------------------------------------
//	ItemFactory.cpp.

//---------------------------------------------------------------------
#include "ItemFactory.h"


//---------------------------------------------------------------------
//		Global static variable for ItemFactory.
//---------------------------------------------------------------------
ItemFactory* ItemFactory::ms_pItemFactory{ nullptr };


//---------------------------------------------------------------------
ItemFactory* ItemFactory::GetSingleton()
{
	if (!ms_pItemFactory)
		ms_pItemFactory = new ItemFactory();

	return ms_pItemFactory;
}


//---------------------------------------------------------------------
void
ItemFactory::destroyInstance()
{
	if (ms_pItemFactory)
	{
		delete ms_pItemFactory;
		ms_pItemFactory = nullptr;
	}

}


//---------------------------------------------------------------------
Item*
ItemFactory::createItem(ItemType eItemType)
{
	Item* tmpItem{ nullptr };

	switch (eItemType)
	{
	case IT_RECTANGLE:
		tmpItem = new RectangleItem();
		break;

	default:
		break;
	}

	return tmpItem;

}


//---------------------------------------------------------------------
ItemFactory::ItemFactory()
{
	//
}


//---------------------------------------------------------------------
ItemFactory::ItemFactory(ItemFactory&)
{
	//
}


//---------------------------------------------------------------------





