#pragma once

#include "Manager.h"
#include "MainWindow.h"

struct LoginInfo
{
	bool exited = true;
	User *user = nullptr;
};

class RegisterWindow : public MainWindow
{
	Q_OBJECT

public:
	RegisterWindow(UserManager &userManager, LoginInfo &info, QWidget *parent = Q_NULLPTR);
	~RegisterWindow();

public slots:
	void cancel_clicked();
	void register_clicked();

private:
	int lastId;
	LoginInfo &info;
	UserManager &userManager;

	QFormLayout *form;
	QLineEdit *usernameEdit, *passwordEdit, *nameEdit, *surnameEdit;
	QSpinBox *balanceEdit;
	QDateEdit *birthdateEdit;
};
