/*****************************
** File: NetworkConnection.h
** Author: Markus Biggel
** Date: 18.08.2009
** Project: Remote Clipboard
** Licence: GPL-V3
******************************/

#ifndef REMOTECLIENT_HEADER
#define REMOTECLIENT_HEADER

#include <QTcpSocket>
#include <QWidget>
#include <QClipboard>

#include "global.h"
#include "ui_ClientTab.h"

class RemoteClient : public QObject
{
	Q_OBJECT
	public:
		RemoteClient(QObject* parent=0);
		QWidget* getWidget();
		void disconnectFromClient();
		void setClientName(QString name);
		QString getRemoteName();
		void sendMessage(QByteArray data, int type);
		bool connectToClient(QString host, int port);
		void setSocket(QTcpSocket* socket);

	public slots:
		void dataResceived();
		void disconnected();
		void copyRemoteClipboard();
		void copyRemoteSelection();
		void clipboardChanged(QClipboard::Mode);

	signals:
		void connectionClosed();
		void remoteNameChanged(QString);
	protected:
		QTcpSocket*      m_tcpSocket;
		Ui::ClientTab*   m_clientWidget;
		QWidget          m_widget;
		QString          m_clientName;
		QString          m_remoteName;
		QClipboard*      m_clipboard;
		bool             m_identificationSend;
}; 

#endif
