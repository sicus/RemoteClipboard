/*****************************
** File: ClipboardPublicControl.cpp
** Author: Markus Biggel
** Date: 25.08.2009
** Project: Remote Clipboard
** Licence: GPL-V3
******************************/

#include "ClipboardPublicControl.h"

ClipboardPublicControl::ClipboardPublicControl()
{
	QClipboard *clip = QApplication::clipboard();
	connect(clip,SIGNAL(dataChanged()),this,SLOT(dataChanged()));
	connect(clip,SIGNAL(selectionChanged()),this,SLOT(selectionChanged()));
}

ClipboardPublicControl* ClipboardPublicControl::getInstance()
{
	static ClipboardPublicControl instance;
	return &instance;
}

void ClipboardPublicControl::setClipboardPublic(bool pub)
{
	m_clipboardPublic = pub;
}

void ClipboardPublicControl::setSelectionPublic(bool pub)
{
	m_selectionPublic = pub;
}

bool ClipboardPublicControl::getClipboardPublic()
{
	return m_clipboardPublic;
}

bool ClipboardPublicControl::getSelectionPublic()
{
	return m_selectionPublic;
}

void ClipboardPublicControl::clipboardChanged(QClipboard::Mode mode)
{
	if((mode == QClipboard::Clipboard && m_clipboardPublic) || (mode == QClipboard::Selection && m_selectionPublic))
		emit changed(mode);
}

void ClipboardPublicControl::clipboardPubChanged(int state)
{
	if(state == Qt::Checked)
		setClipboardPublic(true);
	else
		setClipboardPublic(false);
}

void ClipboardPublicControl::selectionPubChanged(int state)
{
	if(state == Qt::Checked)
		setSelectionPublic(true);
	else
		setSelectionPublic(false);
}

void ClipboardPublicControl::dataChanged()
{
 	clipboardChanged(QClipboard::Clipboard);
}

void ClipboardPublicControl::selectionChanged()
{
	clipboardChanged(QClipboard::Selection);
}

