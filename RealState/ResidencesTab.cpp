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
	ContractManager &conManager = this->conManager;

	auto freeResidences = resManager.query_residence([&conManager](Residence &r)
	{
		auto contracts = conManager.query_contract([resId = r.get_id()](Contract &c)
		{
			return c.get_verified() && c.get_residenceid() == resId;
		});
		if (contracts.size() != 0)
			return false;

		if (r.get_type() == ResidenceType::Apartment)
		{
			Apartment &ap = dynamic_cast<Apartment &>(r);
			auto bcontracts = conManager.query_contract([bId = ap.get_buildingid()](Contract &c)
			{
				return c.get_verified() && c.get_residenceid() == bId;
			});
			if (bcontracts.size() != 0)
				return false;
		}
		else if (r.get_type() == ResidenceType::ApartmentBuilding)
		{
			ApartmentBuilding &building = dynamic_cast<ApartmentBuilding &>(r);
			auto contractedaps = conManager.get_residenceManager().query_residence([&conManager, bId = building.get_id()](Residence &r)
			{
				if (r.get_type() == ResidenceType::Apartment)
					if (dynamic_cast<Apartment&>(r).get_buildingid() == bId)
					{
						auto contracts = conManager.query_contract([resId = r.get_id()](Contract &c)
						{
							return c.get_verified() && c.get_residenceid() == resId;
						});
						return contracts.size() != 0;
					}
				return false;
			});
			
			if (contractedaps.size() != 0)
				return false;
		}
		return true;
	});

	int row = 0;
	for each (auto i in freeResidences)
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

	if (!admin)
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
