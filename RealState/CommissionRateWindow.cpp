#include "CommissionRateWindow.h"

CommissionRateWindow::CommissionRateWindow(ContractManager &conManager, QWidget *parent)
	: QDialog(parent), conManager(conManager)
{
	crateEdit = new QSpinBox();
	crateEdit->setRange(0, 100);
	crateEdit->setValue(conManager.get_commissionrate());

	QHBoxLayout *crateLayout = new QHBoxLayout();
	crateLayout->addWidget(new QLabel("Set commission rate to "));
	crateLayout->addWidget(crateEdit, 1);
	crateLayout->addWidget(new QLabel(" %"));

	QPushButton *cancelButton = new QPushButton("&Cancel");
	QPushButton *okButton = new QPushButton("&OK");

	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
	buttonsLayout->addWidget(cancelButton);
	buttonsLayout->addWidget(okButton);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addLayout(crateLayout);
	layout->addLayout(buttonsLayout);
	setLayout(layout);

	setMaximumHeight(sizeHint().height());

	connect(okButton, SIGNAL(clicked()), this, SLOT(ok_clicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel_clicked()));
}

CommissionRateWindow::~CommissionRateWindow()
{
}

void CommissionRateWindow::cancel_clicked()
{
	close();
}

void CommissionRateWindow::ok_clicked()
{
	conManager.set_commissionrate(crateEdit->value());
	close();
}
