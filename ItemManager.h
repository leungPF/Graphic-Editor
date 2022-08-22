//---------------------------------------------------------------------
//	ItemManager.h

//---------------------------------------------------------------------
#ifndef _ITEMMANAGER_H__
#define _ITEMMANAGER_H__

#pragma once


//---------------------------------------------------------------------
//		Headers.
//---------------------------------------------------------------------
#include "Item.h"
#include <list>
//#include "PropertyEditor.h"
#include "IListener.h"
#include "IItemIterator.h"

//---------------------------------------------------------------------
//	incomplete types.
//---------------------------------------------------------------------
class PropertyEditor;


//---------------------------------------------------------------------	
class ItemManager : public Aggregate<Item*>, public IListener
{
public:
	static ItemManager* getSingleton();
	void destroyInstance();

	bool addItem(Item* pItem);
	void deleteItem(Item* pItem);
	void deleteAllItems();
	unsigned int itemCount() const;
	Item* SelectedItem(const QPoint&);
	Item* currentItem();
	void RenderItems(QWidget*);
	void setPropertyEditor(PropertyEditor*);
	void detachPropertyEditor();
	void synchornizePropeties(bool);
	void update();
	void registerListener(IListener::MsgType, IListener*);

	//---------------------------------------------------------------------
	// implementation of IListener.
	//---------------------------------------------------------------------
	void handleMessage(IListener::MsgType) override;

	//---------------------------------------------------------------------
	// implementation of Aggregate.
	//---------------------------------------------------------------------
	IIterater<Item*>* createIterater() override;
	unsigned int getSize() const override;
	std::list<Item*>::iterator getInnerIterator() override;
	std::list<Item*>::iterator getInnerIteratorEnd() override;

	//---------------------------------------------------------------------
private:
	ItemManager();
	ItemManager(ItemManager&);
	static ItemManager* ms_pItemMgr;
	Item* m_pCurrentItem{ nullptr };
	PropertyEditor* m_pPropertyEditor{ nullptr };

private:
	std::list<Item*> m_ItemContainer;
	static std::map<IListener::MsgType, std::vector<IListener*>> ms_pListenerMap;

public:

	//---------------------------------------------------------------------
	//		Inner class: ItemIterator.
	//---------------------------------------------------------------------
	template<class T>
	class ItemIterater : public IIterater<T>
	{
	public:
		ItemIterater(Aggregate*);
		virtual ~ItemIterater() {}

		T* first();
		T* next();
		bool isDone();
		T* currentItem();

	private:
		Aggregate* m_pConcreateAggregate;
		std::list<Item*>::iterator m_pIterator;
		std::list<Item*>::iterator m_pIteratorEnd;

	};

	//---------------------------------------------------------------------

};


//---------------------------------------------------------------------
//	implementations of template class ItemIterator<class T>.
//---------------------------------------------------------------------
template<class T>
T* ItemManager::ItemIterater<T>::first()
{
	m_pIterator = /*reinterpret_cast<std::list<Item*>::iterator*>
		(*/m_pConcreateAggregate->getInnerIterator();
	m_pIteratorEnd = /*reinterpret_cast<std::list<Item*>::iterator*>
		(*/m_pConcreateAggregate->getInnerIteratorEnd();

	return &(*m_pIterator);

}

//---------------------------------------------------------------------
template<class T>
inline T* ItemManager::ItemIterater<T>::next()
{
	//

	return &(*m_pIterator++);


}

//---------------------------------------------------------------------
template<class T>
inline T* ItemManager::ItemIterater<T>::currentItem()
{

	return &*m_pIterator;

}

//---------------------------------------------------------------------
template<class T>
inline bool ItemManager::ItemIterater<T>::isDone()
{

	if (m_pIterator == m_pIteratorEnd)
	{
		return true;
	}


	return false;
}

//---------------------------------------------------------------------
template<class T>
ItemManager::ItemIterater<T>::ItemIterater(Aggregate* pAggregate)
	: m_pConcreateAggregate(pAggregate)
{
	//none.
}


//---------------------------------------------------------------------
#endif

//---------------------------------------------------------------------

