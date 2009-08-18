#include <stdio.h>
#include <QApplication>
#include "MainWindow.h"
#include "global.h"

int main(int argc, char** argv)
{
	QApplication app(argc,argv);
	printf("OS: %d\n",OPERATING_SYSTEM);
	MainWindow mw;
	mw.show();
	return app.exec();
}

