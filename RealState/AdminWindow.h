#pragma once

#include "MainWindow.h"
#include "UsermanWindow.h"
#include "UserInfoWindow.h"
#include "Manager.h"

class AdminWindow : public MainWindow
{
	Q_OBJECT

public:
	AdminWindow(UserManager& userman,QWidget *parent = Q_NULLPTR);
	~AdminWindow();

private:
	QTabWidget *tabs;
	QTableView *usersTable, *residencesTable, *contractsTable;
	UserManager& userman;
};
