#pragma once

#include "MainWindow.h"
#include "UserInfoWindow.h"
#include "UsersTab.h"
#include "Manager.h"

class AdminWindow : public MainWindow
{
	Q_OBJECT

public:
	AdminWindow(UserManager& userManager, int userId, QWidget *parent = Q_NULLPTR);
	~AdminWindow();

private:
	QTabWidget *tabs;
	QTableView *usersTable, *residencesTable, *contractsTable;
	int userId;
	UserManager& userManager;
};
