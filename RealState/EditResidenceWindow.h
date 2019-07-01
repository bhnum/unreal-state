#pragma once

#include "Manager.h"
#include "MainWindow.h"

class EditResidenceWindow : public MainWindow
{
	Q_OBJECT

public:
	EditResidenceWindow(ResidenceManager &resManager, int resId, QWidget *parent = Q_NULLPTR);
	~EditResidenceWindow();

public slots:
	void cancel_clicked();
	void edit_clicked();
	void browse_clicked();

private:
	int resId;
	Residence *old;
	ResidenceManager &resManager;

	QFormLayout *form;
	QSpinBox *buildareaEdit, *numberofroomsEdit, *basepriceEdit,
		*frontyardareaEdit, *backyardareaEdit, *yardareaEdit,
		*garageareaEdit, *totalareaEdit, *numberoffloorsEdit,
		*numberofapartmentsEdit, *floornumberEdit;
	QLineEdit *addressEdit;
	QLabel *photopathEdit;
	QCheckBox *haselevatorBox;
	QComboBox *usagebox;
};
