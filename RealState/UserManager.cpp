#include "UserManager.h"

#include <fstream>

using std::ifstream;
using std::ofstream;
using std::invalid_argument;

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
			throw invalid_argument("Error reading file \"" + filename + "\"");
		users.push_back(user);
	}
	inf.close();
}

void UserManager::save()
{
	ofstream outf(filename, std::ios::trunc);
	if (outf.fail())
		throw invalid_argument("Error opening file \"" + filename + "\" for writing.");

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
		throw invalid_argument("Username \"" + user.get_username() + "\" already exists.");

	// genarate unique id
	long long id;
	do
	{
		id = longrand(1000'0000, 1'0000'0000); // 8 digits
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
		throw invalid_argument("User id \"" + std::to_string(user.get_id()) + "\" does not exist.");

	User *duplicate = query_user(user.get_username());
	if (duplicate != nullptr && duplicate->get_id() != user.get_id())
		throw invalid_argument("Username \"" + user.get_username() + "\" already exists.");

	*olduser = user;
	save();
}

void UserManager::delete_user(long long id)
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

User *UserManager::query_user(long long id)
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

User *UserManager::query_user(std::function<bool(User&)> predicate)
{
	for (auto i = users.begin(); i != users.end(); i++)
		if (predicate(*i))
			return &*i;
	return nullptr;
}

void UserManager::update_logintime(long long id)
{
	User *user = query_user(id);
	if (user == nullptr)
		throw invalid_argument("User id \"" + std::to_string(id) + "\" does not exist.");
	user->set_lastlogintime(system_clock::now());
	save();
}

void UserManager::update_logouttime(long long id)
{
	User *user = query_user(id);
	if (user == nullptr)
		throw invalid_argument("User id \"" + std::to_string(id) + "\" does not exist.");
	user->set_lastlogouttime(system_clock::now());
	save();
}
