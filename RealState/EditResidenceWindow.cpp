#include "EditResidenceWindow.h"

EditResidenceWindow::EditResidenceWindow(ResidenceManager &resManager, int resId, QWidget *parent)
	: MainWindow(parent), resManager(resManager), resId(resId)
{
	old = resManager.query_residence(resId);
	ResidenceType type = old->get_type();

	setWindowTitle("Edit Residence");

	setMaximumHeight(sizeHint().height());
	setMinimumWidth(400);

	form = new QFormLayout();
	if (type != ResidenceType::Apartment)
	{
		basepriceEdit = new QSpinBox();
		basepriceEdit->setRange(0, 100'000'000'000'000);
		basepriceEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
		basepriceEdit->setValue(old->get_baseprice());

		form->addRow("Base price:", basepriceEdit);
	}

	if (type != ResidenceType::ApartmentBuilding)
	{
		buildareaEdit = new QSpinBox();
		buildareaEdit->setRange(0, 100'000'000'000'000);
		buildareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
		buildareaEdit->setValue(old->get_buildarea());

		form->addRow("Build area:", buildareaEdit);

		numberofroomsEdit = new QSpinBox();
		numberofroomsEdit->setValue(old->get_numberofrooms());

		form->addRow("Number of rooms:", numberofroomsEdit);
	}
	if (type == ResidenceType::NorthernVilla)
	{
		NothernVilla &oldv = dynamic_cast<NothernVilla&>(*old);
		frontyardareaEdit = new QSpinBox();
		frontyardareaEdit->setRange(0, 100'000'000'000'000);
		frontyardareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
		frontyardareaEdit->setValue(oldv.get_frontyardarea());

		backyardareaEdit = new QSpinBox();
		backyardareaEdit->setRange(0, 100'000'000'000'000);
		backyardareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
		backyardareaEdit->setValue(oldv.get_backyardarea());

		form->addRow("Front-yard area:", frontyardareaEdit);
		form->addRow("Backyard area:", backyardareaEdit);
	}
	else if (type == ResidenceType::SouthernVilla)
	{
		SouthernVilla &oldv = dynamic_cast<SouthernVilla&>(*old);
		yardareaEdit = new QSpinBox();
		yardareaEdit->setRange(0, 100'000'000'000'000);
		yardareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
		yardareaEdit->setValue(oldv.get_yardarea());

		garageareaEdit = new QSpinBox();
		garageareaEdit->setRange(0, 100'000'000'000'000);
		garageareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
		garageareaEdit->setValue(oldv.get_garagearea());

		form->addRow("Yard area:", yardareaEdit);
		form->addRow("Garage area:", garageareaEdit);
	}
	else if (type == ResidenceType::ApartmentBuilding)
	{
		ApartmentBuilding &olda = dynamic_cast<ApartmentBuilding&>(*old);
		totalareaEdit = new QSpinBox();
		totalareaEdit->setRange(0, 100'000'000'000'000);
		totalareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
		totalareaEdit->setValue(olda.get_totalarea());

		numberoffloorsEdit = new QSpinBox();
		numberoffloorsEdit->setValue(olda.get_numberoffloors());

		numberofapartmentsEdit = new QSpinBox();
		numberofapartmentsEdit->setValue(olda.get_numberofapartments());

		haselevatorBox = new QCheckBox("Elevator available");
		haselevatorBox->setChecked(olda.get_haselevator());

		form->addRow("Total area:", totalareaEdit);
		form->addRow("Number of floors:", numberoffloorsEdit);
		form->addRow("Number of apartments:", numberofapartmentsEdit);
		form->addRow("", haselevatorBox);
	}
	else if (type == ResidenceType::Apartment)
	{
		Apartment &olda = dynamic_cast<Apartment&>(*old);
		floornumberEdit = new QSpinBox();
		floornumberEdit->setValue(olda.get_floornumber());

		usagebox = new QComboBox();
		usagebox->addItem("Regular usage", (int)ApartmentUsage::Regular);
		usagebox->addItem("Business usage", (int)ApartmentUsage::Business);
		usagebox->addItem("Administrative usage", (int)ApartmentUsage::Administrative);
		usagebox->setCurrentIndex((int)olda.get_usage());

		form->addRow("Floor number:", floornumberEdit);
		form->addRow("Usage", usagebox);
	}

	if (type != ResidenceType::Apartment)
	{
		addressEdit = new QLineEdit(QString::fromStdString(old->get_address()));

		form->addRow("Address", addressEdit);
	}

	photopathEdit = new QLabel();
	QPushButton *browseButton = new QPushButton("...");
	browseButton->setMaximumWidth(30);
	browseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QHBoxLayout *photopathLayout = new QHBoxLayout();
	photopathLayout->addWidget(photopathEdit);
	photopathLayout->addWidget(browseButton);

	QPushButton *cancelButton = new QPushButton("&Cancel");
	QPushButton *editButton = new QPushButton("&Edit");

	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->addWidget(cancelButton);
	buttonsLayout->addWidget(editButton);
	buttonsLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

	form->addRow("New photo", photopathLayout);
	form->addRow("", buttonsLayout);

	QWidget *widget = new QWidget();
	widget->setLayout(form);
	setCentralWidget(widget);

	setMaximumHeight(sizeHint().height());

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel_clicked()));
	connect(editButton, SIGNAL(clicked()), this, SLOT(edit_clicked()));
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse_clicked()));
}

EditResidenceWindow::~EditResidenceWindow()
{
}

void EditResidenceWindow::cancel_clicked()
{
	close();
}

void EditResidenceWindow::edit_clicked()
{
	Residence *r;
	switch (old->get_type())
	{
	case ResidenceType::NorthernVilla:
	{
		NothernVilla *v = new NothernVilla();
		r = v;
		r->set_buildarea(buildareaEdit->value());
		v->set_numberofrooms(numberofroomsEdit->value());
		v->set_frontyardarea(frontyardareaEdit->value());
		v->set_backyardarea(backyardareaEdit->value());
		v->set_baseprice(basepriceEdit->value());
		v->set_address(addressEdit->text().toStdString());
		break;
	}
	case ResidenceType::SouthernVilla:
	{
		SouthernVilla *v = new SouthernVilla();
		r = v;
		r->set_buildarea(buildareaEdit->value());
		v->set_numberofrooms(numberofroomsEdit->value());
		v->set_yardarea(yardareaEdit->value());
		v->set_garagearea(garageareaEdit->value());
		v->set_baseprice(basepriceEdit->value());
		v->set_address(addressEdit->text().toStdString());
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
		a->set_baseprice(basepriceEdit->value());
		a->set_address(addressEdit->text().toStdString());
		break;
	}
	case ResidenceType::Apartment:
	{
		Apartment *a = new Apartment();
		r = a;
		a->set_buildarea(buildareaEdit->value());
		a->set_numberofrooms(numberofroomsEdit->value());
		a->set_floornumber(floornumberEdit->value());
		a->set_usage((ApartmentUsage)usagebox->currentData().value<int>());
		Apartment& olda = dynamic_cast<Apartment&>(*old);
		a->set_buildingid(olda.get_buildingid());
		a->set_building(olda.get_building());
	}
	default:
		throw std::logic_error("");
		break;
	}

	r->set_id(old->get_id());

	if (photopathEdit->text().isEmpty())
		r->set_photodata(old->get_photodata());
	else
	{
		QImage img(photopathEdit->text());
		QByteArray byteArray;
		QBuffer buffer(&byteArray);
		img.save(&buffer, "PNG"); // writes the image in PNG format inside
		r->set_photodata(byteArray.toBase64().data());
	}

	try
	{
		resManager.edit_residence(*r);
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
	delete r;
	QMessageBox mbox(this);
	mbox.setText("Residence edited successfully.");
	mbox.setWindowTitle("!");
	mbox.setIcon(QMessageBox::Icon::Information);
	mbox.exec();
	close();
}

void EditResidenceWindow::browse_clicked()
{
	auto filename = QFileDialog::getOpenFileName(this, "Load from file",
		"", "Images (*.png *.jpg *.jpeg *.gif *.bmp);;All files (*.*)");
	if (filename == nullptr)
		return;
	photopathEdit->setText(filename);
}
