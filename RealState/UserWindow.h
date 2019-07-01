#pragma once

#include "Manager.h"
#include "MainWindow.h"
#include "UserInfoWindow.h"
#include "CommissionRateWindow.h"
#include "UsersTab.h"
#include "ResidencesTab.h"
#include "ContractsTab.h"

class UserWindow : public MainWindow
{
	Q_OBJECT

public:
	UserWindow(ContractManager &conManager, int userId, QWidget *parent = Q_NULLPTR);
	~UserWindow();

	public slots:
	void changeinfo_clicked(QAction*);
	void tab_changed(int index);

private:

	UserManager &userManager;
	ResidenceManager &resManager;
	ContractManager &conManager;

	QTabWidget *tabs;
	ResidencesTab *ressTab;
	ContractsTab *consTab;
	int userId;
};
