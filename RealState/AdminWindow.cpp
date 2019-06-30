#include "AdminWindow.h"

AdminWindow::AdminWindow(UserManager& userManager, int userId, QWidget *parent)
	: MainWindow(parent), userId(userId), userManager(userManager)
{
	setWindowTitle("Admin page");

	UserTab *usersWidget = new UserTab(userManager);
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
