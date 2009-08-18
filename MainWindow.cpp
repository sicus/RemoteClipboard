#include "MainWindow.h"


MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags) : QMainWindow(parent,flags)
{
	m_mw = new Ui::MainWindow;
	m_mw->setupUi(this);
	m_nc = new NetworkConnection(this);
	m_clipboard = QApplication::clipboard();
	m_hostname = "Unknown";

	if(OPERATING_SYSTEM != 1)
	{
		m_mw->LocalSelection->hide();
		m_mw->LocalSelectionTitle->hide();
	}

	connect(m_clipboard,SIGNAL(changed(QClipboard::Mode)),this,SLOT(clipboardChanged(QClipboard::Mode)));
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
}

void MainWindow::stopServer()
{
}

void MainWindow::connectHost()
{
}

void MainWindow::disconnectHost()
{
}
