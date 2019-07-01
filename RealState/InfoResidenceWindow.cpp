#include "InfoResidenceWindow.h"

InfoResidenceWindow::InfoResidenceWindow(ContractManager &conManager, int userId, int resId, bool readonly, QWidget *parent)
	: MainWindow(parent), conManager(conManager), userManager(conManager.get_userManager()),
	resManager(conManager.get_residenceManager()), userId(userId), resId(resId)
{
	admin = userManager.query_user(userId)->get_type() == UserType::Admin;

	Residence *res = resManager.query_residence(resId);
	ResidenceType type = res->get_type();

	setWindowTitle("Residence information");
	//setMinimumWidth(400);
	//setMaximumSize(1200, 800);

	QFormLayout *form = new QFormLayout();
	if (type != ResidenceType::Apartment)
	{
		form->addRow("Base price:", new QLabel(QString::number(res->get_baseprice())));
	}

	if (type != ResidenceType::ApartmentBuilding)
	{
		form->addRow("Build area:", new QLabel(QString::number(res->get_buildarea())));
		form->addRow("Number of rooms:", new QLabel(QString::number(res->get_numberofrooms())));
	}
	if (type == ResidenceType::NorthernVilla)
	{
		NothernVilla &nvilla = dynamic_cast<NothernVilla&>(*res);

		form->addRow("Front-yard area:", new QLabel(QString::number(nvilla.get_frontyardarea())));
		form->addRow("Backyard area:", new QLabel(QString::number((nvilla.get_backyardarea()))));
	}
	else if (type == ResidenceType::SouthernVilla)
	{
		SouthernVilla &svilla = dynamic_cast<SouthernVilla&>(*res);

		form->addRow("Yard area:", new QLabel(QString::number(svilla.get_yardarea())));
		form->addRow("Garage area:", new QLabel(QString::number(svilla.get_garagearea())));
	}
	else if (type == ResidenceType::ApartmentBuilding)
	{
		ApartmentBuilding &apb = dynamic_cast<ApartmentBuilding&>(*res);

		form->addRow("Total area:", new QLabel(QString::number(apb.get_totalarea())));
		form->addRow("Number of floors:", new QLabel(QString::number(apb.get_numberoffloors())));
		form->addRow("Number of apartments:", new QLabel(QString::number(apb.get_numberofapartments())));
		form->addRow("Elevator available", new QLabel(apb.get_haselevator() ? "Yes" : "No"));
	}
	else if (type == ResidenceType::Apartment)
	{
		Apartment &ap = dynamic_cast<Apartment&>(*res);

		form->addRow("Floor number:", new QLabel(QString::number(ap.get_floornumber())));
		form->addRow("Usage", new QLabel(tostring(ap.get_usage())));
	}

	if (type != ResidenceType::Apartment)
	{
		form->addRow("Address", new QLabel(QString::fromStdString(res->get_address())));
	}

	photoLabel = new QLabel();
	if (res->get_photodata() != "")
	{
		QImage img = QImage::fromData(QByteArray::fromBase64(res->get_photodata().c_str()));
		QPixmap pic = QPixmap::fromImage(img);
		photoLabel->setPixmap(pic.scaled(
			img.width() > 700 ? 700 : img.width(), img.height() > 600 ? 600 : img.height(),
			Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}
	photoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QVBoxLayout *detailsLayout = new QVBoxLayout();
	detailsLayout->addLayout(form);
	detailsLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));

	QHBoxLayout *infoLayout = new QHBoxLayout();
	infoLayout->addLayout(detailsLayout);
	infoLayout->addWidget(photoLabel, 1);

	QPushButton *cancelButton = new QPushButton((readonly || admin) ? "&OK" : "&Cancel");
	QPushButton *rentButton = new QPushButton("Apply for &renting");
	QPushButton *buyButton = new QPushButton("Apply for &buying");
	QPushButton *addapartmentButton = new QPushButton("&Add apartment");

	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->addSpacerItem(new QSpacerItem(150, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
	buttonsLayout->addWidget(cancelButton);
	buttonsLayout->addWidget(rentButton);
	buttonsLayout->addWidget(buyButton);
	buttonsLayout->addWidget(addapartmentButton);

	rentButton->setVisible(!readonly && !admin);
	buyButton->setVisible(!readonly && !admin);
	addapartmentButton->setVisible(!readonly && admin && type == ResidenceType::ApartmentBuilding);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addLayout(infoLayout);
	layout->addLayout(buttonsLayout);

	QWidget *widget = new QWidget();
	widget->setLayout(layout);
	setCentralWidget(widget);

	setFixedSize(sizeHint());

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel_clicked()));
	connect(rentButton, SIGNAL(clicked()), this, SLOT(rent_clicked()));
	connect(buyButton, SIGNAL(clicked()), this, SLOT(buy_clicked()));
	connect(addapartmentButton, SIGNAL(clicked()), this, SLOT(addapartment_clicked()));
}

InfoResidenceWindow::~InfoResidenceWindow()
{
}

void InfoResidenceWindow::cancel_clicked()
{
	close();
}

void InfoResidenceWindow::rent_clicked()
{
	AddContractInfo info;
	info.type = ContractType::Rent;
	(new AddContractWindow(info, this))->exec();
	if (info.canceled)
		return;

	RentContract rent;
	rent.set_commissionrate(conManager.get_commissionrate());
	rent.set_holderid(userId);
	rent.set_holder(userManager.query_user(userId));
	rent.set_rentduration(info.duration);
	rent.set_residenceid(resId);
	rent.set_residence(resManager.query_residence(resId));
	rent.set_verified(false);
	conManager.add_contract(rent);
	close();
}

void InfoResidenceWindow::buy_clicked()
{
	AddContractInfo info;
	info.type = ContractType::Sale;
	(new AddContractWindow(info, this))->exec();
	if (info.canceled)
		return;

	SaleContract sale;
	sale.set_commissionrate(conManager.get_commissionrate());
	sale.set_holderid(userId);
	sale.set_holder(userManager.query_user(userId));
	sale.set_terms(info.terms);
	sale.set_residenceid(resId);
	sale.set_residence(resManager.query_residence(resId));
	sale.set_verified(false);
	conManager.add_contract(sale);
	close();
}

void InfoResidenceWindow::addapartment_clicked()
{
	auto aps = resManager.query_residence([id = resId](Residence &r) {
		if (r.get_type() == ResidenceType::Apartment)
		{
			Apartment &a = dynamic_cast<Apartment&>(r);
			return a.get_buildingid() == id;
		}
		return false;
	});

	if (aps.size() >= dynamic_cast<ApartmentBuilding&>(*resManager.query_residence(resId)).get_numberofapartments())
	{
		QMessageBox mbox(this);
		mbox.setText("Ther is no available apartments for this building.");
		mbox.setWindowTitle("Error!");
		mbox.setIcon(QMessageBox::Icon::Warning);
		mbox.exec();
		return;
	}

	AddApartmentWindow *wnd = new AddApartmentWindow(resManager, resId, this);
	wnd->setWindowModality(Qt::ApplicationModal);
	wnd->show();
}
