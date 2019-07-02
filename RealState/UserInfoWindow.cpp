#include "UserInfoWindow.h"

UserInfoWindow::UserInfoWindow(UserManager &userManager, int id, QWidget *parent)
	: MainWindow(parent), userManager(userManager), id(id)
{
	User *user = userManager.query_user(id);

	setWindowTitle("User Information");
	setMaximumHeight(sizeHint().height());
	setMinimumWidth(350);
	usernameEdit = new QLineEdit(QString::fromStdString(user->get_username()));
	passwordEdit = new QLineEdit();
	passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);
	nameEdit = new QLineEdit(QString::fromStdString(user->get_name()));
	surnameEdit = new QLineEdit(QString::fromStdString(user->get_surname()));
	balanceEdit = new QSpinBox();
	balanceEdit->setRange(0, 100'000'000'000'000);
	balanceEdit->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
	balanceEdit->setValue(user->get_balance());

	birthdateEdit = new QDateEdit();
	birthdateEdit->setMaximumDate(QDate::currentDate().addYears(-18));
	birthdateEdit->setDisplayFormat("yyyy-M-d");
	birthdateEdit->setDate(QDate::fromString(QString::fromStdString(putdate(user->get_birthdate())), birthdateEdit->displayFormat()));

	form = new QFormLayout();
	form->addRow("User name: ", usernameEdit);
	form->addRow("New password :", passwordEdit);
	form->addRow("Name: ", nameEdit);
	form->addRow("surname: ", surnameEdit);
	form->addRow("Balance: ", balanceEdit);
	form->addRow("Birth date: ", birthdateEdit);

	cancelButton = new QPushButton("&Cancel");
	doneButton = new QPushButton("&Done");

	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->addWidget(cancelButton);
	buttonsLayout->addWidget(doneButton);
	buttonsLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

	form->addRow("", buttonsLayout);

	QWidget *widget = new QWidget();
	widget->setLayout(form);
	setCentralWidget(widget);

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel_clicked()));
	connect(doneButton, SIGNAL(clicked()), this, SLOT(done_clicked()));
}

UserInfoWindow::~UserInfoWindow()
{
}

void UserInfoWindow::cancel_clicked()
{
	close();
}

void UserInfoWindow::done_clicked()
{
	User user = *userManager.query_user(id);
	user.set_username(usernameEdit->text().toLower().toStdString());
	if (!passwordEdit->text().isEmpty())
		user.set_password(passwordEdit->text().toStdString());
	user.set_name(nameEdit->text().toStdString());
	user.set_surname(surnameEdit->text().toStdString());
	user.set_balance(balanceEdit->value());
	user.set_birthdate(todate(birthdateEdit->text().toStdString()));

	try
	{
		userManager.edit_user(user);
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
	mbox.setText("Your infornation has successfully changed.");
	mbox.setWindowTitle("Informaion changed!");
	mbox.setIcon(QMessageBox::Icon::Information);
	mbox.exec();
	close();
}
