#include "MainWindow.h"


MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags) : QMainWindow(parent,flags)
{
	m_mw = new Ui::MainWindow;
	m_mw->setupUi(this);
}
