/*****************************
** File: NetworkConnection.h
** Author: Markus Biggel
** Date: 18.08.2009
** Project: Remote Clipboard
** Licence: GPL-V3
******************************/

#include "RemoteClient.h"


RemoteClient::RemoteClient(QObject* parent) : QObject(parent)
{
	m_identificationSend = false;
	m_tcpSocket = NULL;
	m_clipboard = QApplication::clipboard();
	m_remoteName = "Unknown";
	m_clientWidget = new Ui::ClientTab();
	m_clientWidget->setupUi(&m_widget);

	ClipboardPublicControl* cpc = ClipboardPublicControl::getInstance();
	
	connect(cpc,SIGNAL(changed(QClipboard::Mode)),this,SLOT(clipboardChanged(QClipboard::Mode)));
	connect(m_clientWidget->CopyClipboardButton,SIGNAL(clicked()),this,SLOT(copyRemoteClipboard()));
	connect(m_clientWidget->CopySelectionButton,SIGNAL(clicked()),this,SLOT(copyRemoteSelection()));
}

QWidget* RemoteClient::getWidget()
{
	return &m_widget;
}

void RemoteClient::clipboardChanged(QClipboard::Mode mode)
{
	QByteArray text = m_clipboard->text(mode).toLocal8Bit();
	if(mode == QClipboard::Clipboard)
	{
		sendMessage(text,TT_CLIPBOARD_ENTRY);
	}
	else if(mode == QClipboard::Selection)
	{
		sendMessage(text,TT_SELECTION_ENTRY);
	}
}

void RemoteClient::disconnectFromClient()
{
	disconnect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(dataResceived()));
	disconnect(m_tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
	if(m_tcpSocket)
	{
		if(m_tcpSocket->isOpen())
			m_tcpSocket->close();
		delete m_tcpSocket;
		m_tcpSocket = NULL;
	}
	disconnect(m_clipboard,SIGNAL(changed(QClipboard::Mode)),this,SLOT(clipboardChanged(QClipboard::Mode)));
	emit connectionClosed();
}

void RemoteClient::setClientName(QString name)
{
	m_clientName = name;
}

QString RemoteClient::getRemoteName()
{
	return m_remoteName;
}

void RemoteClient::dataResceived()
{
	static bool stillReading = false;
	static RC_ProtocolHeader header;
	static unsigned int bytesRead = 0;
	static QString data;
	fflush(stdout);
	if(!m_tcpSocket)
		return;

	if(m_tcpSocket->bytesAvailable() >= sizeof(RC_ProtocolHeader) && !stillReading)
	{
		m_tcpSocket->readLine((char*)&header,sizeof(RC_ProtocolHeader)+1);
		bytesRead = 0;
		if( memcmp(header.ProtocolIdentifier,PROTOCOL_VERSION,5) )
		{
			disconnectFromClient();
			return;
		}
	}
		
	if(header.DataSize > 0)
	{
		while(m_tcpSocket->bytesAvailable() > 0)
		{
			unsigned int maxRead = m_tcpSocket->bytesAvailable();
			if(maxRead > (header.DataSize-bytesRead) )
				maxRead = header.DataSize-bytesRead;

			char* dat = new char[header.DataSize+2];
			bytesRead += m_tcpSocket->readLine(dat,maxRead+1);
			data.append(dat);
			delete dat;


			if(bytesRead == header.DataSize)
			{
				stillReading = false;
				bytesRead = 0;
				header.DataSize = 0;
				if(header.TransmissionType == TT_LOGIN)
				{
					m_remoteName = data;
					if(header.OperatingSystem != OS_LINUX)
					{
						m_clientWidget->RemoteSelectionTitle->hide();
						m_clientWidget->RemoteSelection->hide();
						m_clientWidget->CopySelectionButton->hide();
					}
					if(!m_identificationSend)
					{
						sendMessage(m_clientName.toLocal8Bit(),TT_LOGIN);
						m_identificationSend = true;
					}
					emit remoteNameChanged(m_remoteName);
				}
				else if(header.TransmissionType == TT_CLIPBOARD_ENTRY)
				{
					m_clientWidget->RemoteClipboard->setText(data);
				}
				else if(header.TransmissionType == TT_SELECTION_ENTRY)
				{
					m_clientWidget->RemoteSelection->setText(data);
				}
				data.clear();
				break;
			}
			else
				stillReading = true;
		}
	}
}

void RemoteClient::disconnected()
{
	disconnectFromClient();
}

void RemoteClient::copyRemoteClipboard()
{
	m_clipboard->setText(m_clientWidget->RemoteClipboard->toPlainText());
}

void RemoteClient::copyRemoteSelection()
{
	m_clipboard->setText(m_clientWidget->RemoteSelection->toPlainText());
}

void RemoteClient::sendMessage(QByteArray data, int type)
{
	if(!m_tcpSocket || !m_tcpSocket->isOpen())
		return;
	RC_ProtocolHeader header;
	header.OperatingSystem  = OPERATING_SYSTEM;
	header.DataSize         = data.size();
	header.TransmissionType = type;
	memcpy(header.ProtocolIdentifier,PROTOCOL_VERSION,5);
	QByteArray toSend;
	toSend.clear();
	toSend.append((char*)&header,sizeof(RC_ProtocolHeader));
	toSend.append(data);
	m_tcpSocket->write(toSend.data(),toSend.size());
	m_tcpSocket->flush();
	fflush(stdout);
	return;
}

bool RemoteClient::connectToClient(QString host, int port)
{
	if(host.isEmpty())
		return false;

	if(!m_tcpSocket)
	{
		m_tcpSocket = new QTcpSocket();
		m_tcpSocket->connectToHost(host,port);
		if(!m_tcpSocket->waitForConnected())
		{
			if(m_tcpSocket && m_tcpSocket->isOpen())
				m_tcpSocket->close();
			disconnect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(dataResceived()));
			disconnect(m_tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
			delete m_tcpSocket;
			m_tcpSocket = NULL;
			return false;
		}
		else
		{
			if(m_tcpSocket->open(QIODevice::ReadWrite))
			{
				connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(dataResceived()));
				connect(m_tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
				sendMessage(m_clientName.toLocal8Bit(),TT_LOGIN);
				m_identificationSend = true;
				return true;
			}
			else
			{
				if(m_tcpSocket->isOpen())
					m_tcpSocket->close();
				delete m_tcpSocket;
				m_tcpSocket = NULL;
			}
		}
	}

	return false;
}

void RemoteClient::setSocket(QTcpSocket* socket)
{
	m_tcpSocket = socket;
	connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(dataResceived()));
	connect(m_tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
}

