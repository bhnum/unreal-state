#pragma once

#include "MainWindow.h"
#include "Manager.h"

class UserInfoWindow : public QMainWindow
{
	Q_OBJECT

public:
	UserInfoWindow(UserManager &userManager, int id, QWidget *parent = Q_NULLPTR);
	~UserInfoWindow();
	public slots:
	void cancel_clicked();
	void done_clicked();
private:
	int id;
	UserManager &userManager;
	QFormLayout *form;
	QLineEdit *usernameEdit, *passwordEdit, *nameEdit, *surnameEdit;
	QSpinBox *balanceEdit;
	QDateEdit *birthdateEdit;
	QPushButton *cancelButton, *doneButton;
};
