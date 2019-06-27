#pragma once
#include <functional>
#include "Residence.h"
#include <list>
using std::list;
class ResidenceManager
{
	list<ApartmentBuilding> aparments;
	string filename;
	list<Residence*> residences;
public:
	void delete_residence(int id);
	void edit_residence(Residence&);
	void add_residence(Residence* residence);
	list<Residence*>& get_residences() {//apartment
		return residences;
	}
	Residence* query_residence(int id);//apartment
	list<Residence*> query_residence(std::function<bool (Residence*&)>);
	void delete_apartment(int id);
	void edit_apartment(ApartmentBuilding&);
	void add_apartment(ApartmentBuilding& apartment);
	list<ApartmentBuilding>& get_apartment() {
		return aparments;
	}
	Residence* query_residence(int id);
	list<Residence*> query_residence(std::function<bool(Residence*&)>);
	void save();
	void load();
	explicit ResidenceManager(const string &filename = "residences.txt");
	~ResidenceManager();
};

