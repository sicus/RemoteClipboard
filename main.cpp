#include <stdio.h>
#include <QApplication>
#include "global.h"
#include "MainWindow.h"

int main(int argc, char** argv)
{
	QApplication app(argc,argv);
	MainWindow mw;
	mw.show();
	return app.exec();
}

