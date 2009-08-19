/*****************************
** File: MainWindow.h
** Author: Markus Biggel
** Date: 18.08.2009
** Project: Remote Clipboard
** Licence: GPL-V3
******************************/

#ifndef MAINWINDOW_HEADER
#define MAINWINDOW_HEADER

#include <QMainWindow>
#include <QClipboard>
#include <QApplication>
#include <QTextEdit>
#include <QDialog>
#include <QMessageBox>

#include "global.h"
#include "NetworkConnection.h"

#include "ui_MainWindow.h"
#include "ui_ConnectDlg.h"
#include "ui_StartServerDlg.h"


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
		void messageRecived(QString data, int type, int os);
		void disconnected();
		void connected();
		void copySelectionToClipboard();
		void copyClipboardToClipboard();

	protected:
		Ui::MainWindow*    m_mw;
		NetworkConnection  m_nc;
		QClipboard*        m_clipboard;
		QString            m_hostname;
		bool               m_idSend;
		bool               m_server;

		Ui::StartServerDlg m_serverDlgUi;
		Ui::ConnectDlg     m_connectDlgUi;
		QDialog            m_serverDlg;
		QDialog            m_connectDlg;
};

#endif
