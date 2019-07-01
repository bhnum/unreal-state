#include "AddContractWindow.h"

AddContractWindow::AddContractWindow(AddContractInfo &info, QWidget *parent)
	: QDialog(parent), info(info)
{
	QHBoxLayout *crateLayout = new QHBoxLayout();
	if (info.type == ContractType::Rent)
	{
		durationEdit = new QSpinBox();
		crateLayout->addWidget(new QLabel("Enter duration of stay: "));
		crateLayout->addWidget(durationEdit, 1);
	}
	else
	{
		termsEdit = new QLineEdit();
		crateLayout->addWidget(new QLabel("Enter terms: "));
		crateLayout->addWidget(termsEdit, 1);
	}

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

AddContractWindow::~AddContractWindow()
{
}

void AddContractWindow::cancel_clicked()
{
	info.canceled = true;
	close();
}

void AddContractWindow::ok_clicked()
{
	info.canceled = false;
	if (info.type == ContractType::Rent)
		info.duration = durationEdit->value();
	else
		info.terms = termsEdit->text().toStdString();
	close();
}
