#pragma once
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "MainWindow.h"
#include "AdminWindow.h"
#include <QFormLayout>
#include <QDateEdit>
#include "User.h"
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include "Manager.h"

class UserInfoWindow : public QMainWindow
{
	Q_OBJECT

public:
	UserInfoWindow(UserManager &userManager,int id,QWidget *parent = Q_NULLPTR);
	~UserInfoWindow();
	public slots:
	void cancel_clicked();
	void done_clicked();
private:
	UserManager &userManager;
	QFormLayout *form;
	QLineEdit *usernameEdit, *passwordEdit, *nameEdit, *surnameEdit;
	QSpinBox *balanceEdit;
	QDateEdit *birthdateEdit;
	QPushButton *cancelButton, *doneButton;
	int id;
};
