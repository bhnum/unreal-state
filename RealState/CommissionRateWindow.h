#pragma once

#include "Manager.h"
#include "MainWindow.h"

class CommissionRateWindow : public QDialog
{
	Q_OBJECT

public:
	CommissionRateWindow(ContractManager &conManager, QWidget *parent = Q_NULLPTR);
	~CommissionRateWindow();

public slots:
	void cancel_clicked();
	void ok_clicked();

private:
	ContractManager &conManager;
	QSpinBox *crateEdit;
};
