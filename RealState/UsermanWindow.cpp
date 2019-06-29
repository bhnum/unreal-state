#include "UsermanWindow.h"

UsermanWindow::UsermanWindow(UserManager& userman ,QWidget *parent)
	: QWidget(parent),userman(userman)
{

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


	usersTable = new QTableView();
	usersTable->setModel(model);

	QVBoxLayout *usersLayout = new QVBoxLayout();
	usersLayout->addWidget(usersTable);
	setLayout(usersLayout);
	
	usersTable->setSortingEnabled(true);
	usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	populate();
}
void UsermanWindow::action_clicked() {
	int id = sender()->property("id").value<int>();
	User* user = userman.query_user(id);
user->set_inactive(!user->get_inactive());
userman.save();
populate();
}
UsermanWindow::~UsermanWindow()
{
}
void UsermanWindow::populate() {
	
	for (int i = 0; i < model->rowCount(); i++)
	{
		auto tmp = usersTable->indexWidget(model->index(i, 10));
		usersTable->setIndexWidget(model->index(i, 10),NULL);
		delete tmp;

	}
	model->removeRows(0, model->rowCount());
	int row = 0;
	
	for each (auto i in userman.get_users())
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
		model->setData(model->index(row, 8), QString::fromStdString(puttime(i.get_lastlogintime())), Qt::DisplayRole);
		model->setData(model->index(row, 9), QString::fromStdString(puttime(i.get_lastlogouttime())), Qt::DisplayRole);
					   



		QPushButton * action = new QPushButton(i.get_inactive() ? "Unblock" : "Block");
		usersTable->setIndexWidget(model->index(model->rowCount() - 1, 10), action);
		row++;
		action->setProperty("id", i.get_id());
		connect(action, SIGNAL(clicked()), this, SLOT(action_clicked()));
	}
}