#pragma once

#include "MainWindow.h"
#include "AdminWindow.h"
#include "UserWindow.h"
#include "Manager.h"

class RegisterWindow : public MainWindow
{
	Q_OBJECT

public:
	RegisterWindow(ContractManager &conManager, QWidget *parent = Q_NULLPTR);
	~RegisterWindow();

public slots:
	void cancel_clicked();
	void register_clicked();
	void loggedout();

private:
	int lastId;
	UserManager &userManager;
	ResidenceManager &resManager;
	ContractManager &conManager;

	QFormLayout *form;
	QLineEdit *usernameEdit, *passwordEdit, *nameEdit, *surnameEdit;
	QSpinBox *balanceEdit;
	QDateEdit *birthdateEdit;
};
