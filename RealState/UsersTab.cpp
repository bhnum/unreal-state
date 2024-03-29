#include "UsersTab.h"

UsersTab::UsersTab(UserManager& userManager ,QWidget *parent)
	: QWidget(parent),userManager(userManager)
{
	countLabel = new QLabel("");

	model = new QStandardItemModel();
	model->setColumnCount(11);
	model->setHeaderData(0, Qt::Horizontal, "Id");
	model->setHeaderData(1, Qt::Horizontal, "Username");
	model->setHeaderData(2, Qt::Horizontal, "Name");
	model->setHeaderData(3, Qt::Horizontal, "Surname");
	model->setHeaderData(4, Qt::Horizontal, "Type");
	model->setHeaderData(5, Qt::Horizontal, "Balance");
	model->setHeaderData(6, Qt::Horizontal, "Birth date");
	model->setHeaderData(7, Qt::Horizontal, "Blocked");
	model->setHeaderData(8, Qt::Horizontal, "Last Login");
	model->setHeaderData(9, Qt::Horizontal, "Last Logout");
	model->setHeaderData(10, Qt::Horizontal, "Actions");

	table = new QTableView();
	table->setModel(model);

	table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(countLabel);
	layout->addWidget(table);
	setLayout(layout);
	
	table->setSortingEnabled(true);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	populate();
}

UsersTab::~UsersTab()
{
}

void UsersTab::action_clicked() {
	int id = sender()->property("id").value<int>();
	User* user = userManager.query_user(id);
	user->set_inactive(!user->get_inactive());
	userManager.save();
	populate();
}

void UsersTab::populate()
{	
	for (int i = 0; i < model->rowCount(); i++)
	{
		auto tmp = table->indexWidget(model->index(i, 10));
		table->setIndexWidget(model->index(i, 10),NULL);
		delete tmp;
	}
	model->removeRows(0, model->rowCount());

	int row = 0;
	for each (auto i in userManager.get_users())
	{
		model->appendRow(new QStandardItem());
		model->setData(model->index(row, 0), i.get_id(), Qt::DisplayRole);
		model->setData(model->index(row, 1), QString::fromStdString(i.get_username()), Qt::DisplayRole);
		model->setData(model->index(row, 2), QString::fromStdString(i.get_name()), Qt::DisplayRole);
		model->setData(model->index(row, 3), QString::fromStdString(i.get_surname()), Qt::DisplayRole);
		model->setData(model->index(row, 4), i.get_type() == UserType::Regular ? "Regular" : "Admin", Qt::DisplayRole);
		model->setData(model->index(row, 5), i.get_balance(), Qt::DisplayRole);
		model->setData(model->index(row, 6), QString::fromStdString(putdate(i.get_birthdate())), Qt::DisplayRole);
		model->setData(model->index(row, 7), i.get_inactive() ? "Yes" : "No", Qt::DisplayRole);
		model->setData(model->index(row, 8), i.get_lastlogintime() == time_point() ? "Never" : QString::fromStdString(puttime(i.get_lastlogintime())), Qt::DisplayRole);
		model->setData(model->index(row, 8), i.get_lastlogintime() == time_point() ? "Never" : QString::fromStdString(puttime(i.get_lastlogintime()) + " (" + puttimediff(i.get_lastlogintime()) + ")"), Qt::ToolTipRole);
		model->setData(model->index(row, 9), i.get_lastlogouttime() == time_point() ? "Never" : QString::fromStdString(puttime(i.get_lastlogouttime())), Qt::DisplayRole);
		model->setData(model->index(row, 9), i.get_lastlogouttime() == time_point() ? "Never" : QString::fromStdString(puttime(i.get_lastlogouttime()) + " (" + puttimediff(i.get_lastlogouttime()) + ")"), Qt::ToolTipRole);
					   
		QPushButton *action = new QPushButton(i.get_inactive() ? "Unblock" : "Block");
		action->setProperty("id", i.get_id());
		table->setIndexWidget(model->index(model->rowCount() - 1, 10), action);
		connect(action, SIGNAL(clicked()), this, SLOT(action_clicked()));
		row++;
	}
	countLabel->setText(QString::number(model->rowCount()) + " users");
}
