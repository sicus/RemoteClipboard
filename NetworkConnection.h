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

// Protocol

// Transmission type
const unsigned char TT_CLIPBOARD_ENTRY = 0;
const unsigned char TT_SELECTION_ENTRY = 1;
const unsigned char TT_LOGIN           = 2;
const unsigned char TT_LOGOUT          = 3;

struct RC_ProtocolHeader
{
	unsigned char TransmissionType;
	unsigned char OperatingSystem;
	unsigned int  DataSize;
};

// Transmission Typs:
//
// Clipboard Entry:
// Protocolheader + bytefield
// bytefiel -> Clipboard Data
// DataSize = length of Data in Bytes
//
// Selection Entry:
// Linux Only
// Protocolheader + bytefield
// bytefiel -> Selection Data
// DataSize = length of Data in Bytes
//
// Login:
// Protocolheader + bytefield
// bytefiel -> Hostname
// DataSize = length of Data in Bytes
//
// Logout:
// Protocolheader
// DataSize = 0

class NetworkConnection : public QObject
{
	Q_OBJECT
	public:
		NetworkConnection(QObject* parent = 0, int port = DEFAULT_PORT);
		~NetworkConnection();
		void setPort(int port);
		int getPort();
		bool hasClientConnection();
		bool isServerOpen();
		bool startServer();
		bool stopServer();
		bool connectToClient(QString host, int port = DEFAULT_PORT);
		bool disconnectFromClient();
		bool sendMessage(unsigned int type, QByteArray data);

	public slots:
		void newConnection();
		void dataResceived();
		void disconnected();

	signals:
		void messageRecived(QString, int, int);
		void disconnectedFromHost();
		void connectedToHost();

	protected:
		QTcpServer m_tcpServer;
		QTcpSocket* m_tcpSocket;
		unsigned int m_port;
		QString  m_hostname;
		bool m_server;
};

#endif
