/*****************************
** File: NetworkConnection.h
** Author: Markus Biggel
** Date: 18.08.2009
** Project: Remote Clipboard
** Licence: GPL-V3
******************************/

#ifndef NETWORK_CONNECTION_HEADER
#define NETWORK_CONNECTION_HEADER

#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>

#include "global.h"
#include "RemoteClient.h"

class NetworkConnection : public QObject
{
	Q_OBJECT
	public:
		NetworkConnection(QObject* parent = 0, int port = DEFAULT_PORT);
		~NetworkConnection();
		void setPort(int port);
		int getPort();
		void setClientName(QString name);
		bool isServerOpen();
		bool startServer();
		bool stopServer();

	public slots:
		void newConnection();

	signals:
		void incommingConnection(RemoteClient*);

	protected:
		QTcpServer m_tcpServer;
		unsigned int m_port;
		QString  m_clientName;
		bool m_server;
};

#endif
