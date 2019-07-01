#include "SearchResidenceWindow.h"

SearchResidenceWindow::SearchResidenceWindow(SearchInfo &info, QWidget *parent)
	: MainWindow(parent), info(info)
{
	type = info.type;

	max_basepriceEdit = new QSpinBox();
	max_basepriceEdit->setRange(0, 100'000'000'000'000);
	max_basepriceEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
	max_basepriceEdit->setValue(info.max_baseprice);
	min_buildareaEdit = new QSpinBox();
	min_buildareaEdit->setRange(0, 100'000'000'000'000);
	min_buildareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
	min_buildareaEdit->setValue(info.min_buildarea);
	min_apartmentsEdit = new QSpinBox();
	min_apartmentsEdit->setValue(info.min_apartments);
	same_buildareaEdit = new QSpinBox();
	same_buildareaEdit->setRange(0, 100'000'000'000'000);
	same_buildareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
	same_buildareaEdit->setValue(info.same_buildarea);
	min_roomsEdit = new QSpinBox();
	min_roomsEdit->setValue(info.min_rooms);
	streetEdit = new QLineEdit(QString::fromStdString(info.street));

	nvillaBox = new QCheckBox("North Villa");
	nvillaBox->setChecked(info.nvilla);
	svillaBox = new QCheckBox("North Villa");
	svillaBox->setChecked(info.svilla);
	buildingBox = new QCheckBox("Apartment building");
	buildingBox->setChecked(info.building);
	apartmentBox = new QCheckBox("Apartment");
	apartmentBox->setChecked(info.apartment);

	QHBoxLayout *l[7];
	QRadioButton *r[6];
	for (int i = 0; i < 6; i++)
	{
		l[i] = new QHBoxLayout();
		r[i] = new QRadioButton();
		r[i]->setProperty("i", i);
		l[i]->addWidget(r[i]);
		connect(r[i], SIGNAL(clicked()), this, SLOT(type_changed()));
	}
	l[6] = new QHBoxLayout();
	r[(int)info.type]->setChecked(true);

	l[0]->addWidget(new QLabel("None"));
	l[0]->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

	l[1]->addWidget(new QLabel("Base price less than "));
	l[1]->addWidget(max_basepriceEdit, 1);
	l[1]->addWidget(new QLabel(" and build area more than "));
	l[1]->addWidget(min_buildareaEdit, 1);

	l[2]->addWidget(new QLabel("Is on street"));
	l[2]->addWidget(streetEdit, 1);

	l[3]->addWidget(new QLabel("Has more than "));
	l[3]->addWidget(min_apartmentsEdit, 1);
	l[3]->addWidget(new QLabel(" apartments"));

	l[4]->addWidget(new QLabel("Apartment buildings having at least one free apartment with build area of "));
	l[4]->addWidget(same_buildareaEdit, 1);

	l[5]->addWidget(new QLabel("Apartment buildings having at least one free apartment with "));
	l[5]->addWidget(min_roomsEdit, 1);
	l[5]->addWidget(new QLabel(" rooms"));

	l[6]->addWidget(new QLabel("Show: "));
	l[6]->addWidget(nvillaBox, 1);
	l[6]->addWidget(svillaBox, 1);
	l[6]->addWidget(buildingBox, 1);
	l[6]->addWidget(apartmentBox, 1);

	QVBoxLayout *layout = new QVBoxLayout();
	for (int i = 0; i < 7; i++)
		layout->addLayout(l[i]);

	QPushButton *cancelButton = new QPushButton("&Cancel");
	QPushButton *okButton = new QPushButton("&OK");


	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->addSpacerItem(new QSpacerItem(150, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
	buttonsLayout->addWidget(cancelButton);
	buttonsLayout->addWidget(okButton);
	layout->addLayout(buttonsLayout);

	QWidget *widget = new QWidget();
	widget->setLayout(layout);
	setCentralWidget(widget);

	setMaximumHeight(sizeHint().height()); 

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel_clicked()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(ok_clicked()));

	type_changed();
}

SearchResidenceWindow::~SearchResidenceWindow()
{
}

void SearchResidenceWindow::cancel_clicked()
{
	close();
}

void SearchResidenceWindow::ok_clicked()
{
	info.type = type;
	info.max_baseprice = max_basepriceEdit->value();
	info.min_buildarea = min_buildareaEdit->value();
	info.min_apartments = min_apartmentsEdit->value();
	info.same_buildarea = same_buildareaEdit->value();
	info.min_rooms = min_roomsEdit->value();
	info.street = streetEdit->text().toStdString();
	info.nvilla = nvillaBox->isChecked();
	info.svilla = svillaBox->isChecked();
	info.building = buildingBox->isChecked();
	info.apartment = apartmentBox->isChecked();

	close();
}

void SearchResidenceWindow::type_changed()
{
	if (sender() != NULL)
		type = (SearchInfo::Type)sender()->property("i").value<int>();

	max_basepriceEdit->setDisabled(true);
	min_buildareaEdit->setDisabled(true);
	min_apartmentsEdit->setDisabled(true);
	same_buildareaEdit->setDisabled(true);
	min_roomsEdit->setDisabled(true);
	streetEdit->setDisabled(true);

	switch (type)
	{
	case SearchInfo::Type::None:
		break;
	case SearchInfo::Type::MaxBasepriceMinBuildarea:
		max_basepriceEdit->setEnabled(true);
		min_buildareaEdit->setEnabled(true);
		break;
	case SearchInfo::Type::OnStreet:
		streetEdit->setEnabled(true);
		break;
	case SearchInfo::Type::MinApartments:
		min_apartmentsEdit->setEnabled(true);
		break;
	case SearchInfo::Type::MinApartmentSameBuildArea:
		same_buildareaEdit->setEnabled(true);
		break;
	case SearchInfo::Type::MinRooms:
		min_roomsEdit->setEnabled(true);
		break;
	default:
		break;
	}
}
