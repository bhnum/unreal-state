#pragma once

#include "RegisterWindow.h"

class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	LoginWindow(ContractManager &conManager, QWidget *parent = Q_NULLPTR);

public slots:
	void login_clicked();
	void loggedout();
	void register_clicked();
	void unhide();

private:
	int lastId;
	UserManager &userManager;
	ResidenceManager &resManager;
	ContractManager &conManager;

	QFormLayout *form;
	QLineEdit *usernameEdit, *passwordEdit;
	QPushButton *loginButton, *registerButton;
};
