#pragma once

#include "Manager.h"
#include "MainWindow.h"

class AddResidenceWindow : public QMainWindow
{
	Q_OBJECT

public:
	AddResidenceWindow(ResidenceManager &resManager, QWidget *parent = Q_NULLPTR);
	~AddResidenceWindow();

	public slots:
	void cancel_clicked();
	void add_clicked();
	void type_changed(int);
	void browse_clicked();

private:
	ResidenceManager &resManager;

	QFormLayout *form;
	QComboBox *typeBox;
	QSpinBox *buildareaEdit, *numberofroomsEdit, *basepriceEdit,
		*frontyardareaEdit, *backyardareaEdit, *yardareaEdit,
		*garageareaEdit, *totalareaEdit, *numberoffloorsEdit,
		*numberofapartmentsEdit;
	QLineEdit *addressEdit;
	QLabel *photopathEdit;
	QCheckBox *haselevatorBox;
};
