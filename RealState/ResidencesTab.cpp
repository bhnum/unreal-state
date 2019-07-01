#include "ResidencesTab.h"

ResidencesTab::ResidencesTab(ContractManager &conManager, int userId, QWidget *parent)
	: QWidget(parent), conManager(conManager), userManager(conManager.get_userManager()),
	resManager(conManager.get_residenceManager()), userId(userId)
{
	admin = userManager.query_user(userId)->get_type() == UserType::Admin;

	countLabel = new QLabel("");
	QPushButton *searchButton = new QPushButton("Search");
	QPushButton *addButton = new QPushButton("Add residence");
	addButton->setVisible(admin);

	QHBoxLayout *header = new QHBoxLayout();
	header->addWidget(countLabel);
	header->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
	header->addWidget(searchButton);
	header->addWidget(addButton);

	model = new QStandardItemModel();
	model->setColumnCount(9);
	model->setHeaderData(0, Qt::Horizontal, "Id");
	model->setHeaderData(1, Qt::Horizontal, "Type");
	model->setHeaderData(2, Qt::Horizontal, "Build area");
	model->setHeaderData(3, Qt::Horizontal, "Total area");
	model->setHeaderData(4, Qt::Horizontal, "Base price");
	model->setHeaderData(5, Qt::Horizontal, "Final price");
	model->setHeaderData(6, Qt::Horizontal, "No. of rooms");
	model->setHeaderData(7, Qt::Horizontal, "Address");
	model->setHeaderData(8, Qt::Horizontal, "Actions");

	table = new QTableView();
	table->setModel(model);
	table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeMode::Stretch);
	table->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeMode::ResizeToContents);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addLayout(header);
	layout->addWidget(table);
	setLayout(layout);

	table->setSortingEnabled(true);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	populate();

	connect(searchButton, SIGNAL(clicked()), this, SLOT(search_clicked()));
	connect(addButton, SIGNAL(clicked()), this, SLOT(add_clicked()));
}

ResidencesTab::~ResidencesTab()
{
}

void ResidencesTab::populate() {

	for (int i = 0; i < model->rowCount(); i++)
	{
		auto tmp = table->indexWidget(model->index(i, 10));
		table->setIndexWidget(model->index(i, 10), NULL);
		delete tmp;
	}
	model->removeRows(0, model->rowCount());

	auto filtered = resManager.query_residence([&conManager = this->conManager, &searchInfo = searchInfo](Residence &r)
	{
		if (!searchInfo.nvilla && r.get_type() == ResidenceType::NorthernVilla)
			return false;
		if (!searchInfo.svilla && r.get_type() == ResidenceType::SouthernVilla)
			return false;
		if (!searchInfo.building && r.get_type() == ResidenceType::ApartmentBuilding)
			return false;
		if (!searchInfo.apartment && r.get_type() == ResidenceType::Apartment)
			return false;

		switch (searchInfo.type)
		{
		case SearchInfo::Type::None:
			break;
		case SearchInfo::Type::MaxBasepriceMinBuildarea:
			if (r.get_baseprice() > searchInfo.max_baseprice || r.get_buildarea() < searchInfo.min_buildarea)
				return false;
			break;
		case SearchInfo::Type::OnStreet:
			if (r.get_address().find(searchInfo.street) == string::npos)
				return false;
			break;
		case SearchInfo::Type::MinApartments:
			if (r.get_type() == ResidenceType::ApartmentBuilding) {
				if (dynamic_cast<ApartmentBuilding&>(r).get_numberofapartments() < searchInfo.min_apartments)
					return false;
			}
			else
				return false;
			break;
		case SearchInfo::Type::MinApartmentSameBuildArea:
			if (r.get_type() == ResidenceType::ApartmentBuilding) {
				auto aps = conManager.get_residenceManager().query_residence([bArea = searchInfo.same_buildarea, bId = dynamic_cast<ApartmentBuilding&>(r).get_id()](Residence &r)
				{
					if (r.get_type() == ResidenceType::Apartment)
						if (dynamic_cast<Apartment&>(r).get_buildingid() == bId)
							return r.get_buildarea() == bArea;
					return false;
				});
				if (aps.size() == 0)
					return false;
			}
			else
				return false;
			break;
		case SearchInfo::Type::MinApartmentSameRooms:
			if (r.get_type() == ResidenceType::ApartmentBuilding) {
				auto aps = conManager.get_residenceManager().query_residence([rooms = searchInfo.same_rooms, bId = dynamic_cast<ApartmentBuilding&>(r).get_id()](Residence &r)
				{
					if (r.get_type() == ResidenceType::Apartment)
						if (dynamic_cast<Apartment&>(r).get_buildingid() == bId)
							return r.get_numberofrooms() == rooms;
					return false;
				});
				if (aps.size() == 0)
					return false;
			}
			else
				return false;
			break;
		default:
			throw std::logic_error("");
			break;
		}

		// Check free
		return !conManager.is_residence_taken(r.get_id());
	});

	int row = 0;
	for each (auto i in filtered)
	{
		model->appendRow(new QStandardItem());
		model->setData(model->index(row, 0), i->get_id(), Qt::DisplayRole);
		model->setData(model->index(row, 1), QString::fromStdString(tostring(i->get_type())), Qt::DisplayRole);
		model->setData(model->index(row, 2), i->get_buildarea(), Qt::DisplayRole);
		model->setData(model->index(row, 3), i->get_totalarea(), Qt::DisplayRole);
		model->setData(model->index(row, 4), i->get_baseprice(), Qt::DisplayRole);
		model->setData(model->index(row, 5), i->get_finalprice(), Qt::DisplayRole);
		model->setData(model->index(row, 6), i->get_numberofrooms() , Qt::DisplayRole);
		model->setData(model->index(row, 7), QString::fromStdString(i->get_address()), Qt::DisplayRole);

		QPushButton *applyButton = new QPushButton(admin ? "Info" : "Apply");
		QPushButton *editButton = new QPushButton("Edit");
		QPushButton *deleteButton = new QPushButton("Delete");
		QHBoxLayout *buttonsLayout = new QHBoxLayout();
		buttonsLayout->addWidget(applyButton);
		buttonsLayout->addWidget(editButton);
		buttonsLayout->addWidget(deleteButton);
		buttonsLayout->setSpacing(0);
		buttonsLayout->setMargin(0);

		QWidget *buttonsWidget = new QWidget();
		buttonsWidget->setLayout(buttonsLayout);

		table->setIndexWidget(model->index(model->rowCount() - 1, 8), buttonsWidget);

		applyButton->setProperty("id", i->get_id());
		editButton->setProperty("id", i->get_id());
		deleteButton->setProperty("id", i->get_id());
		editButton->setVisible(admin);
		deleteButton->setVisible(admin);
		connect(applyButton, SIGNAL(clicked()), this, SLOT(apply_clicked()));
		connect(editButton, SIGNAL(clicked()), this, SLOT(edit_clicked()));
		connect(deleteButton, SIGNAL(clicked()), this, SLOT(delete_clicked()));

		row++;
	}
	countLabel->setText(QString::number(model->rowCount()) + " residences");
}

void ResidencesTab::search_clicked()
{
	SearchResidenceWindow *wnd = new SearchResidenceWindow(searchInfo, this);
	wnd->setWindowModality(Qt::ApplicationModal);
	wnd->show();

	connect(wnd, SIGNAL(closed()), this, SLOT(populate()));
}

void ResidencesTab::add_clicked()
{
	AddResidenceWindow *wnd = new AddResidenceWindow(resManager, this);
	wnd->setWindowModality(Qt::ApplicationModal);
	wnd->show();

	connect(wnd, SIGNAL(closed()), this, SLOT(populate()));
}

void ResidencesTab::apply_clicked()
{
	int resId = sender()->property("id").value<int>();
	InfoResidenceWindow *wnd = new InfoResidenceWindow(conManager, userId, resId, this);
	wnd->setWindowModality(Qt::ApplicationModal);
	wnd->show();

	connect(wnd, SIGNAL(closed()), this, SLOT(populate()));
}

void ResidencesTab::edit_clicked()
{
	int id = sender()->property("id").value<int>();
	EditResidenceWindow *wnd = new EditResidenceWindow(resManager, id, this);
	wnd->setWindowModality(Qt::ApplicationModal);
	wnd->show();

	connect(wnd, SIGNAL(closed()), this, SLOT(populate()));
}

void ResidencesTab::delete_clicked()
{
	int id = sender()->property("id").value<int>();
	resManager.delete_residence(id);
	populate();
}
