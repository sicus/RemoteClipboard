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
	m_connectionCount = 0;

	m_timer.setInterval(500);

	m_serverDlgUi.setupUi(&m_serverDlg);
	m_connectDlgUi.setupUi(&m_connectDlg);

	if(OPERATING_SYSTEM == OS_WIN32)
	{
		m_mw->LocalSelection->hide();
		m_mw->LocalSelectionTitle->hide();
	}

	m_mw->actionDisconnect_from_Server->setDisabled(true);

	connect(m_mw->action_About,SIGNAL(triggered()),this,SLOT(about()));
	
	connect(m_clipboard,SIGNAL(changed(QClipboard::Mode)),this,SLOT(clipboardChanged(QClipboard::Mode)));
	connect(m_mw->actionStart_Server,SIGNAL(triggered()),this,SLOT(startServer()));
	connect(m_mw->actionS_top_Server,SIGNAL(triggered()),this,SLOT(stopServer()));
	connect(m_mw->action_Connect_to_Server,SIGNAL(triggered()),this,SLOT(connectHost()));

	connect(&m_nc,SIGNAL(incommingConnection(RemoteClient*)),this,SLOT(connected(RemoteClient*)));

	connect(&m_timer,SIGNAL(timeout()),this,SLOT(removeDeleted()));
	connect(m_mw->ClientTabs,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
	m_timer.start();
}

MainWindow::~MainWindow()
{
	disconnect(m_clipboard,SIGNAL(changed(QClipboard::Mode)),this,SLOT(clipboardChanged(QClipboard::Mode)));
	disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(removeDeleted()));
	m_timer.stop();
	RemoteClient* rc;

	for(int i=0;i<m_clientList.size();i++)
	{
		rc = m_clientList.value(i);
		rc->disconnectFromClient();
	}
	m_nc.stopServer();
	removeDeleted();
}

void MainWindow::closeTab(int idx)
{
	RemoteClient* rc = NULL;
	QWidget* wid = m_mw->ClientTabs->widget(idx);
	for(int i=0;i<m_clientList.size();i++)
	{
		if(m_clientList.value(i)->getWidget() == wid)
		{
			rc = m_clientList.value(i);
			break;
		}
	}
	if(rc != NULL && !strcmp(rc->metaObject()->className(),"RemoteClient"))
		rc->disconnectFromClient();
}

void MainWindow::clipboardChanged(QClipboard::Mode mode)
{
	QTextEdit* textEdit;
	QByteArray text = m_clipboard->text(mode).toLocal8Bit();
	if(mode == QClipboard::Clipboard)
	{
		textEdit = m_mw->LocalClipboard;
	}
	else if(mode == QClipboard::Selection)
	{
		textEdit = m_mw->LocalSelection;
	}
	else
		return;
	
	textEdit->clear();
	textEdit->setText(QString(text));
}

void MainWindow::startServer()
{
	
	int port;
	m_serverDlgUi.PortLE->setText(QString::number(DEFAULT_PORT));
	if(m_serverDlg.exec() == QDialog::Accepted)
	{
		m_hostname = m_serverDlgUi.HostnameLE->text();
		if(m_hostname.size() <= 0)
			m_hostname = "Unknown";

		if(m_serverDlgUi.PortLE->text().isEmpty())
			port = DEFAULT_PORT;
		else
		{
			bool ok;
			port = m_serverDlgUi.PortLE->text().toInt(&ok);
			if(!ok)
				port = DEFAULT_PORT;
		}

		m_mw->actionStart_Server->setDisabled(true);
		m_mw->actionS_top_Server->setDisabled(false);

		m_nc.setPort(port);
		m_nc.setClientName(m_hostname);

		if(m_nc.startServer())
		{
			m_mw->ServerLabel->setText("Server: Running");
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
	int port;

	m_connectDlgUi.PortLE->setText(QString::number(DEFAULT_PORT));
	
	if(m_connectDlg.exec() == QDialog::Accepted)
	{
		if(m_connectDlgUi.PortLE->text().isEmpty())
			port = DEFAULT_PORT;
		else
		{
			bool ok;
			port = m_connectDlgUi.PortLE->text().toInt(&ok);
			if(!ok)
				port = DEFAULT_PORT;
		}
		
		m_hostname = m_connectDlgUi.ClientnameLE->text();
		QString host = m_connectDlgUi.HostLE->text();
		if(m_hostname.size() <= 0)
			m_hostname = "Unknown";

		RemoteClient* rc = new RemoteClient();
		rc->setClientName(m_hostname);
		rc->connectToClient(host,port);
		m_clientList.append(rc);
		connect(rc,SIGNAL(remoteNameChanged(QString)),this,SLOT(remoteHostNameChanged(QString)));
		connect(rc,SIGNAL(connectionClosed()),this,SLOT(disconnected()));
		if(m_connectionCount == 0)
		{
			m_mw->ClientTabs->addTab(rc->getWidget(),rc->getRemoteName());
			m_mw->ClientTabs->removeTab(0);
			m_connectionCount++;
		}
		else
		{
			m_mw->ClientTabs->addTab(rc->getWidget(),rc->getRemoteName());
			m_connectionCount++;
		}
	}
}

void MainWindow::disconnected()
{
	RemoteClient* rc = (RemoteClient*)sender();
	m_closedClientList.append(rc);
}

void MainWindow::removeDeleted()
{
	int idx;
	RemoteClient* rc;
	while(m_closedClientList.size()>0)
	{
		rc = m_closedClientList.takeFirst();
		idx = m_clientList.indexOf(rc);
		m_clientList.removeAt(idx);
		m_connectionCount--;
		if(m_connectionCount <= 0)
		{
			m_connectionCount = 0;
			m_mw->ClientTabs->addTab(new QWidget(),"No Connection");
		}
		m_mw->ClientTabs->removeTab(m_mw->ClientTabs->indexOf(rc->getWidget()));
		delete rc;
	}
	m_timer.start();
}

void MainWindow::remoteHostNameChanged(QString name)
{
	int idx = m_mw->ClientTabs->indexOf( ((RemoteClient*)sender())->getWidget() );
	m_mw->ClientTabs->setTabText(idx,name);
}

void MainWindow::connected(RemoteClient* rc)
{
	connect(rc,SIGNAL(remoteNameChanged(QString)),this,SLOT(remoteHostNameChanged(QString)));
	connect(rc,SIGNAL(connectionClosed()),this,SLOT(disconnected()));
	m_clientList.append(rc);
	if(m_connectionCount == 0)
	{
		m_mw->ClientTabs->addTab(rc->getWidget(),rc->getRemoteName());
		m_mw->ClientTabs->removeTab(0);
		m_connectionCount++;
	}
	else
	{
		m_mw->ClientTabs->addTab(rc->getWidget(),rc->getRemoteName());
		m_connectionCount++;
	}
}

void MainWindow::about()
{
	QDialog dlg;
	Ui::AboutDialog adlg;
	adlg.setupUi(&dlg);
	dlg.exec();
}

