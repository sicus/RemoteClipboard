#ifndef MAINWINDOW_HEADER
#define MAINWINDOW_HEADER

#include <QMainWindow>
#include <QClipboard>
#include <QApplication>
#include <QTextEdit>

#include "global.h"
#include "ui_MainWindow.h"
#include "NetworkConnection.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0 );
		~MainWindow();

	public slots:
		void clipboardChanged(QClipboard::Mode);
		void startServer();
		void stopServer();
		void connectHost();
		void disconnectHost();
	protected:
		Ui::MainWindow*    m_mw;
		NetworkConnection* m_nc;
		QClipboard*        m_clipboard;
		QString            m_hostname;
};

#endif
