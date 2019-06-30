#include "RegisterWindow.h"

RegisterWindow::RegisterWindow(UserManager &userManager, QWidget *parent)
	: MainWindow(parent), userManager(userManager)
{
	setWindowTitle("Register");

	setMaximumHeight(sizeHint().height());
	setMinimumWidth(300);

	usernameEdit = new QLineEdit();
	passwordEdit = new QLineEdit();
	passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);
	nameEdit = new QLineEdit();
	surnameEdit = new QLineEdit();
	balanceEdit = new QSpinBox();
	balanceEdit->setRange(0, 100'000'000'000'000);
	balanceEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
	balanceEdit->setValue(1000'000);

	birthdateEdit = new QDateEdit();
	birthdateEdit->setMaximumDate(QDate::currentDate().addYears(-18));
	birthdateEdit->setDisplayFormat("yyyy-M-d");

	form = new QFormLayout();
	form->addRow("User name: ", usernameEdit);
	form->addRow("Password :", passwordEdit);
	form->addRow("Name: ", nameEdit);
	form->addRow("surname: ", surnameEdit);
	form->addRow("Balance: ", balanceEdit);
	form->addRow("Birthdate: ", birthdateEdit);

	QPushButton *cancelButton = new QPushButton("&Cancel");
	QPushButton *registerButton = new QPushButton("&Register");

	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->addWidget(cancelButton);
	buttonsLayout->addWidget(registerButton);
	buttonsLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

	form->addRow("", buttonsLayout);

	QWidget *widget = new QWidget();
	widget->setLayout(form);
	setCentralWidget(widget);

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel_clicked()));
	connect(registerButton, SIGNAL(clicked()), this, SLOT(register_clicked()));

}

RegisterWindow::~RegisterWindow()
{
}

void RegisterWindow::cancel_clicked()
{
	close();
}

void RegisterWindow::register_clicked()
{
	User user;
	user.set_username(usernameEdit->text().toLower().toStdString());
	user.set_password(passwordEdit->text().toStdString());
	user.set_name(nameEdit->text().toStdString());
	user.set_surname(surnameEdit->text().toStdString());
	user.set_balance(balanceEdit->value());
	user.set_birthdate(todate(birthdateEdit->date().toString(birthdateEdit->displayFormat()).toStdString()));

	try
	{
		userManager.register_user(user);
	}
	catch (const std::exception& ex)
	{
		QMessageBox mbox(this);
		mbox.setText(ex.what());
		mbox.setWindowTitle("Error!");
		mbox.setIcon(QMessageBox::Icon::Warning);
		mbox.exec();
		return;
	}
	QMessageBox mbox(this);
	mbox.setText("Register succeeded");
	mbox.setWindowTitle("!");
	mbox.setIcon(QMessageBox::Icon::Information);
	mbox.exec();
}
