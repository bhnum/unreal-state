#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QDialog>
#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QDateEdit>
#include <QTabWidget>
#include <QTableView>
#include <QStandardItemModel>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR) : QMainWindow(parent) {}
	~MainWindow() {}

	virtual void closeEvent(QCloseEvent *event) override
	{
		QMainWindow::closeEvent(event);
		emit(closed());
	}

signals:
	void closed();

private:
};
