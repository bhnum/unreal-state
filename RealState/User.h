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

	void set_password(string password);
	bool check_password(string password);

	int get_id() { return id; }
	void set_id(int _id) { id = _id; }

	UserType get_type() { return type; }
	void set_type(UserType _type) { type = _type; }

	string get_username() { return username; }
	void set_username(string _username) { username = _username; }

	string get_passwordhash() { return passwordhash; }
	void set_passwordhash(string _passwordhash) { passwordhash = _passwordhash; }

	time_point get_lastlogintime() { return lastlogintime; }
	void set_lastlogintime(time_point _lastlogintime) { lastlogintime = _lastlogintime; }

	time_point get_lastlogouttime() { return lastlogouttime; }
	void set_lastlogouttime(time_point _lastlogouttime) { lastlogouttime = _lastlogouttime; }
	
	string get_name() { return name; }
	void set_name(string _name) { name = _name; }
	
	string get_surname() { return surname; }
	void set_surname(string _surname) { surname = _surname; }
	
	bool get_inactive() { return inactive; }
	void set_inactive(bool _inactive) { inactive = _inactive; }
	
	time_point get_birthdate() { return birthdate; }
	void set_birthdate(time_point _birthdate) { birthdate = _birthdate; }
	
	int get_balance() { return balance; }
	void set_balance(int _balance) { balance = _balance; }

private:
	int id;
	UserType type;
	string username;
	string passwordhash;
	time_point lastlogintime, lastlogouttime;

	string name, surname;
	time_point birthdate;
	bool inactive = false;
	int balance;
};

