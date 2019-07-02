#include "ContractsTab.h"

ContractsTab::ContractsTab(ContractManager &conManager, int userId, QWidget *parent)
	: QWidget(parent), conManager(conManager), userManager(conManager.get_userManager()),
	resManager(conManager.get_residenceManager()), userId(userId)
{
	admin = userManager.query_user(userId)->get_type() == UserType::Admin;

	countLabel = new QLabel("");

	model = new QStandardItemModel();
	model->setColumnCount(10);
	model->setHeaderData(0, Qt::Horizontal, "Id");
	model->setHeaderData(1, Qt::Horizontal, "Residence");
	model->setHeaderData(2, Qt::Horizontal, "Commision rate (%)");
	model->setHeaderData(3, Qt::Horizontal, "Rent duration (months)");
	model->setHeaderData(4, Qt::Horizontal, "Rent per month");
	model->setHeaderData(5, Qt::Horizontal, "Mortgage");
	model->setHeaderData(6, Qt::Horizontal, "Terms of sale");
	model->setHeaderData(7, Qt::Horizontal, "Status");
	model->setHeaderData(8, Qt::Horizontal, "Final price");
	model->setHeaderData(9, Qt::Horizontal, "Actions");

	table = new QTableView();
	table->setModel(model);

	table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeMode::Stretch);
	table->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeMode::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeMode::ResizeToContents);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(countLabel);
	layout->addWidget(table);
	setLayout(layout);

	table->setSortingEnabled(true);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	populate();
}

ContractsTab::~ContractsTab()
{
}

void ContractsTab::cancel_clicked()
{
	int id = sender()->property("id").value<int>();
	
	Contract *c = conManager.query_contract(id);
	User *admin = userManager.query_user("admin");
	c->get_holder()->set_balance(c->get_holder()->get_balance() + c->get_finalprice());
	admin->set_balance(admin->get_balance() - c->get_finalprice());
	userManager.save();
	conManager.delete_contract(id);
	populate();
}

void ContractsTab::residence_clicked()
{
	int id = sender()->property("id").value<int>();

	InfoResidenceWindow *wnd = new InfoResidenceWindow(conManager, userId, id, true, this);
	wnd->setWindowModality(Qt::ApplicationModal);
	wnd->show();
}

void ContractsTab::accept_clicked()
{
	int id = sender()->property("id").value<int>();

	auto contract = conManager.query_contract(id);

	if (conManager.is_residence_taken(contract->get_residenceid()))
	{
		QMessageBox mbox(this);
		mbox.setText("The coresponding residence is already under another active contract and can not be contracted upon.");
		mbox.setWindowTitle("Error!");
		mbox.setIcon(QMessageBox::Icon::Warning);
		mbox.exec();
		return;
	}

	contract->set_verified(true);
	conManager.save();
	populate();
}

void ContractsTab::decline_clicked()
{
	int id = sender()->property("id").value<int>();
	conManager.delete_contract(id);
	populate();
}

void ContractsTab::populate()
{
	for (int i = 0; i < model->rowCount(); i++)
	{
		auto tmp = table->indexWidget(model->index(i, 1));
		table->setIndexWidget(model->index(i, 1), NULL);
		delete tmp;

		tmp = table->indexWidget(model->index(i, 8));
		table->setIndexWidget(model->index(i, 8), NULL);
		delete tmp;
	}
	model->removeRows(0, model->rowCount());

	list<Contract*> contracts = admin
		? conManager.get_contracts()
		: conManager.query_contract([userId = userId](Contract &c)
		{ return c.get_holderid() == userId; });

	int row = 0;
	for each (auto i in contracts)
	{
		RentContract *ri = dynamic_cast<RentContract*>(i);
		SaleContract *si = dynamic_cast<SaleContract*>(i);

		model->appendRow(new QStandardItem());
		model->setData(model->index(row, 0), i->get_id(), Qt::DisplayRole);
		model->setData(model->index(row, 1), QString::fromStdString(tostring(i->get_residence()->get_type())), Qt::DisplayRole);
		model->setData(model->index(row, 2), i->get_commissionrate(), Qt::DisplayRole);
		model->setData(model->index(row, 3), ri == nullptr ? 0 : ri->get_rentduration(), Qt::DisplayRole);
		model->setData(model->index(row, 4), ri == nullptr ? 0 : ri->get_rent(), Qt::DisplayRole);
		model->setData(model->index(row, 5), ri == nullptr ? 0 : ri->get_mortgage(), Qt::DisplayRole);
		model->setData(model->index(row, 6), si == nullptr ? "" : QString::fromStdString(si->get_terms()), Qt::DisplayRole);
		model->setData(model->index(row, 7), i->get_verified() ? "Accepted" : "Pending", Qt::DisplayRole);
		model->setData(model->index(row, 8), i->get_finalprice(), Qt::DisplayRole);

		QPushButton *residenceButton = new QPushButton("Residence");
		residenceButton->setProperty("id", i->get_residenceid());

		QPushButton *cancelButton = new QPushButton("Cancel");
		cancelButton->setProperty("id", i->get_id());

		QPushButton *acceptButton = new QPushButton("Accept");
		acceptButton->setProperty("id", i->get_id());

		QPushButton *declineButton = new QPushButton("Decline");
		declineButton->setProperty("id", i->get_id());

		QHBoxLayout *buttonsLayout = new QHBoxLayout();
		buttonsLayout->addWidget(residenceButton);
		buttonsLayout->addWidget(cancelButton);
		buttonsLayout->addWidget(acceptButton);
		buttonsLayout->addWidget(declineButton);
		buttonsLayout->setSpacing(0);
		buttonsLayout->setMargin(0);

		cancelButton->setVisible(!admin && !i->get_verified());
		acceptButton->setVisible(admin && !i->get_verified());
		declineButton->setVisible(admin && !i->get_verified());

		QWidget *buttonsWidget = new QWidget();
		buttonsWidget->setLayout(buttonsLayout);

		table->setIndexWidget(model->index(row, 9), buttonsWidget);

		connect(residenceButton, SIGNAL(clicked()), this, SLOT(residence_clicked()));
		connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel_clicked()));
		connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept_clicked()));
		connect(declineButton, SIGNAL(clicked()), this, SLOT(decline_clicked()));

		row++;
	}
	countLabel->setText(QString::number(model->rowCount()) + " users");
}
