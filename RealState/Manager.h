#pragma once

#include "Residence.h"
#include <string>
#include <list>

using std::string;
using std::list;

template <typename T>
class Manager
{
public:
	explicit Manager(const string &filename = "") : filename(filename)
	{
		load(filename);
	}

	virtual ~Manager() {}

	virtual void load(const string &filename) = 0;
	virtual void save(const string &filename) = 0;
	virtual void validate(T &t) {}

protected:
	string filename;
};


template <typename T>
class ValManager : public Manager<T>
{
public:


protected:
	list<T> data;
};

template <typename T>
class RefManager : Manager<T>
{
public:


protected:
	list<T*> data;
};

class ResidenceManager : public RefManager<Residence>, public ValManager<ApartmentBuilding>
{
public:

private:


};
