/*****************************
** File: NetworkConnection.cpp
** Author: Markus Biggel
** Date: 18.08.2009
** Project: Remote Clipboard
** Licence: GPL-V3
******************************/

#include "NetworkConnection.h"

NetworkConnection::NetworkConnection(QObject* parent, int port)
{
	m_tcpSocket = NULL;
	m_port      = port;
	m_server    = false;
	connect(&m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnection()));
}

NetworkConnection::~NetworkConnection()
{
// 	if(m_tcpSocket && m_tcpSocket->isOpen())
// 	{
// 		disconnect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(dataResceived()));
// 		disconnect(m_tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
// 		m_tcpSocket->close();
// 	}
// 	if(m_tcpSocket)
// 	{
// 		delete m_tcpSocket;
// 		m_tcpSocket = NULL;
// 	}
	disconnectFromClient();
	stopServer();
	disconnect(&m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnection()));
}

void NetworkConnection::setPort(int port)
{
	m_port = port;
}

int NetworkConnection::getPort()
{
	return m_port;
}

bool NetworkConnection::hasClientConnection()
{
	if(m_tcpSocket && m_tcpSocket->isOpen())
		return true;
	return false;
}

bool NetworkConnection::isServerOpen()
{
	return m_tcpServer.isListening();
}

bool NetworkConnection::startServer()
{
	if(!m_tcpServer.isListening() && m_tcpServer.listen(QHostAddress::Any,m_port))
	{
		m_server = true;
		return true;
	}
	else
		return false;
}

bool NetworkConnection::stopServer()
{
	m_tcpServer.close();
	m_server = false;
	return false;
}

bool NetworkConnection::connectToClient(QString host, int port)
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
				m_tcpServer.close();
				connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(dataResceived()));
				connect(m_tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
				emit connectedToHost();
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

bool NetworkConnection::disconnectFromClient()
{
	bool result = false;

	if(m_tcpSocket)
	{
		disconnect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(dataResceived()));
		disconnect(m_tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
		
		if(m_tcpSocket->isOpen())
			m_tcpSocket->close();
		delete m_tcpSocket;
		m_tcpSocket = NULL;
		result = true;
	}
	emit disconnectedFromHost();
	
	if(m_server)
		startServer();
	
	return result;
}


bool NetworkConnection::sendMessage(unsigned int type, QByteArray data)
{
	if(!m_tcpSocket || !m_tcpSocket->isOpen())
		return false;
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
	return true;
}

void NetworkConnection::newConnection()
{
	m_tcpSocket = m_tcpServer.nextPendingConnection();
	if(m_tcpSocket->open(QIODevice::ReadWrite))
	{
		m_tcpServer.close();
		connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(dataResceived()));
		connect(m_tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
		emit connectedToHost();
	}
}

void NetworkConnection::dataResceived()
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
				emit messageRecived(data,header.TransmissionType,header.OperatingSystem);
				stillReading = false;
				data.clear();
				bytesRead = 0;
				header.DataSize = 0;
				break;
			}
			else
				stillReading = true;
		}
	}
}

void NetworkConnection::disconnected()
{
	disconnectFromClient();
}

