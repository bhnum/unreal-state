#include "AddResidenceWindow.h"

AddResidenceWindow::AddResidenceWindow(ResidenceManager &resManager, QWidget *parent)
	: MainWindow(parent), resManager(resManager)
{
	setWindowTitle("Add Residence");

	setMaximumHeight(sizeHint().height());
	setMinimumWidth(400);

	typeBox = new QComboBox();
	typeBox->addItem("Nothern villa", (int)ResidenceType::NorthernVilla);
	typeBox->addItem("Southern villa", (int)ResidenceType::SouthernVilla);
	typeBox->addItem("Apartment building", (int)ResidenceType::ApartmentBuilding);

	basepriceEdit = new QSpinBox();
	basepriceEdit->setRange(0, 100'000'000'000'000);
	basepriceEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);

	buildareaEdit = new QSpinBox();
	buildareaEdit->setRange(0, 100'000'000'000'000);
	buildareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);

	numberofroomsEdit = new QSpinBox();

	frontyardareaEdit = new QSpinBox();
	frontyardareaEdit->setRange(0, 100'000'000'000'000);
	frontyardareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);

	backyardareaEdit = new QSpinBox();
	backyardareaEdit->setRange(0, 100'000'000'000'000);
	backyardareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);

	yardareaEdit = new QSpinBox();
	yardareaEdit->setRange(0, 100'000'000'000'000);
	yardareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);

	garageareaEdit = new QSpinBox();
	garageareaEdit->setRange(0, 100'000'000'000'000);
	garageareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);

	totalareaEdit = new QSpinBox();
	totalareaEdit->setRange(0, 100'000'000'000'000);
	totalareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);

	numberoffloorsEdit = new QSpinBox();
	numberofapartmentsEdit = new QSpinBox();

	haselevatorBox = new QCheckBox("Elevator available");
	addressEdit = new QLineEdit();

	photopathEdit = new QLabel();
	QPushButton *browseButton = new QPushButton("...");
	browseButton->setMaximumWidth(30);
	browseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	photopathLayout = new QHBoxLayout();
	photopathLayout->addWidget(photopathEdit);
	photopathLayout->addWidget(browseButton);

	QPushButton *cancelButton = new QPushButton("&Cancel");
	QPushButton *addButton = new QPushButton("&Add");

	buttonsLayout = new QHBoxLayout();
	buttonsLayout->addWidget(cancelButton);
	buttonsLayout->addWidget(addButton);
	buttonsLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

	QWidget *widget = new QWidget();
	setCentralWidget(widget);

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel_clicked()));
	connect(addButton, SIGNAL(clicked()), this, SLOT(add_clicked()));
	connect(typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(type_changed(int)));
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse_clicked()));

	form = nullptr;
	refresh(ResidenceType::NorthernVilla);
}

AddResidenceWindow::~AddResidenceWindow()
{
	delete typeBox;
	delete buildareaEdit;
	delete basepriceEdit;
	delete numberofroomsEdit;
	delete frontyardareaEdit;
	delete backyardareaEdit;
	delete yardareaEdit;
	delete garageareaEdit;
	delete totalareaEdit;
	delete numberoffloorsEdit;
	delete numberofapartmentsEdit;
	delete haselevatorBox;
	delete addressEdit;
	delete photopathLayout;
	delete buttonsLayout;
}

void AddResidenceWindow::cancel_clicked()
{
	close();
}

void AddResidenceWindow::add_clicked()
{
	IndipendentResidence *r;
	switch ((ResidenceType)typeBox->currentData().value<int>())
	{
	case ResidenceType::NorthernVilla:
	{
		NothernVilla *v = new NothernVilla();
		r = v;
		v->set_buildarea(buildareaEdit->value());
		v->set_numberofrooms(numberofroomsEdit->value());
		v->set_frontyardarea(frontyardareaEdit->value());
		v->set_backyardarea(backyardareaEdit->value());
		break;
	}
	case ResidenceType::SouthernVilla:
	{
		SouthernVilla *v = new SouthernVilla();
		r = v;
		v->set_buildarea(buildareaEdit->value());
		v->set_numberofrooms(numberofroomsEdit->value());
		v->set_yardarea(yardareaEdit->value());
		v->set_garagearea(garageareaEdit->value());
		break;
	}
	case ResidenceType::ApartmentBuilding:
	{
		ApartmentBuilding *a = new ApartmentBuilding();
		r = a;
		a->set_totalarea(totalareaEdit->value());
		a->set_numberoffloors(numberoffloorsEdit->value());
		a->set_numberofapartments(numberofapartmentsEdit->value());
		a->set_haselevator(haselevatorBox->isChecked());
		break;
	}
	default:
		throw std::logic_error("");
		break;
	}
	r->set_baseprice(basepriceEdit->value());
	r->set_address(addressEdit->text().toStdString());

	if (!photopathEdit->text().isEmpty())
	{
		QImage img(photopathEdit->text());
		QByteArray byteArray;
		QBuffer buffer(&byteArray);
		img.save(&buffer, "PNG"); // writes the image in PNG format inside
		r->set_photodata(byteArray.toBase64().data());
	}

	try
	{
		resManager.add_residence(*r);
	}
	catch (const std::exception& ex)
	{
		QMessageBox mbox(this);
		mbox.setText(ex.what());
		mbox.setWindowTitle("Error!");
		mbox.setIcon(QMessageBox::Icon::Warning);
		mbox.exec();
		return;
	}
	QMessageBox mbox(this);
	mbox.setText("Residence added successfully.");
	mbox.setWindowTitle("!");
	mbox.setIcon(QMessageBox::Icon::Information);
	mbox.exec();
	close();
}

void AddResidenceWindow::type_changed(int)
{
	ResidenceType type = (ResidenceType)typeBox->currentData().value<int>();
	refresh(type);
}

void AddResidenceWindow::refresh(ResidenceType type)
{
	typeBox->setParent(nullptr);
	buildareaEdit->setParent(nullptr);
	basepriceEdit->setParent(nullptr);
	numberofroomsEdit->setParent(nullptr);
	frontyardareaEdit->setParent(nullptr);
	backyardareaEdit->setParent(nullptr);
	yardareaEdit->setParent(nullptr);
	garageareaEdit->setParent(nullptr);
	totalareaEdit->setParent(nullptr);
	numberoffloorsEdit->setParent(nullptr);
	numberofapartmentsEdit->setParent(nullptr);
	haselevatorBox->setParent(nullptr);
	addressEdit->setParent(nullptr);
	photopathLayout->setParent(nullptr);
	buttonsLayout->setParent(nullptr);

	form = new QFormLayout();
	QWidget *widget = new QWidget();
	widget->setLayout(form);
	setCentralWidget(widget);

	form->addRow("Residence type:", typeBox);
	form->addRow("Base price:", basepriceEdit);
	if (type != ResidenceType::ApartmentBuilding)
	{
		form->addRow("Build area:", buildareaEdit);
		form->addRow("Number of rooms:", numberofroomsEdit);
	}
	if (type == ResidenceType::NorthernVilla)
	{
		form->addRow("Front-yard area:", frontyardareaEdit);
		form->addRow("Backyard area:", backyardareaEdit);
	}
	else if (type == ResidenceType::SouthernVilla)
	{
		form->addRow("Yard area:", yardareaEdit);
		form->addRow("Garage area:", garageareaEdit);
	}
	else if (type == ResidenceType::ApartmentBuilding)
	{
		form->addRow("Total area:", totalareaEdit);
		form->addRow("Number of floors:", numberoffloorsEdit);
		form->addRow("Number of apartments:", numberofapartmentsEdit);
		form->addRow("", haselevatorBox);
	}
	form->addRow("Address", addressEdit);
	form->addRow("Photo", photopathLayout);

	form->addRow("", buttonsLayout);

	setMaximumHeight(sizeHint().height());
}

void AddResidenceWindow::browse_clicked()
{
	auto filename = QFileDialog::getOpenFileName(this, "Load from file",
		"", "Images (*.png *.jpg *.jpeg *.bmp);;All files (*.*)");
	if (filename == nullptr)
		return;
	photopathEdit->setText(filename);
}
