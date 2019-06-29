#pragma once

#include <QWidget>
#include "MainWindow.h"
#include "Manager.h"

class UsermanWindow : public QWidget
{
	Q_OBJECT

public:
	UsermanWindow(UserManager&userman,QWidget *parent = Q_NULLPTR);
	~UsermanWindow();
	public slots:
	void action_clicked();
private:
	void populate();
	QStandardItemModel* model;
	QTableView *usersTable;
	UserManager&userman;
};
