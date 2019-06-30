#pragma once

#include "MainWindow.h"
#include "Manager.h"

class UserTab : public QWidget
{
	Q_OBJECT

public:
	UserTab(UserManager &userManager, QWidget *parent = Q_NULLPTR);
	~UserTab();

public slots:
	void action_clicked();

private:
	void populate();
	UserManager &userManager;
	QLabel *countLabel;
	QStandardItemModel *model;
	QTableView *table;
};
