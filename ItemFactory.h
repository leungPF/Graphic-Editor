//---------------------------------------------------------------------
//	ItemFactory.h.

//---------------------------------------------------------------------
#ifndef _ITEMFACTORY_H__
#define _ITEMFACTORY_H__

#pragma once

//---------------------------------------------------------------------
//		Headers
//---------------------------------------------------------------------
#include "Item.h"


//---------------------------------------------------------------------
//	enumeration for Item.
//---------------------------------------------------------------------
enum ItemType
{
	IT_RECTANGLE = 0,
	IT_NUM
};


//---------------------------------------------------------------------
class ItemFactory
{
public:
	static ItemFactory* GetSingleton();
	void destroyInstance();
	Item* createItem(ItemType);

private:
	ItemFactory();
	ItemFactory(ItemFactory&);

private:
	static ItemFactory* ms_pItemFactory;

};


//---------------------------------------------------------------------
#endif // !_ITEMFACTORY_H__

//---------------------------------------------------------------------






