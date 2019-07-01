#pragma once

#include "Manager.h"
#include "MainWindow.h"

class AddApartmentWindow : public MainWindow
{
	Q_OBJECT

public:
	AddApartmentWindow(ResidenceManager &resManager, int bId, QWidget *parent = Q_NULLPTR);
	~AddApartmentWindow();

public slots:
	void cancel_clicked();
	void add_clicked();
	void browse_clicked();

private:
	ResidenceManager &resManager;
	int bId;

	QFormLayout *form;
	QSpinBox *buildareaEdit, *numberofroomsEdit, *basepriceEdit, *floornumberEdit;;
	QLabel *photopathEdit;
	QComboBox *usagebox;
};
