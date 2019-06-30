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
	model->setHeaderData(6, Qt::Horizontal, "Number of rooms");
	model->setHeaderData(7, Qt::Horizontal, "Address");
	model->setHeaderData(8, Qt::Horizontal, "Actions");

	table = new QTableView();
	table->setModel(model);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(countLabel);
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

	int row = 0;
	for each (auto i in resManager.get_residences())
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

		QPushButton *applyButton = new QPushButton("Apply");
		QPushButton *editButton = new QPushButton("Edit");
		QPushButton *deleteButton = new QPushButton("Delete");
		applyButton->setProperty("id", i->get_id());
		editButton->setProperty("id", i->get_id());
		deleteButton->setProperty("id", i->get_id());
		applyButton->setVisible(!admin);
		editButton->setVisible(admin);
		deleteButton->setVisible(admin);

		QHBoxLayout *buttonsLayout = new QHBoxLayout();
		buttonsLayout->addWidget(applyButton);
		buttonsLayout->addWidget(editButton);
		buttonsLayout->addWidget(deleteButton);

		QWidget *buttonsWidget = new QWidget();
		buttonsWidget->setLayout(buttonsLayout);

		table->setIndexWidget(model->index(model->rowCount() - 1, 10), buttonsWidget);
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
}

void ResidencesTab::apply_clicked()
{
}

void ResidencesTab::edit_clicked()
{
	int id = sender()->property("id").value<int>();
	auto contracts = conManager.query_contract([id](Contract &c) { return c.get_residenceid() == id; });
	if (contracts.size() > 0)
	{
		return;
	}
}

void ResidencesTab::delete_clicked()
{
	int id = sender()->property("id").value<int>();
	auto contracts = conManager.query_contract([id](Contract &c) { return c.get_residenceid() == id; });
	if (contracts.size() > 0)
	{
		return;
	}
	resManager.delete_residence(id);
}
