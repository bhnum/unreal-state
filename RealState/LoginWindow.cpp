#include "LoginWindow.h"

LoginWindow::LoginWindow(UserManager &userManager, QWidget *parent)
	: QMainWindow(parent), userManager(userManager)
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

	QMessageBox mbox;
	mbox.setText(s ? "Register succeeded" : "NO!");
	mbox.setWindowTitle("!");
	mbox.setIcon(QMessageBox::Icon::Information);
	mbox.exec();
}

void LoginWindow::register_clicked()
{
	hide();
	RegisterWindow *registerwin = new RegisterWindow(userManager, this);
	registerwin->show();
	connect(registerwin, SIGNAL(closed()), this, SLOT(register_closed()));
}

void LoginWindow::register_closed()
{
	show();
}
