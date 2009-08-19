/*****************************
** File: MainWindow.cpp
** Author: Markus Biggel
** Date: 18.08.2009
** Project: Remote Clipboard
** Licence: GPL-V3
******************************/

#include "MainWindow.h"


MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags) : QMainWindow(parent,flags)
{
	m_mw = new Ui::MainWindow;
	m_mw->setupUi(this);
	m_clipboard = QApplication::clipboard();
	m_hostname = "Unknown";
	m_idSend = false;
	m_server = false;

	if(OPERATING_SYSTEM == OS_WIN32)
	{
		m_mw->LocalSelection->hide();
		m_mw->LocalSelectionTitle->hide();
	}

	connect(m_clipboard,SIGNAL(changed(QClipboard::Mode)),this,SLOT(clipboardChanged(QClipboard::Mode)));
	connect(m_mw->actionStart_Server,SIGNAL(triggered()),this,SLOT(startServer()));
	connect(m_mw->actionS_top_Server,SIGNAL(triggered()),this,SLOT(stopServer()));
	connect(m_mw->action_Connect_to_Server,SIGNAL(triggered()),this,SLOT(connectHost()));
	connect(m_mw->actionDisconnect_from_Server,SIGNAL(triggered()),this,SLOT(disconnectHost()));
	connect(m_mw->CopySelectionButton,SIGNAL(clicked()),this,SLOT(copySelectionToClipboard()));
	connect(m_mw->CopyClipboardButton,SIGNAL(clicked()),this,SLOT(copyClipboardToClipboard()));

	connect(&m_nc,SIGNAL(disconnectedFromHost()),this,SLOT(disconnected()));
	connect(&m_nc,SIGNAL(connectedToHost()),this,SLOT(connected()));
}

MainWindow::~MainWindow()
{
	disconnect(m_clipboard,SIGNAL(changed(QClipboard::Mode)),this,SLOT(clipboardChanged(QClipboard::Mode)));
	m_clipboard = NULL;
}

void MainWindow::clipboardChanged(QClipboard::Mode mode)
{
	QTextEdit* textEdit;
	QByteArray text = m_clipboard->text(mode).toLocal8Bit();
	if(mode == QClipboard::Clipboard)
	{
		textEdit = m_mw->LocalClipboard;
		m_nc.sendMessage(TT_CLIPBOARD_ENTRY,text);
	}
	else if(mode == QClipboard::Selection)
	{
		textEdit = m_mw->LocalSelection;
		m_nc.sendMessage(TT_SELECTION_ENTRY,text);
	}
	else
		return;
	
	textEdit->clear();
	textEdit->setText(QString(text));
}

void MainWindow::startServer()
{
	Ui::StartServerDlg csDlg;
	QDialog dlg;
	csDlg.setupUi(&dlg);
	int port;
	csDlg.PortLE->setText(QString::number(DEFAULT_PORT));
	if(dlg.exec() == QDialog::Accepted)
	{
		m_hostname = csDlg.HostnameLE->text();
		if(m_hostname.size() <= 0)
			m_hostname = "Unknown";

		if(csDlg.PortLE->text().isEmpty())
			port = DEFAULT_PORT;
		else
		{
			bool ok;
			port = csDlg.PortLE->text().toInt(&ok);
			if(!ok)
				port = DEFAULT_PORT;
		}

		m_mw->actionStart_Server->setDisabled(true);
		m_mw->actionS_top_Server->setDisabled(false);

		m_nc.setPort(port);

		if(m_nc.startServer())
		{
			m_mw->ServerLabel->setText("Server: Running");
			connect(&m_nc,SIGNAL(messageRecived(QString,int,int)),this,SLOT(messageRecived(QString,int,int)));
			m_server = true;
		}
		else
		{
			QMessageBox::critical(NULL,"Server not started","Server not started");
			m_mw->ServerLabel->setText("Server: Not Running");
		}
	}
}

void MainWindow::stopServer()
{
	m_mw->actionStart_Server->setDisabled(false);
	m_mw->actionS_top_Server->setDisabled(true);
	m_nc.stopServer();
	m_mw->ServerLabel->setText("Server: Not Running");
	m_server = false;
	m_mw->action_Connect_to_Server->setDisabled(false);
}

void MainWindow::connectHost()
{
	Ui::ConnectDlg csDlg;
	QDialog dlg;
	csDlg.setupUi(&dlg);
	int port;
	csDlg.PortLE->setText(QString::number(DEFAULT_PORT));
	if(dlg.exec() == QDialog::Accepted)
	{
		m_hostname = csDlg.ClientnameLE->text();
		QString host = csDlg.HostLE->text();
		if(m_hostname.size() <= 0)
			m_hostname = "Unknown";

		if(csDlg.PortLE->text().isEmpty())
			port = DEFAULT_PORT;
		else
		{
			bool ok;
			port = csDlg.PortLE->text().toInt(&ok);
			if(!ok)
				port = DEFAULT_PORT;
		}

		if(m_nc.connectToClient(host,port))
		{
			m_mw->action_Connect_to_Server->setDisabled(true);
			m_mw->actionDisconnect_from_Server->setDisabled(false);
			m_mw->actionStart_Server->setDisabled(true);
			m_mw->actionS_top_Server->setDisabled(true);
			connect(&m_nc,SIGNAL(messageRecived(QString,int,int)),this,SLOT(messageRecived(QString,int,int)));
			m_nc.sendMessage(TT_LOGIN,m_hostname.toLocal8Bit());
			m_idSend = true;
		}
	}
}

void MainWindow::disconnectHost()
{
	disconnect(&m_nc,SIGNAL(messageRecived(QString,int,int)),this,SLOT(messageRecived(QString,int,int)));
	m_nc.disconnectFromClient();
}

void MainWindow::messageRecived(QString data, int type, int os)
{
	if(type == TT_LOGIN)
	{
		m_mw->RemoteTextLabel->setText("Remote Host: "+data);
		if(m_idSend == false)
		{
			m_nc.sendMessage(TT_LOGIN,m_hostname.toLocal8Bit());
		}

		if(os == OS_WIN32)
		{
			m_mw->RemoteSelection->hide();
			m_mw->RemoteSelectionTitle->hide();
		}
	}
	else if(type == TT_CLIPBOARD_ENTRY)
	{
		m_mw->RemoteClipboard->setText(data);
	}
	else if(type == TT_SELECTION_ENTRY)
	{
		m_mw->RemoteSelection->setText(data);
	}
}

void MainWindow::disconnected()
{
	m_mw->action_Connect_to_Server->setDisabled(m_server);
	m_mw->actionDisconnect_from_Server->setDisabled(true);
	m_mw->actionStart_Server->setDisabled(m_server);
	m_mw->actionS_top_Server->setDisabled(!m_server);
	
	m_mw->RemoteTextLabel->setText("Remote Host");
	m_idSend = false;
	m_mw->RemoteSelection->show();
	m_mw->RemoteSelectionTitle->show();
	m_mw->CopyClipboardButton->setEnabled(false);
	m_mw->CopySelectionButton->setEnabled(false);
}

void MainWindow::connected()
{
	m_mw->CopyClipboardButton->setEnabled(true);
	m_mw->CopySelectionButton->setEnabled(true);

	m_mw->action_Connect_to_Server->setDisabled(true);
	m_mw->actionDisconnect_from_Server->setDisabled(false);
	m_mw->actionStart_Server->setDisabled(true);
	m_mw->actionS_top_Server->setDisabled(true);
}

void MainWindow::copySelectionToClipboard()
{
	m_clipboard->setText(m_mw->RemoteSelection->toPlainText());
}

void MainWindow::copyClipboardToClipboard()
{
	m_clipboard->setText(m_mw->RemoteClipboard->toPlainText());
}
