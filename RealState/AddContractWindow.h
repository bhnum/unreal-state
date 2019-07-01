#pragma once

#include "Manager.h"
#include "MainWindow.h"

struct AddContractInfo
{
	int duration = 0;
	string terms;
	ContractType type;
	bool canceled = false;
};

class AddContractWindow : public QDialog
{
	Q_OBJECT

public:
	AddContractWindow(AddContractInfo &info, QWidget *parent = Q_NULLPTR);
	~AddContractWindow();

public slots:
	void cancel_clicked();
	void ok_clicked();

private:
	AddContractInfo &info;

	QSpinBox *durationEdit;
	QLineEdit *termsEdit;
};
