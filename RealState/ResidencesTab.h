#pragma once

#include <Manager.h>
#include <MainWindow.h>

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

private:
	void populate();
	bool admin;
	int userId;
	UserManager &userManager;
	ResidenceManager &resManager;
	ContractManager &conManager;

	QLabel *countLabel;
	QStandardItemModel *model;
	QTableView *table;
};
