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
			if (&b == nullptr)
				throw std::runtime_error("Corrupted file!");
			a.set_building(&b);
		}
	}
}


ContractManager::ContractManager(ResidenceManager &resman, UserManager &userman, const string & filename)
	: RefManager<Contract>(filename), resman(resman), userman(userman)
{
	load();
}

void ContractManager::load_data(std::ifstream &inf)
{
	inf >> commissionrate;
	inf.ignore();
	RefManager<Contract>::load_data(inf);
}

void ContractManager::save_data(std::ofstream &outf)
{
	outf << commissionrate << std::endl;
	RefManager<Contract>::save_data(outf);
}

bool ContractManager::is_residence_taken(int id, bool be_verified)
{
	Residence *r = resman.query_residence(id);
	auto contracts = query_contract([resId = id, be_verified](Contract &c)
	{
		return (!be_verified || c.get_verified()) && c.get_residenceid() == resId;
	});
	if (contracts.size() != 0)
		return true;

	if (r->get_type() == ResidenceType::Apartment)
	{
		Apartment &ap = dynamic_cast<Apartment &>(*r);
		auto bcontracts = query_contract([bId = ap.get_buildingid(), be_verified](Contract &c)
		{
			return (!be_verified || c.get_verified()) && c.get_residenceid() == bId;
		});
		if (bcontracts.size() != 0)
			return true;
	}
	else if (r->get_type() == ResidenceType::ApartmentBuilding)
	{
		ApartmentBuilding &building = dynamic_cast<ApartmentBuilding &>(*r);
		auto contractedaps = resman.query_residence([this, bId = building.get_id(), be_verified](Residence &r)
		{
			if (r.get_type() == ResidenceType::Apartment)
				if (dynamic_cast<Apartment&>(r).get_buildingid() == bId)
				{
					auto contracts = query_contract([resId = r.get_id(), be_verified](Contract &c)
					{
						return (!be_verified || c.get_verified()) && c.get_residenceid() == resId;
					});
					return contracts.size() != 0;
				}
			return false;
		});
		if (contractedaps.size() != 0)
			return true;
	}
	return false;
}

void ContractManager::bind()
{
	for each (auto c in data)
	{
		Residence *r = resman.query_residence(c->get_residenceid());
		User *u = userman.query_user(c->get_holderid());
		if (r == nullptr || u == nullptr)
			throw std::runtime_error("Corrupted file!");
		c->set_residence(r);
		c->set_holder(u);
	}
}
