#pragma once

#include "MainWindow.h"
#include "AdminWindow.h"
#include "Manager.h"

class RegisterWindow : public MainWindow
{
	Q_OBJECT

public:
	RegisterWindow(UserManager &userManager, QWidget *parent = Q_NULLPTR);
	~RegisterWindow();

public slots:
	void cancel_clicked();
	void register_clicked();

private:
	UserManager &userManager;
	QFormLayout *form;
	QLineEdit *usernameEdit, *passwordEdit, *nameEdit, *surnameEdit;
	QSpinBox *balanceEdit;
	QDateEdit *birthdateEdit;
};
