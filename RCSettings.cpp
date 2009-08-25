/*****************************
** File: RCSettings.cpp
** Author: Markus Biggel
** Date: 25.08.2009
** Project: Remote Clipboard
** Licence: GPL-V3
******************************/

#include "RCSettings.h"

RCSettings::RCSettings()
{
	m_settingsFile = QDir::toNativeSeparators(QDir::home().absolutePath());

	if(OPERATING_SYSTEM == OS_LINUX)
		m_settingsFile += "/.RemoteClipboard/";
	else if(OPERATING_SYSTEM == OS_WIN32)
		m_settingsFile += "\\.RemoteClipboard\\";
	QDir dir(m_settingsFile);
	dir.mkpath(m_settingsFile);

	m_settingsFile += "settings.ini";
	loadSettings();
}

RCSettings* RCSettings::getInstance()
{
	static RCSettings instance;
	return &instance;
}

QString RCSettings::getClientName()
{
	return m_clientName;
}

void RCSettings::setClientName(QString name)
{
	if(name.isEmpty())
		m_clientName = "Unknown";
	else
		m_clientName = name;
}

int RCSettings::getPort()
{
	return m_port;
}

void RCSettings::setPort(int port)
{
	m_port = port;
}

bool RCSettings::getSslOnly()
{
	return m_sslOnly;
}

void RCSettings::setSslOnly(bool ssl)
{
	m_sslOnly = ssl;
}

void RCSettings::loadSettings()
{
	QSettings settings(m_settingsFile,QSettings::IniFormat);
	settings.beginGroup("ClientSettings");
	m_clientName = settings.value("ClientName","Unknown").toString();
	m_port = settings.value("Port",DEFAULT_PORT).toInt();
	m_sslOnly = settings.value("SSL",false).toBool();
	settings.endGroup();
}

void RCSettings::saveSettings()
{
	QSettings settings(m_settingsFile,QSettings::IniFormat);
	settings.beginGroup("ClientSettings");
	settings.setValue("ClientName",m_clientName);
	settings.setValue("Port",m_port);
	settings.setValue("SSL",m_sslOnly);
	settings.endGroup();
}
