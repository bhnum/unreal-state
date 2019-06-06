#include "User.h"
#include "sha256.h"

User::User()
{

}

void User::set_password(string password)
{
	passwordhash = hash256_hex_string(password);
}

bool User::check_password(string password)
{
	return passwordhash == hash256_hex_string(password);
}

