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
	m_port      = port;
	m_server    = false;
	m_clientName = "Unknown";
	connect(&m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnection()));
}

NetworkConnection::~NetworkConnection()
{
	stopServer();
	disconnect(&m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnection()));
}

void NetworkConnection::setClientName(QString name)
{
	m_clientName = name;
}

void NetworkConnection::setPort(int port)
{
	m_port = port;
}

int NetworkConnection::getPort()
{
	return m_port;
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


void NetworkConnection::newConnection()
{
	QTcpSocket* tcpSocket = m_tcpServer.nextPendingConnection();
	if(tcpSocket->open(QIODevice::ReadWrite))
	{
		if(m_clientName.isEmpty())
			m_clientName = "Unknown";
		RemoteClient* rc = new RemoteClient();
		rc->setClientName(m_clientName);
		rc->setSocket(tcpSocket);
		emit incommingConnection(rc);
	}
}

