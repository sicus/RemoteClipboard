/*****************************
** File: RCSettings.h
** Author: Markus Biggel
** Date: 25.08.2009
** Project: Remote Clipboard
** Licence: GPL-V3
******************************/

#ifndef REMOTE_CLIPBOARD_SETTINGS
#define REMOTE_CLIPBOARD_SETTINGS

#include <QSettings>
#include <QDir>
#include "global.h"

class RCSettings
{
	public:
		static RCSettings* getInstance();
		void loadSettings();
		QString getClientName();
		int getPort();
		bool getSslOnly();

		void setClientName(QString name);
		void setPort(int port);
		void setSslOnly(bool ssl);
		void saveSettings();

	protected:
		RCSettings();
		QString m_clientName;
		int m_port;
		bool m_sslOnly;
		QString m_settingsFile;
};

#endif
