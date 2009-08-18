#ifndef MAINWINDOW_HEADER
#define MAINWINDOW_HEADER

#include "ui_MainWindow.h"
#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0 );

	protected:
		Ui::MainWindow* m_mw;
};

#endif
