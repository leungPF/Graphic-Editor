//---------------------------------------------------------------------
//	ResourceManager.cpp

//---------------------------------------------------------------------
#include "ResourceManager.h"
//#include <QRegExp>
#include <QStringList>
#include <QRegularExpression>
#include "../3rdParty/tinyxml/tinystr.h"
#include "../3rdParty/tinyxml/tinyxml.h"
#include "ItemFactory.h"
#include "ItemManager.h"


//---------------------------------------------------------------------
//		initialize static variables.
//---------------------------------------------------------------------
QString ResourceManager::ms_string;
QRect ResourceManager::ms_graphRect;
QPoint ResourceManager::ms_graphPt;
std::map<IListener::MsgType, std::vector<IListener*>> ResourceManager::ms_pListenerMap;


//---------------------------------------------------------------------
ResourceManager::ResourceManager()
{
	//
}


//---------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
	//
}


//---------------------------------------------------------------------
bool
ResourceManager::readDataFromFile(QString strFile)
{
	//
	try
	{
		//
		TiXmlDocument* myDocument = new TiXmlDocument(strFile.toUtf8());
		myDocument->LoadFile(TIXML_ENCODING_UTF8);
		//
		TiXmlElement* RootElement = myDocument->RootElement();

		//
		if (strcmp(RootElement->Value(), "GraphicEditor") != 0)
		{
			return false;
		}

		// maybe 'Picture'
		TiXmlElement* pElement = RootElement->FirstChildElement();

		if (pElement == nullptr)
		{
			return false;
		}

		do
		{
			if (strcmp(pElement->Value(), "Picture") == 0)
			{
				TiXmlAttribute* Attribute = pElement->FirstAttribute();

				ms_string = Attribute->Value(); // path
				Attribute = Attribute->Next();
				ms_graphPt.setX(Attribute->IntValue()); // x
				Attribute = Attribute->Next();
				ms_graphPt.setY(Attribute->IntValue()); // y

				handleMessage(IListener::MT_SetPicture);

			}
			else if (strcmp(pElement->Value(), "Items") == 0)
			{
				TiXmlElement* pItemElement = pElement->FirstChildElement();

				if (!pItemElement)
				{
					return false;
				}

				// traversal all items.
				do
				{
					// fill the item's fields...
					TiXmlElement* StartPosElement = pItemElement->FirstChildElement();
					TiXmlElement* EndPosElement = StartPosElement->NextSiblingElement();
					TiXmlElement* LineStartElement = EndPosElement->NextSiblingElement();
					TiXmlAttribute* NameAttribute = pItemElement->FirstAttribute();

					// create a new item.
					Item* pItem = dynamic_cast<Item*>(ItemFactory::GetSingleton()->
						createItem(IT_RECTANGLE));

					pItem->setOriginalPos(coordinateParser2(StartPosElement->FirstChild()->Value()));
					pItem->setEndPos(coordinateParser2(EndPosElement->FirstChild()->Value()));
					pItem->setSettingLine((strcmp(LineStartElement->FirstChild()->Value(), "true")
						== 0) ? true : false);

					pItem->setText(QString::fromUtf8(NameAttribute->Value()));
					pItem->update();

					// add a new item that we just created to ItemManager.
					ItemManager::getSingleton()->addItem(pItem);

					ItemManager::getSingleton()->synchornizePropeties(true);

				} while (pItemElement = pItemElement->NextSiblingElement());


			}

		} while (pElement = pElement->NextSiblingElement());


	}
	catch (...)
	{
		return false;
	}

	return true;

}


//---------------------------------------------------------------------
bool
ResourceManager::writeDataToFile(QString strFile)
{
	//
	try
	{
		// create a XML DOM object.
		TiXmlDocument* myDocument = new TiXmlDocument();
		// create a root element and join it to document.
		TiXmlElement* RootElement = new TiXmlElement("GraphicEditor");
		myDocument->LinkEndChild(RootElement);

		// create a picture element.
		TiXmlElement* pPictureElement = new TiXmlElement("Picture");
		RootElement->LinkEndChild(pPictureElement);

		handleMessage(IListener::MT_GetPicture);

		pPictureElement->SetAttribute("Path", static_cast<const char*>
			(ms_string.toUtf8()));
		pPictureElement->SetAttribute("x", ms_graphRect.topLeft().x());
		pPictureElement->SetAttribute("y", ms_graphRect.topLeft().y());
		pPictureElement->SetAttribute("w", ms_graphRect.width());
		pPictureElement->SetAttribute("h", ms_graphRect.height());

		// create the Items element.
		TiXmlElement* pItemsElement = new TiXmlElement("Items");
		RootElement->LinkEndChild(pItemsElement);

		TiXmlElement* pItemElement{ nullptr };

		ItemManager::ItemIterater<Item*>* it =
			dynamic_cast<ItemManager::ItemIterater<Item*>*>
			(ItemManager::getSingleton()->createIterater());

		for (Item* pItem = *(it->first()); !it->isDone(); it->next())
		{
			pItem = *(it->currentItem());

			pItemElement = new TiXmlElement("Item");
			pItemElement->SetAttribute("Name", static_cast<const char*>
				(pItem->Text().toUtf8()));

			TiXmlElement* pStartPtElement = new TiXmlElement("StartPt");
			TiXmlElement* pEndPtElement = new TiXmlElement("EndPt");
			TiXmlElement* pLineStartElement = new TiXmlElement("LineStart");

			TiXmlText* pStartPtContent = new TiXmlText(static_cast<const char*>
				(coordinateParser(pItem->OriginalPos()).toUtf8()));
			TiXmlText* pEndPtContent = new TiXmlText(static_cast<const char*>
				(coordinateParser(pItem->EndPos()).toUtf8()));
			TiXmlText* pLineStartContent = new TiXmlText(pItem->settingLine() ? "true" : "false");

			pStartPtElement->LinkEndChild(pStartPtContent);
			pEndPtElement->LinkEndChild(pEndPtContent);
			pLineStartElement->LinkEndChild(pLineStartContent);

			pItemElement->LinkEndChild(pStartPtElement);
			pItemElement->LinkEndChild(pEndPtElement);
			pItemElement->LinkEndChild(pLineStartElement);

			pItemsElement->LinkEndChild(pItemElement);
		}

		// save it.
		myDocument->SaveFile(strFile.toUtf8());
	}
	catch (...)
	{
		return false;
	}

	return true;

}


//---------------------------------------------------------------------
void
ResourceManager::registerListener(
	IListener::MsgType eMessage,
	IListener* inListener)
{
	//register listener...
	ms_pListenerMap[eMessage].push_back(inListener);
}


//---------------------------------------------------------------------
void
ResourceManager::handleMessage(IListener::MsgType eMessage)
{
	//check if the type exists. 
	if (ms_pListenerMap.find(eMessage) == ms_pListenerMap.end())
		return;

	for (int i = 0; i < ms_pListenerMap[eMessage].size(); i++)
	{
		ms_pListenerMap[eMessage].at(i)->handleMessage(eMessage);
	}

}


//---------------------------------------------------------------------
QString
ResourceManager::PicturePath()
{
	return ms_string;
}


//---------------------------------------------------------------------
QPoint
ResourceManager::graphPt()
{
	return ms_graphPt;
}


//---------------------------------------------------------------------
void
ResourceManager::setPicturePath(QString strPath)
{
	ms_string = strPath;
}


//---------------------------------------------------------------------
void
ResourceManager::setGraphRect(const QRect rect)
{
	ms_graphRect = rect;
}


//---------------------------------------------------------------------
QString
ResourceManager::coordinateParser(QPoint point)
{
	return QString("(%1, %2)").arg(point.x()).arg(point.y());
}


//---------------------------------------------------------------------
QPoint
ResourceManager::coordinateParser2(QString string)
{
	// split the string into two coordinates i.e x and y ...
	QStringList list;
	//auto reg = QRegExp("[(),]");
	list = string.split(QRegularExpression("[(),]"), Qt::SplitBehaviorFlags::SkipEmptyParts);
	int num = list.size();

	return QPoint(list[0].toInt(), list[1].toInt());

}


//---------------------------------------------------------------------





