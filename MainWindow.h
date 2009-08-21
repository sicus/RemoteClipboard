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
#include <QTimer>

#include "global.h"
#include "NetworkConnection.h"
#include "RemoteClient.h"

#include "ui_MainWindow.h"
#include "ui_ConnectDlg.h"
#include "ui_StartServerDlg.h"
#include "ui_About.h"


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
		void remoteHostNameChanged(QString name);
		void disconnected();
		void connected(RemoteClient* rc);
		void removeDeleted();
		void about();
		void closeTab(int idx);

	protected:
		Ui::MainWindow*      m_mw;
		NetworkConnection    m_nc;
		QList<RemoteClient*> m_clientList;
		QList<RemoteClient*> m_closedClientList;
		QClipboard*          m_clipboard;
		QString              m_hostname;
		bool                 m_idSend;
		bool                 m_server;
		QTimer               m_timer;

		Ui::StartServerDlg   m_serverDlgUi;
		Ui::ConnectDlg       m_connectDlgUi;
		QDialog              m_serverDlg;
		QDialog              m_connectDlg;

		int                  m_connectionCount;
};

#endif
