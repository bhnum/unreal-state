#include "User.h"
#include "sha256.h"

using namespace std;
using std::chrono::duration_cast;
using std::chrono::seconds;

void User::set_password(const string &password)
{
	passwordhash = hash256_hex_string(password);
}

bool User::check_password(const string &password) const
{
	return passwordhash == hash256_hex_string(password);
}

ostream &operator<<(ostream &out, const User &user)
{
	out << user.id << ',' << static_cast<int>(user.type) << ','
		<< user.username << ',' << user.passwordhash << ','
		<< user.name << ',' << user.surname << ',' << user.balance << ','
		<< duration_cast<seconds>(user.birthdate.time_since_epoch()).count() << ','
		<< duration_cast<seconds>(user.lastlogintime.time_since_epoch()).count() << ','
		<< duration_cast<seconds>(user.lastlogouttime.time_since_epoch()).count() << ','
		<< user.inactive << endl;
	return out;
}

istream &operator>>(istream &in, User &user)
{
	in >> user.id;
	in.ignore();
	in >> reinterpret_cast<int &>(user.type);
	in.ignore();
	getline(in, user.username, ',');
	getline(in, user.passwordhash, ',');
	getline(in, user.name, ',');
	getline(in, user.surname, ',');
	in >> user.balance;
	in.ignore();

	long long birthdate, lastlogintime, lastlogouttime;
	in >> birthdate;
	in.ignore();
	in >> lastlogintime;
	in.ignore();
	in >> lastlogouttime;
	in.ignore();
	user.birthdate = time_point(seconds(birthdate));
	user.lastlogintime = time_point(seconds(lastlogintime));
	user.lastlogouttime = time_point(seconds(lastlogouttime));

	in >> user.inactive;
	in.ignore();
	return in;
}
