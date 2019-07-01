#include "InfoResidenceWindow.h"

InfoResidenceWindow::InfoResidenceWindow(ContractManager &conManager, int userId, int resId, QWidget *parent)
	: MainWindow(parent), conManager(conManager), userManager(conManager.get_userManager()),
	resManager(conManager.get_residenceManager()), userId(userId), resId(resId)
{
	admin = userManager.query_user(userId)->get_type() == UserType::Admin;


}

InfoResidenceWindow::~InfoResidenceWindow()
{
}
