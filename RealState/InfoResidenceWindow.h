#pragma once

#include "Manager.h"
#include "MainWindow.h"

class InfoResidenceWindow : public MainWindow
{
	Q_OBJECT

public:
	InfoResidenceWindow(ContractManager &conManager, int userId, int resId, QWidget *parent = Q_NULLPTR);
	~InfoResidenceWindow();

private:
	int userId, resId;
	bool admin;
	UserManager &userManager;
	ResidenceManager &resManager;
	ContractManager &conManager;
};
