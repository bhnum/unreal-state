#include "AdminWindow.h"

AdminWindow::AdminWindow(QWidget *parent)
	: MainWindow(parent)
{
	setWindowTitle("Admin page");

	auto usersModel = new QStandardItemModel();
	usersModel->setColumnCount(11);
	usersModel->setHeaderData(0, Qt::Horizontal, "Id");
	usersModel->setHeaderData(1, Qt::Horizontal, "Username");
	usersModel->setHeaderData(2, Qt::Horizontal, "Name");
	usersModel->setHeaderData(3, Qt::Horizontal, "Surname");
	usersModel->setHeaderData(4, Qt::Horizontal, "Type");
	usersModel->setHeaderData(5, Qt::Horizontal, "Balance");
	usersModel->setHeaderData(6, Qt::Horizontal, "Birthdate");
	usersModel->setHeaderData(7, Qt::Horizontal, "Blocked");
	usersModel->setHeaderData(8, Qt::Horizontal, "Last Login");
	usersModel->setHeaderData(9, Qt::Horizontal, "Last Logout");
	usersModel->setHeaderData(10, Qt::Horizontal, "Actions");

	usersTable = new QTableView();
	usersTable->setModel(usersModel);

	QVBoxLayout *usersLayout = new QVBoxLayout();
	usersLayout->addWidget(usersTable);

	QWidget *usersWidget = new QWidget();
	usersWidget->setLayout(usersLayout);

	tabs = new QTabWidget();
	tabs->addTab(usersWidget, "&Users");
	
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(tabs);

	QWidget *widget = new QWidget();
	widget->setLayout(layout);
	setCentralWidget(widget);
}

AdminWindow::~AdminWindow()
{
}
