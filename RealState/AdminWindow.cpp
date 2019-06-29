#include "AdminWindow.h"

AdminWindow::AdminWindow(UserManager& userman,QWidget *parent)
	: MainWindow(parent),userman(userman)
{
	setWindowTitle("Admin page");

	UsermanWindow *usersWidget = new UsermanWindow(userman);
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
