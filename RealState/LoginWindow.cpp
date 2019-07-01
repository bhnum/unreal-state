#include "LoginWindow.h"

LoginWindow::LoginWindow(ContractManager &conManager, QWidget *parent)
	: QMainWindow(parent), conManager(conManager), userManager(conManager.get_userManager()),
	resManager(conManager.get_residenceManager())
{
	setWindowTitle("Log in");

	setMaximumHeight(sizeHint().height());
	setMinimumWidth(300);

	usernameEdit = new QLineEdit();
	passwordEdit = new QLineEdit();
	passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);

	form = new QFormLayout();
	form->addRow("User name: ", usernameEdit);
	form->addRow("Password :", passwordEdit);

	loginButton = new QPushButton("&Log in");
	registerButton = new QPushButton("&Register");

	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->addWidget(loginButton);
	buttonsLayout->addWidget(registerButton);
	buttonsLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

	form->addRow("", buttonsLayout);

	QWidget *widget = new QWidget();
	widget->setLayout(form);
	setCentralWidget(widget);

	connect(loginButton, SIGNAL(clicked()), this, SLOT(login_clicked()));
	connect(registerButton, SIGNAL(clicked()), this, SLOT(register_clicked()));
}

void LoginWindow::login_clicked()
{
	bool s = userManager.authenticate_user(usernameEdit->text().toLower().toStdString(),
		passwordEdit->text().toStdString());

	if (!s)
	{
		QMessageBox mbox;
		mbox.setText("Invalid login attempt");
		mbox.setWindowTitle("Error!");
		mbox.setIcon(QMessageBox::Icon::Information);
		mbox.exec();
		return;
	}

	User *user = userManager.query_user(usernameEdit->text().toLower().toStdString());
	if (user->get_type() == UserType::Admin)
	{
		hide();
		AdminWindow *wnd = new AdminWindow(conManager, user->get_id(), this);
		wnd->show();
		connect(wnd, SIGNAL(closed()), this, SLOT(unhide()));
	}
	else
	{
		hide();
		UserWindow *wnd = new UserWindow(conManager, user->get_id(), this);
		wnd->show();
		connect(wnd, SIGNAL(closed()), this, SLOT(unhide()));
	}
	userManager.update_logintime(lastId = user->get_id());
}

void LoginWindow::loggedout()
{
	userManager.update_logouttime(lastId);
	show();
}

void LoginWindow::register_clicked()
{
	hide();
	RegisterWindow *registerwin = new RegisterWindow(conManager, this);
	registerwin->show();
	connect(registerwin, SIGNAL(closed()), this, SLOT(unhide()));
}

void LoginWindow::unhide()
{
	show();
}
