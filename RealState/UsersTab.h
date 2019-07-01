#pragma once

#include "MainWindow.h"
#include "Manager.h"

class UsersTab : public QWidget
{
	Q_OBJECT

public:
	UsersTab(UserManager &userManager, QWidget *parent = Q_NULLPTR);
	~UsersTab();

public slots:
	void action_clicked();
	void populate();

private:
	UserManager &userManager;
	QLabel *countLabel;
	QStandardItemModel *model;
	QTableView *table;
};
