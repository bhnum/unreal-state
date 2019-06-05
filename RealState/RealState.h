#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RealState.h"

class RealState : public QMainWindow
{
	Q_OBJECT

public:
	RealState(QWidget *parent = Q_NULLPTR);

private:
	Ui::RealStateClass ui;
};
