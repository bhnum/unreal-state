#pragma once

#include "MainWindow.h"
#include "RegisterWindow.h"

class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	LoginWindow(UserManager &userManager, LoginInfo &info, QWidget *parent = Q_NULLPTR);

public slots:
	void login_clicked();
	void register_clicked();
	void register_closed();

private:
	int lastId;
	LoginInfo &info;
	UserManager &userManager;

	QFormLayout *form;
	QLineEdit *usernameEdit, *passwordEdit;
	QPushButton *loginButton, *registerButton;
};
