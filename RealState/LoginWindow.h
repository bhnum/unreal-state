#pragma once

#include "RegisterWindow.h"

class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	LoginWindow(UserManager &userManager, QWidget *parent = Q_NULLPTR);

public slots:
	void login_clicked();
	void register_clicked();
	void register_closed();

private:
	UserManager &userManager;
	QFormLayout *form;
	QLineEdit *usernameEdit, *passwordEdit;
	QPushButton *loginButton, *registerButton;
};
