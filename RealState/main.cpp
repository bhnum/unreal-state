#include "LoginWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	UserManager userManager;
	User* admin=userManager.query_user("admin");
	if (admin== nullptr)
	{
		User admin;
		admin.set_type(UserType::Admin);
		admin.set_username("admin");
		admin.set_password("admin");
		userManager.register_user(admin);
	}
	else if (admin->get_inactive()) {
		admin->set_inactive(false);
		userManager.save();
	}
	QApplication a(argc, argv);
	//LoginWindow w(userManager);
	AdminWindow w(userManager);
	w.show();
	return a.exec();
}
