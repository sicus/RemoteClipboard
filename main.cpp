/*****************************
** File: main.cpp
** Author: Markus Biggel
** Date: 18.08.2009
** Project: Remote Clipboard
** Licence: GPL-V3
******************************/


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

