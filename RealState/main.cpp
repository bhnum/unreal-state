#include <QtWidgets/QApplication>
#include "LoginWindow.h"
#include "AdminWindow.h"
#include "UserWindow.h"

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
	while (true)
	{
		LoginInfo login = LoginInfo();
		LoginWindow loginWindow(userManager, login);
		loginWindow.show();
		a.exec();

		if (login.exited)
			break;

		MainWindow *wnd;
		if (login.user->get_type() == UserType::Admin)
		{
			wnd = new AdminWindow(conManager, login.user->get_id());
		}
		else
		{
			wnd = new UserWindow(conManager, login.user->get_id());
		}
		wnd->setWindowModality(Qt::ApplicationModal);
		wnd->show();
		userManager.update_logintime(login.user->get_id());
		a.exec();
		userManager.update_logouttime(login.user->get_id());
	}

	return 0;
}
