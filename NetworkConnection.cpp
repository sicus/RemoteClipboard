#include "NetworkConnection.h"

NetworkConnection::NetworkConnection(QObject* parent, int port)
{
	m_tcpSocket = NULL;
	m_port      = port;
	m_server    = false;
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
	return false;
}

bool NetworkConnection::isServerOpen()
{
	return false;
}

bool NetworkConnection::startServer()
{
	if(m_tcpServer.listen(QHostAddress::Any,m_port))
	{
		connect(&m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnection()));
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
	return false;
}

bool NetworkConnection::disconnectFromClient()
{
	if(m_server)
		startServer();
	return false;
}

bool NetworkConnection::sendMessage(unsigned int type, QByteArray data)
{
	return false;
}

void NetworkConnection::newConnection()
{
	m_tcpSocket = m_tcpServer.nextPendingConnection();
	m_tcpServer.close();
}
