#include "AddApartmentWindow.h"

AddApartmentWindow::AddApartmentWindow(ResidenceManager &resManager, int bId, QWidget *parent)
	: MainWindow(parent), resManager(resManager), bId(bId)
{
	form = new QFormLayout();

	buildareaEdit = new QSpinBox();
	buildareaEdit->setRange(0, 100'000'000'000'000);
	buildareaEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
	form->addRow("Build area:", buildareaEdit);

	numberofroomsEdit = new QSpinBox();
	form->addRow("Number of rooms:", numberofroomsEdit);

	floornumberEdit = new QSpinBox();
	form->addRow("Floor number:", floornumberEdit);

	usagebox = new QComboBox();
	usagebox->addItem("Regular usage", (int)ApartmentUsage::Regular);
	usagebox->addItem("Business usage", (int)ApartmentUsage::Business);
	usagebox->addItem("Administrative usage", (int)ApartmentUsage::Administrative);
	form->addRow("Usage", usagebox);

	photopathEdit = new QLabel();
	QPushButton *browseButton = new QPushButton("...");
	browseButton->setMaximumWidth(30);
	browseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QHBoxLayout *photopathLayout = new QHBoxLayout();
	photopathLayout->addWidget(photopathEdit);
	photopathLayout->addWidget(browseButton);

	QPushButton *cancelButton = new QPushButton("&Cancel");
	QPushButton *addButton = new QPushButton("&Add");

	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->addWidget(cancelButton);
	buttonsLayout->addWidget(addButton);
	buttonsLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

	form->addRow("New photo", photopathLayout);
	form->addRow("", buttonsLayout);

	QWidget *widget = new QWidget();
	widget->setLayout(form);
	setCentralWidget(widget);

	setMaximumHeight(sizeHint().height());

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel_clicked()));
	connect(addButton, SIGNAL(clicked()), this, SLOT(add_clicked()));
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse_clicked()));
}

AddApartmentWindow::~AddApartmentWindow()
{
}

void AddApartmentWindow::cancel_clicked()
{
	close();
}

void AddApartmentWindow::add_clicked()
{
	Apartment a;

	a.set_buildarea(buildareaEdit->value());
	a.set_numberofrooms(numberofroomsEdit->value());
	a.set_floornumber(floornumberEdit->value());
	a.set_usage((ApartmentUsage)usagebox->currentData().value<int>());
	a.set_building(&dynamic_cast<ApartmentBuilding&>(*resManager.query_residence(bId)));
	a.set_buildingid(bId);

	if (!photopathEdit->text().isEmpty())
	{
		QImage img(photopathEdit->text());
		QByteArray byteArray;
		QBuffer buffer(&byteArray);
		img.save(&buffer, "PNG"); // writes the image in PNG format inside
		a.set_photodata(byteArray.toBase64().data());
	}

	try
	{
		resManager.add_residence(a);
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
	mbox.setText("Apartment added successfully.");
	mbox.setWindowTitle("!");
	mbox.setIcon(QMessageBox::Icon::Information);
	mbox.exec();
	close();
}

void AddApartmentWindow::browse_clicked()
{
	auto filename = QFileDialog::getOpenFileName(this, "Load from file",
		"", "Images (*.png *.jpg *.jpeg *.bmp);;All files (*.*)");
	if (filename == nullptr)
		return;
	photopathEdit->setText(filename);
}
