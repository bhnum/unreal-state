#include "Manager.h"
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::invalid_argument;
using std::runtime_error;

UserManager::UserManager(const string & filename) : ValManager<User>(filename)
{
	load();
}

void UserManager::validate(const User & user)
{
	User *duplicate = query_user(user.get_username());
	if (duplicate != nullptr && duplicate->get_id() != user.get_id())
		throw invalid_argument("Username \"" + user.get_username() + "\" already exists.");
}

void UserManager::load()
{
	ifstream inf(filename);
	if (inf.fail())
		return; // First application run

	while (true)
	{
		User user;
		inf >> user;
		if (inf.eof()) break;
		if (inf.fail())
			throw runtime_error("Error reading file \"" + filename + "\"");
		data.push_back(user);
	}
	inf.close();
}

void UserManager::save()
{
	if (filename == "") return;

	ofstream outf(filename, std::ios::trunc);
	if (outf.fail())
		throw runtime_error("Error opening file \"" + filename + "\" for writing.");

	for (auto i = data.begin(); i != data.end(); i++)
		outf << *i;
	outf.close();
}

bool UserManager::authenticate_user(const string & username, const string & password)
{
	for (auto i = data.begin(); i != data.end(); i++)
		if (i->get_username() == username)
			return !(i->get_inactive()) && i->check_password(password);
	return false;
}

User *UserManager::query_user(const string & username)
{
	for (auto i = data.begin(); i != data.end(); i++)
		if (i->get_username() == username)
			return &*i;
	return nullptr;
}

void UserManager::update_logintime(int id)
{
	User *user = query_user(id);
	if (user == nullptr)
		throw invalid_argument("User id \"" + std::to_string(id) + "\" does not exist.");
	user->set_lastlogintime(system_clock::now());
	save();
}

void UserManager::update_logouttime(int id)
{
	User *user = query_user(id);
	if (user == nullptr)
		throw invalid_argument("User id \"" + std::to_string(id) + "\" does not exist.");
	user->set_lastlogouttime(system_clock::now());
	save();
}

ResidenceManager::ResidenceManager(const string & filename) : RefManager<Residence>(filename)
{
	load();
}

void ResidenceManager::load()
{
	ifstream inf(filename);
	if (inf.fail())
		return; // First application run

	while (true)
	{
		if (inf.peek() != 'a')
			break;

		inf.ignore(2);
		ApartmentBuilding a;
		inf >> a;
		if (inf.eof()) break;
		if (inf.fail())
			throw runtime_error("Error reading file \"" + filename + "\"");
		apartments.push_back(a);
	}

	while (true)
	{
		Residence* r;
		inf >> r;
		if (r->get_type() == ResidenceType::Apartment)
		{
			int bid;
			inf >> bid;
			inf.ignore();
			ApartmentBuilding *b = query_apartment(bid);
			if (b == nullptr)
				throw runtime_error("Error! file \"" + filename + "\" is corrupted.");
			dynamic_cast<Apartment *>(r)->set_building(b);
		}
		if (inf.eof()) break;
		if (inf.fail())
			throw runtime_error("Error reading file \"" + filename + "\"");
		residences.push_back(r);
	}
	inf.close();
}

void ResidenceManager::save()
{
	if (filename == "") return;

	ofstream outf(filename, std::ios::trunc);
	if (outf.fail())
		throw runtime_error("Error opening file \"" + filename + "\" for writing.");
	for each (auto i in apartments)
	{
		outf << "a," << i << std::endl;
	}
	for each (auto i in residences)
	{
		outf << i << std::endl;
	} 
			
	outf.close();
}

ContractManager::ContractManager(ResidenceManager &resman, const string & filename)
	: RefManager<Contract>(filename), resman(resman)
{
	load();
}

void ContractManager::load()
{
	ifstream inf(filename);
	if (inf.fail())
		return; // First application run

	while (true)
	{
		Contract *c;
		inf >> c;

		int rid;
		inf >> rid;
		inf.ignore();
		Residence *r = resman.query_residence(rid);
		if (r == nullptr)
			throw runtime_error("Error! file \"" + filename + "\" is corrupted.");
		c->set_residence(r);

		if (inf.eof()) break;
		if (inf.fail())
			throw runtime_error("Error reading file \"" + filename + "\"");
		data.push_back(c);
	}
	inf.close();
}

void ContractManager::save()
{
	if (filename == "") return;

	ofstream outf(filename, std::ios::trunc);
	if (outf.fail())
		throw runtime_error("Error opening file \"" + filename + "\" for writing.");

	for (auto i = data.begin(); i != data.end(); i++)
		outf << *i << (*i)->get_residence()->get_id() << std::endl;
	outf.close();
}
