//---------------------------------------------------------------------
//	ItemManager.cpp

//---------------------------------------------------------------------
#include "ItemManager.h"
#include "propertyeditor.h"


//---------------------------------------------------------------------
//	initialize static variables.
//---------------------------------------------------------------------
ItemManager* ItemManager::ms_pItemMgr{ nullptr };
std::map<IListener::MsgType, std::vector<IListener*>> ItemManager::ms_pListenerMap;


//---------------------------------------------------------------------
ItemManager* ItemManager::getSingleton()
{
	if (!ms_pItemMgr)
	{
		ms_pItemMgr = new ItemManager();
	}

	return ms_pItemMgr;

}


//---------------------------------------------------------------------
void
ItemManager::destroyInstance()
{
	if (ms_pItemMgr)
	{
		delete ms_pItemMgr;
		ms_pItemMgr = nullptr;
	}

}


//---------------------------------------------------------------------
bool
ItemManager::addItem(Item* pItem)
{
	m_ItemContainer.push_back(pItem);
	m_pCurrentItem = pItem;

	return true;
}


//---------------------------------------------------------------------
void
ItemManager::deleteItem(Item* pItem)
{
	if (pItem)
	{
		m_ItemContainer.remove(pItem);
		delete pItem;

		if (pItem == m_pCurrentItem)
			m_pCurrentItem = nullptr;

		pItem = nullptr;
	}

}


//---------------------------------------------------------------------
void
ItemManager::deleteAllItems()
{
	for (auto& item : m_ItemContainer)
		if (item)
			delete item;

	m_ItemContainer.clear();
	m_pCurrentItem = nullptr;

}


//---------------------------------------------------------------------
unsigned int
ItemManager::itemCount()
const
{
	return static_cast<unsigned int>(m_ItemContainer.size());
}


//---------------------------------------------------------------------
Item*
ItemManager::SelectedItem(const QPoint& point)
{
	std::list<Item*>::iterator it;

	for (it = m_ItemContainer.begin(); it != m_ItemContainer.end(); ++it)
	{
		if ((*it)->rect().contains(point))
		{
			m_pCurrentItem = *it;
			return *it;
		}
	}

	return m_pCurrentItem = nullptr;

}


//---------------------------------------------------------------------
Item*
ItemManager::currentItem()
{
	return m_pCurrentItem;
}


//---------------------------------------------------------------------
void
ItemManager::RenderItems(QWidget* pWidget)
{
	if (!pWidget)
		return;

	for (auto it = m_ItemContainer.begin(); it != m_ItemContainer.end(); ++it)
	{
		if (m_pCurrentItem)
		{
			if (m_pCurrentItem == *it)
			{
				(*it)->render(pWidget, true);
			}
			else
			{
				goto normalRender;
			}

		}
		else
		{
		normalRender:
			(*it)->render(pWidget);
		}

	}

}


//---------------------------------------------------------------------
void
ItemManager::setPropertyEditor(PropertyEditor* pPropertyEditor)
{
	m_pPropertyEditor = pPropertyEditor;
}


//---------------------------------------------------------------------
void
ItemManager::detachPropertyEditor()
{
	m_pPropertyEditor = nullptr;
}


//---------------------------------------------------------------------
void
ItemManager::synchornizePropeties(bool bType)
{
	/*	true is uploading data from ItemManager to widget,
		and false is downloading data from widget.			*/
	if (bType)
	{
		// uploading...
		if (m_pPropertyEditor)
		{
			if (m_pCurrentItem)
			{
				m_pPropertyEditor->setName(m_pCurrentItem->Text());
				m_pPropertyEditor->setStartPt(m_pCurrentItem->OriginalPos());
				m_pPropertyEditor->setEndPt(m_pCurrentItem->EndPos());
			}
			else
			{
				m_pPropertyEditor->setName("***");
				m_pPropertyEditor->setStartPt(QPoint(0, 0));
				m_pPropertyEditor->setEndPt(QPoint(0, 0));
			}


		}

	}
	else
	{
		// downloading...
		if (m_pCurrentItem && m_pPropertyEditor)
		{
			m_pCurrentItem->setText(m_pPropertyEditor->name());
		}

		update();

		// dispatch message: MT_Update.
		handleMessage(IListener::MT_Update);

	}

}


//---------------------------------------------------------------------
void
ItemManager::update()
{
	// update all items.
	for (auto it = m_ItemContainer.begin(); it != m_ItemContainer.end(); ++it)
		(*it)->update();

}


//---------------------------------------------------------------------
void
ItemManager::registerListener(
	IListener::MsgType eMessage,
	IListener* inListener)
{
	//register listener...
	ms_pListenerMap[eMessage].push_back(inListener);
}


//---------------------------------------------------------------------
void
ItemManager::handleMessage(IListener::MsgType eMessage)
{
	// check if the type exists. 
	if (ms_pListenerMap.find(eMessage) == ms_pListenerMap.end())
		return;

	for (int i = 0; i < ms_pListenerMap[eMessage].size(); i++)
	{
		ms_pListenerMap[eMessage].at(i)->handleMessage(eMessage);
	}

}


//---------------------------------------------------------------------
IIterater<Item*>*
ItemManager::createIterater()
{
	// create a new iterator.
	return new ItemIterater<Item*>(this);
}


//---------------------------------------------------------------------
unsigned int
ItemManager::getSize()
const
{
	return itemCount();
}


//---------------------------------------------------------------------
std::list<Item*>::iterator
ItemManager::getInnerIterator()
{
	return m_ItemContainer.begin();
}


//---------------------------------------------------------------------
std::list<Item*>::iterator
ItemManager::getInnerIteratorEnd()
{
	return m_ItemContainer.end();
}


//---------------------------------------------------------------------
ItemManager::ItemManager()
{
	//
}


//---------------------------------------------------------------------







