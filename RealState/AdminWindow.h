#pragma once

#include "MainWindow.h"
#include "Manager.h"

class AdminWindow : public MainWindow
{
	Q_OBJECT

public:
	AdminWindow(QWidget *parent = Q_NULLPTR);
	~AdminWindow();

private:
	QTabWidget *tabs;
	QTableView *usersTable, *residencesTable, *contractsTable;

};
