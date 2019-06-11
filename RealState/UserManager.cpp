#include "UserManager.h"

#include <fstream>

using std::ifstream;
using std::ofstream;
using std::exception;

UserManager::UserManager(const string &filename) : filename(filename)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	load();
}

UserManager::~UserManager()
{
	save();
}

void UserManager::load()
{
	ifstream inf(filename);
	if (inf.fail())
		return; // First application run

	inf.exceptions(ifstream::badbit);
	while (true)
	{
		User user;
		inf >> user;
		if (inf.eof()) break;
		if (inf.fail())
			throw exception(("Error reading file \"" + filename + "\"").c_str());
		users.push_back(user);
	}
	inf.close();
}

void UserManager::save()
{
	ofstream outf(filename, std::ios::trunc);
	if (outf.fail())
		throw exception(("Error opening file \"" + filename + "\" for writing.").c_str());

	for (auto i = users.begin(); i != users.end(); i++)
		outf << *i;
	outf.close();
}

list<User> &UserManager::get_users()
{
	return users;
}

void UserManager::register_user(User &user)
{
	if (query_user(user.get_username()) != nullptr)
		throw exception(("Username \"" + user.get_username() + "\" already exists.").c_str());

	// genarate unique id
	int id;
	do
	{
		id = rand();
	} while (query_user(id) != nullptr);

	user.set_id(id);
	user.set_lastlogintime(time_point());
	user.set_lastlogouttime(time_point());
	
	users.push_back(user);
	save();
}

void UserManager::edit_user(const User &user)
{
	User *olduser = query_user(user.get_id());
	if (olduser == nullptr)
		throw exception(("User id \"" + std::to_string(user.get_id()) + "\" already exists.").c_str());

	User *duplicate = query_user(user.get_username());
	if (duplicate != nullptr && duplicate->get_id() != user.get_id())
		throw exception(("Username \"" + user.get_username() + "\" already exists.").c_str());

	*olduser = user;
	save();
}

void UserManager::delete_user(int id)
{
	for (auto i = users.begin(); i != users.end(); i++)
		if (i->get_id() == id)
		{
			users.erase(i);
			return;
		}
	save();
}

bool UserManager::authenticate_user(const string &username, const string &password)
{
	for (auto i = users.begin(); i != users.end(); i++)
		if (i->get_username() == username)
			return i->check_password(password);
	return false;
}

User *UserManager::query_user(int id)
{
	for (auto i = users.begin(); i != users.end(); i++)
		if (i->get_id() == id)
			return &*i;
	return nullptr;
}

User *UserManager::query_user(const string &username)
{
	for (auto i = users.begin(); i != users.end(); i++)
		if (i->get_username() == username)
			return &*i;
	return nullptr;
}

User *UserManager::query_user(bool(*predicate)(User &))
{
	for (auto i = users.begin(); i != users.end(); i++)
		if (predicate(*i))
			return &*i;
	return nullptr;
}
