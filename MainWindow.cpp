#include "MainWindow.h"


MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags) : QMainWindow(parent,flags)
{
	m_mw = new Ui::MainWindow;
	m_mw->setupUi(this);
	m_clipboard = QApplication::clipboard();
	m_hostname = "Unknown";

	if(OPERATING_SYSTEM != 1)
	{
		m_mw->LocalSelection->hide();
		m_mw->LocalSelectionTitle->hide();
	}

	connect(m_clipboard,SIGNAL(changed(QClipboard::Mode)),this,SLOT(clipboardChanged(QClipboard::Mode)));
	connect(m_mw->actionStart_Server,SIGNAL(triggered()),this,SLOT(startServer()));
	connect(m_mw->actionS_top_Server,SIGNAL(triggered()),this,SLOT(stopServer()));
	connect(m_mw->action_Connect_to_Server,SIGNAL(triggered()),this,SLOT(connectHost()));
	connect(m_mw->actionDisconnect_from_Server,SIGNAL(triggered()),this,SLOT(disconnectHost()));
}

MainWindow::~MainWindow()
{
	disconnect(m_clipboard,SIGNAL(changed(QClipboard::Mode)),this,SLOT(clipboardChanged(QClipboard::Mode)));
	m_clipboard = NULL;
}

void MainWindow::clipboardChanged(QClipboard::Mode mode)
{
	QTextEdit* textEdit;
	if(mode == QClipboard::Clipboard)
		textEdit = m_mw->LocalClipboard;
	else if(mode == QClipboard::Selection)
		textEdit = m_mw->LocalSelection;
	else
		return;
	
	textEdit->clear();
	textEdit->setText(m_clipboard->text(mode));
}

void MainWindow::startServer()
{
	Ui::StartServerDlg csDlg;
	QDialog dlg;
	csDlg.setupUi(&dlg);
	int port;
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
			QMessageBox::information(NULL,"Server started","Server successfully started");
			m_mw->ServerLabel->setText("Server: Running");
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
}

void MainWindow::connectHost()
{
	Ui::ConnectDlg csDlg;
	QDialog dlg;
	csDlg.setupUi(&dlg);
	if(dlg.exec() == QDialog::Accepted)
	{
		m_hostname = csDlg.HostnameLE->text();
		if(m_hostname.size() <= 0)
			m_hostname = "Unknown";

		m_mw->action_Connect_to_Server->setDisabled(true);
		m_mw->actionDisconnect_from_Server->setDisabled(false);
		QMessageBox::information(NULL,"Connected","Connected");
	}
}

void MainWindow::disconnectHost()
{
	m_mw->action_Connect_to_Server->setDisabled(false);
		m_mw->actionDisconnect_from_Server->setDisabled(true);
}
