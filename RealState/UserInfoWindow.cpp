#include "UserInfoWindow.h"
#include <QMainWindow>
UserInfoWindow::UserInfoWindow(UserManager &userManager,int id,QWidget *parent)
	: QMainWindow(parent),userManager(userManager)
{
	setWindowTitle("User Info");
	setMaximumHeight(sizeHint().height());
	setMinimumWidth(350);
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
	form->addRow("Birth date: ", birthdateEdit);
	usernameEdit->setText(QString::fromStdString(userManager.query_user(id)->get_username()));
//	passwordEdit->setText(QString::fromStdString(userManager.query_user(id)->g()));
	nameEdit->setText(QString::fromStdString(userManager.query_user(id)->get_name()));
	surnameEdit->setText(QString::fromStdString(userManager.query_user(id)->get_username()));
	//QDate date = (QDate)userManager.query_user(id)->get_birthdate();
	balanceEdit->setValue(userManager.query_user(id)->get_balance());

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


}

void UserInfoWindow::cancel_clicked()
{
this->close();
}
void  UserInfoWindow::done_clicked() {
	userManager.query_user(id)->set_balance(balanceEdit->value());
	//userManager.query_user(id)->set_username(QString::fromStdString(usernameEdit->text()));
	//userManager.query_user(id)->set_balance(balanceEdit->value());
	//userManager.query_user(id)->set_balance(balanceEdit->value());
	//userManager.query_user(id)->set_balance(balanceEdit->value());

}
UserInfoWindow::~UserInfoWindow()
{
}
