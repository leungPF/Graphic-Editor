//---------------------------------------------------------------------
//	IItemIterator.h.
//	
//---------------------------------------------------------------------
#ifndef _IITEMITERATOR_H__
#define _IITEMITERATOR_H__

#pragma once
#include <list>


//---------------------------------------------------------------------
template <class T>
class IIterater;

//---------------------------------------------------------------------
//	container abstract class.
//---------------------------------------------------------------------
template <class T>
class Aggregate
{
public:
	virtual ~Aggregate() {}

	virtual IIterater<T>* createIterater() = 0;
	virtual unsigned int getSize() const = 0;
	virtual std::list<Item*>::iterator getInnerIterator() = 0;
	virtual std::list<Item*>::iterator getInnerIteratorEnd() = 0;

};


//---------------------------------------------------------------------
//	iterator abstract class.
//---------------------------------------------------------------------
template <class T>
class IIterater
{
public:
	virtual ~IIterater() {}

	virtual T* first() = 0;
	virtual T* next() = 0;
	virtual bool isDone() = 0;
	virtual T* currentItem() = 0;

private:

};


//---------------------------------------------------------------------
#endif // !_IITEMITERATOR_H__





