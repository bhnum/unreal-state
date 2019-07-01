#pragma once

#include "Manager.h"
#include "MainWindow.h"
#include "AddResidenceWindow.h"
#include "EditResidenceWindow.h"
#include "InfoResidenceWindow.h"
#include "SearchResidenceWindow.h"

class ResidencesTab : public QWidget
{
	Q_OBJECT

public:
	ResidencesTab(ContractManager &conManager, int userId, QWidget *parent = Q_NULLPTR);
	~ResidencesTab();

public slots:
	void search_clicked();
	void add_clicked();
	void apply_clicked();
	void edit_clicked();
	void delete_clicked();
	void populate();

private:
	bool admin;
	int userId;
	UserManager &userManager;
	ResidenceManager &resManager;
	ContractManager &conManager;
	SearchInfo searchInfo;

	QLabel *countLabel;
	QStandardItemModel *model;
	QTableView *table;
};
