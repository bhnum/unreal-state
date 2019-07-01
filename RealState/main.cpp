#include <QtWidgets/QApplication>
#include "LoginWindow.h"
#include "ResidencesTab.h"
#include "UserInfoWindow.h"
int main(int argc, char *argv[])
{
	UserManager userManager;
	ResidenceManager resManager;
	ContractManager conManager(resManager, userManager);
	User* admin=userManager.query_user("admin");
	if (admin== nullptr)
	{
		User newadmin;
		newadmin.set_type(UserType::Admin);
		newadmin.set_username("admin");
		newadmin.set_password("admin");
		userManager.register_user(newadmin);
		admin = userManager.query_user("admin");
	}
	else if (admin->get_inactive()) {
		admin->set_inactive(false);
		userManager.save();
	}
	QApplication a(argc, argv);
	
	//LoginWindow w(userManager);
	//UserInfoWindow w(userManager,admin->get_id());
	//AdminWindow w(userManager);
	ResidencesTab w(conManager, admin->get_id());
	w.show();
	return a.exec();
}
