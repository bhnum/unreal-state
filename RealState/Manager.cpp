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
		Residence* r;
		inf >> r;
		if (inf.eof()) break;
		if (inf.fail())
			throw runtime_error("Error reading file \"" + filename + "\"");
		data.push_back(r);
	}
	inf.close();

	bind();
}

void ResidenceManager::save()
{
	if (filename == "") return;

	ofstream outf(filename, std::ios::trunc);
	if (outf.fail())
		throw runtime_error("Error opening file \"" + filename + "\" for writing.");
	for each (auto i in data)
	{
		outf << i << std::endl;
	} 
			
	outf.close();
}

void ResidenceManager::bind()
{
	for each (auto r in data)
	{
		if (r->get_type() == ResidenceType::Apartment)
		{
			Apartment &a = dynamic_cast<Apartment&>(*r);
			ApartmentBuilding &b = dynamic_cast<ApartmentBuilding&>(*query_residence(a.get_buildingid()));
			a.set_building(&b);
		}
	}
}


ContractManager::ContractManager(ResidenceManager &resman, UserManager &userman, const string & filename)
	: RefManager<Contract>(filename), resman(resman), userman(userman)
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

		if (inf.eof()) break;
		if (inf.fail())
			throw runtime_error("Error reading file \"" + filename + "\"");
		data.push_back(c);
	}
	inf.close();

	bind();
}

void ContractManager::save()
{
	if (filename == "") return;

	ofstream outf(filename, std::ios::trunc);
	if (outf.fail())
		throw runtime_error("Error opening file \"" + filename + "\" for writing.");

	for (auto i = data.begin(); i != data.end(); i++)
		outf << *i;
	outf.close();
}

void ContractManager::bind()
{
	for each (auto c in data)
	{
		Residence *r = resman.query_residence(c->get_residenceid());
		User *u = userman.query_user(c->get_holderid());
		c->set_residence(r);
		c->set_holder(u);
	}
}
