#pragma once

#include "Manager.h"
#include "MainWindow.h"
#include "InfoResidenceWindow.h"

class ContractsTab : public QWidget
{
	Q_OBJECT

public:
	ContractsTab(ContractManager &conManager, int userId, QWidget *parent = Q_NULLPTR);
	~ContractsTab();

public slots:
	void cancel_clicked();
	void residence_clicked();
	void accept_clicked();
	void decline_clicked();
	void populate();

private:
	int userId;
	bool admin;
	UserManager &userManager;
	ResidenceManager &resManager;
	ContractManager &conManager;

	QLabel *countLabel;
	QStandardItemModel *model;
	QTableView *table;
};
