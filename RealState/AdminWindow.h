#pragma once

#include "Manager.h"
#include "MainWindow.h"
#include "UserInfoWindow.h"
#include "CommissionRateWindow.h"
#include "UsersTab.h"
#include "ResidencesTab.h"
#include "ContractsTab.h"

class AdminWindow : public MainWindow
{
	Q_OBJECT

public:
	AdminWindow(ContractManager &conManager, int userId, QWidget *parent = Q_NULLPTR);
	~AdminWindow();

public slots:
	void crateedit_clicked();
	void changeinfo_clicked();
	void changeinfo_closed();
	void tab_changed(int index);

private:

	UserManager &userManager;
	ResidenceManager &resManager;
	ContractManager &conManager;

	QTabWidget *tabs;
	UsersTab *usersTab;
	ResidencesTab *ressTab;
	ContractsTab *consTab;
	int userId;
};
