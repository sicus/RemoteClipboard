#include "NetworkConnection.h"

NetworkConnection::NetworkConnection(QObject* parent, int port)
{
	m_tcpServer = NULL;
	m_tcpSocket = NULL;
	m_port      = port;
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
	return false;
}

bool NetworkConnection::stopServer()
{
	return false;
}

bool NetworkConnection::connectToClient(QString host, int port)
{
	return false;
}

bool NetworkConnection::disconnectFromClient()
{
	return false;
}

bool NetworkConnection::sendMessage(unsigned int type, QByteArray data)
{
	return false;
}
