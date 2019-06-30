#include "Manager.h"

UserManager::UserManager(const string & filename) : ValManager<User>(filename)
{
	load();
}

void UserManager::validate(const User & user)
{
	User *duplicate = query_user(user.get_username());
	if (duplicate != nullptr && duplicate->get_id() != user.get_id())
		throw std::invalid_argument("Username \"" + user.get_username() + "\" already exists.");
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
		throw std::invalid_argument("User id \"" + std::to_string(id) + "\" does not exist.");
	user->set_lastlogintime(system_clock::now());
	save();
}

void UserManager::update_logouttime(int id)
{
	User *user = query_user(id);
	if (user == nullptr)
		throw std::invalid_argument("User id \"" + std::to_string(id) + "\" does not exist.");
	user->set_lastlogouttime(system_clock::now());
	save();
}

ResidenceManager::ResidenceManager(const string & filename) : RefManager<Residence>(filename)
{
	load();
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
	std::ifstream inf(filename);
	if (inf.fail())
		return; // First application run
	inf >> commissionrate;
	inf.ignore();
	inf.close();
	RefManager<Contract>::load();
}

void ContractManager::save()
{
	if (filename == "") return;

	std::ofstream outf(filename, std::ios::trunc);
	if (outf.fail())
		throw std::runtime_error("Error opening file \"" + filename + "\" for writing.");
	outf << commissionrate << std::endl;
	outf.close();
	RefManager<Contract>::save();
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
