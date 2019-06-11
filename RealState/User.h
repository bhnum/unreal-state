#pragma once

#include "timeutility.h"

enum class UserType
{
	Regular, Admin
};

class User
{
public:
	User();

	void set_password(const string &password);
	bool check_password(const string &password) const;

	int get_id() const { return id; }
	void set_id(int _id) { id = _id; }

	UserType get_type() const { return type; }
	void set_type(UserType _type) { type = _type; }

	string get_username() const { return username; }
	void set_username(string _username) { username = _username; }

	string get_passwordhash() const { return passwordhash; }
	void set_passwordhash(string _passwordhash) { passwordhash = _passwordhash; }

	time_point get_lastlogintime() const { return lastlogintime; }
	void set_lastlogintime(time_point _lastlogintime) { lastlogintime = _lastlogintime; }

	time_point get_lastlogouttime() const { return lastlogouttime; }
	void set_lastlogouttime(time_point _lastlogouttime) { lastlogouttime = _lastlogouttime; }

	string get_name() const { return name; }
	void set_name(string _name) { name = _name; }

	string get_surname() const { return surname; }
	void set_surname(string _surname) { surname = _surname; }

	bool get_inactive() const { return inactive; }
	void set_inactive(bool _inactive) { inactive = _inactive; }

	time_point get_birthdate() const { return birthdate; }
	void set_birthdate(time_point _birthdate) { birthdate = _birthdate; }

	friend std::ostream &operator<<(std::ostream &out, const User &user);
	friend std::istream &operator>>(std::istream &in, User &user);

private:
	int id;
	UserType type;
	string username;
	string passwordhash;

	string name, surname;
	time_point birthdate;
	time_point lastlogintime, lastlogouttime;
	bool inactive = false;
};
