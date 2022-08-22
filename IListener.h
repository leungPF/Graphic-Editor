//---------------------------------------------------------
//	IListener.h.

//--------------------------------------------------------- 
#ifndef _ILISTENER_H__
#define _ILISTENER_H__

#pragma once

//---------------------------------------------------------
class IListener
{
public:
	enum MsgType
	{
		MT_Update,
		MT_SetPicture,
		MT_GetPicture,
		MT_Num
	};

public:
	virtual void handleMessage(IListener::MsgType) = 0;

};


//---------------------------------------------------------
#endif //!_ILISTENER_H__





