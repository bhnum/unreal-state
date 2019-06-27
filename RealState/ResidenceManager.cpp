#include "ResidenceManager.h"
#include<fstream>
#include "utility.h"
using std::ifstream;
using std::ofstream;
using std::invalid_argument;
void ResidenceManager::delete_residence(int id)
{

	for (auto i = residences.begin(); i != residences.end(); i++)
		if ((*i)->get_id() == id)
		{
			residences.erase(i);
			return;
		}
	save();
}
void ResidenceManager::edit_residence(Residence &residence)
{
	Residence *oldresidence = query_residence(residence.get_id());
	if (oldresidence == nullptr)
		throw invalid_argument("Residence id \"" + std::to_string(residence.get_id()) + "\" does not exist.");
	*oldresidence = residence;
	save();
}
void ResidenceManager::add_residence(Residence* residence)
{
	// generate unique id
	int id;
	do
	{
		id =(int )longrand(1000'00, 1'0000'00); // 6 digits
	} while (query_residence(id) != nullptr);

	residence->set_id(id);
	residences.push_back(residence);
	save();
}
Residence * ResidenceManager::query_residence(int id)
{
	for each (auto i in residences)
	{
		if (i->get_id() == id)
			return i;

	} 

	return nullptr;

	
}
list<Residence*> ResidenceManager::query_residence(std::function<bool(Residence*&)>predicate)
{
	list<Residence*> residence;
	for each (auto i in residences)
	
		if (predicate(i))
	residence.push_back(&*i);
	return residence;

}
void ResidenceManager::delete_apartment(int id)
{
}
void ResidenceManager::edit_apartment(ApartmentBuilding &)
{
}
void ResidenceManager::add_apartment(ApartmentBuilding & apartment)
{

}
void ResidenceManager::save() {
	ofstream outf(filename, std::ios::trunc);
	if (outf.fail())
		throw invalid_argument("Error opening file \"" + filename + "\" for writing.");
	for each (auto i in aparments)
	{
		outf <<"a,"<< i<<std::endl;
	}
	for each (auto i in residences)
	{

		outf <<"r,"<< i<<std::endl;
	} 
		
	outf.close();

}
void ResidenceManager::load() {
	ifstream inf(filename);
	if (inf.fail())
		return; // First application run

	inf.exceptions(ifstream::badbit);
	while (true)
	{
		char a;
		inf >> a;
		inf.ignore();
		if (a == 'r') {
			Residence* r;
			inf >> r;
			if (inf.eof()) break;
			if (inf.fail())
				throw invalid_argument("Error reading file \"" + filename + "\"");
			residences.push_back(r);
		}
		else {
			ApartmentBuilding r;
			inf >> r;
			if (inf.eof()) break;
			if (inf.fail())
				throw invalid_argument("Error reading file \"" + filename + "\"");
			aparments.push_back(r);
		}
	}
	inf.close();
}
ResidenceManager::ResidenceManager(const string &filename) {
	this->filename = filename;
	load();
}


ResidenceManager::~ResidenceManager()
{
	save();
}
