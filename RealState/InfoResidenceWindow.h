#pragma once

#include "Manager.h"
#include "MainWindow.h"
#include "AddApartmentWindow.h"

class InfoResidenceWindow : public MainWindow
{
	Q_OBJECT

public:
	InfoResidenceWindow(ContractManager &conManager, int userId, int resId, QWidget *parent = Q_NULLPTR);
	~InfoResidenceWindow();

public slots:
	void cancel_clicked();
	void rent_clicked();
	void buy_clicked();
	void addapartment_clicked();

private:
	int userId, resId;
	bool admin;
	UserManager &userManager;
	ResidenceManager &resManager;
	ContractManager &conManager;
	QLabel *photoLabel;
};
