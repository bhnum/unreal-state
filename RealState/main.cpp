#include "LoginWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	UserManager userManager;
	if (userManager.query_user("admin") == nullptr)
	{
		User admin;
		admin.set_type(UserType::Admin);
		admin.set_username("admin");
		admin.set_password("admin");
		userManager.register_user(admin);
	}

	QApplication a(argc, argv);
	//LoginWindow w(userManager);
	AdminWindow w;
	w.show();
	return a.exec();
}
