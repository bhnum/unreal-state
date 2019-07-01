#include "UserWindow.h"

UserWindow::UserWindow(ContractManager &conManager, int userId, QWidget *parent)
	: MainWindow(parent), conManager(conManager), userManager(conManager.get_userManager()),
	resManager(conManager.get_residenceManager()), userId(userId)
{
	setWindowTitle("User page");

	QMenu *changeinfoButton = menuBar()->addMenu("Edit user &information");

	tabs = new QTabWidget();

	ressTab = new ResidencesTab(conManager, userId);
	tabs->addTab(ressTab, "&Residences");

	consTab = new ContractsTab(conManager, userId);
	tabs->addTab(consTab, "Your &contracts");

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(tabs);

	QWidget *widget = new QWidget();
	widget->setLayout(layout);
	setCentralWidget(widget);

	connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tab_changed(int)));
	connect(changeinfoButton, SIGNAL(triggered(QAction*)), this, SLOT(changeinfo_clicked()));
}

UserWindow::~UserWindow()
{
}

void UserWindow::changeinfo_clicked(QAction *)
{
	auto wnd = new UserInfoWindow(userManager, userId, this);
	wnd->setWindowModality(Qt::ApplicationModal);
	wnd->show();
}

void UserWindow::tab_changed(int index)
{
	switch (index)
	{
	case 0:
		ressTab->populate();
		break;
	case 1:
		consTab->populate();
		break;
	default:
		throw std::logic_error("");
		break;
	}
}
