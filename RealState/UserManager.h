#pragma once

#include "User.h"
#include <list>
#include <functional>

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
	void delete_user(long long id);

	bool authenticate_user(const string &username, const string &password);
	User *query_user(long long id);
	User *query_user(const string &username);
	User *query_user(std::function<bool(User&)> predicate);

	void update_logintime(long long id);
	void update_logouttime(long long id);

private:
	string filename;
	list<User> users;
};
