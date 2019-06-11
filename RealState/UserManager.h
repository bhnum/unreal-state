#pragma once

#include "User.h"
#include <list>

using std::list;

class UserManager
{
public:
	explicit UserManager(const string &filename = "users.txt");
	~UserManager();

	void load();
	void save();

	list<User> &get_users();
	void register_user(User &user);
	void edit_user(const User &user);
	void delete_user(int id);

	bool authenticate_user(const string &username, const string &password);
	User *query_user(int id);
	User *query_user(const string &username);
	User *query_user(bool(*predicate)(User &));

private:
	string filename;
	list<User> users;
};

