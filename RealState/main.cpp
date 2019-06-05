#include "RealState.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RealState w;
	w.show();
	return a.exec();
}
