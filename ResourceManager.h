//---------------------------------------------------------------------
//	ResourceManager.h

//---------------------------------------------------------------------
#ifndef _RESOURCE_MANAGER_H__
#define _RESOURCE_MANAGER_H__

#pragma once

//---------------------------------------------------------------------
//		Headers.
//---------------------------------------------------------------------
//#include "../3rdParty/tinyxml/tinyxml.h"
//#include "../3rdParty/tinyxml/tinystr.h"
#include <QtCore/QString>
#include <QtCore/QRect>
#include <QtCore/QPoint>
#include <map>
#include <vector>
#include "IListener.h"


//---------------------------------------------------------------------
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static bool readDataFromFile(QString);
	static bool writeDataToFile(QString);
	static void registerListener(IListener::MsgType, IListener*);
	static void handleMessage(IListener::MsgType);
	static QString PicturePath();
	static QPoint graphPt();
	static void setPicturePath(QString);
	static void setGraphRect(const QRect);

private:
	static QString coordinateParser(QPoint);
	static QPoint coordinateParser2(QString);

private:
	static std::map<IListener::MsgType, std::vector<IListener*>> ms_pListenerMap;
	static QString ms_string;
	static QRect ms_graphRect;
	static QPoint ms_graphPt;

};


//---------------------------------------------------------------------
#endif // !_RESOURCE_MANAGER_H__




