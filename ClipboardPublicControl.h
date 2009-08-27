/*****************************
** File: ClipboardPublicControl.h
** Author: Markus Biggel
** Date: 25.08.2009
** Project: Remote Clipboard
** Licence: GPL-V3
******************************/

#ifndef CLIPBOARD_PUBLIC_CONTROL
#define CLIPBOARD_PUBLIC_CONTROL

#include <QClipboard>
#include <QApplication>
#include <QMessageBox>

class ClipboardPublicControl : public QObject
{
	Q_OBJECT
	public:
		static ClipboardPublicControl* getInstance();
		void setClipboardPublic(bool pub);
		void setSelectionPublic(bool pub);
		bool getClipboardPublic();
		bool getSelectionPublic();

	public slots:
		void clipboardChanged(QClipboard::Mode);
		void clipboardPubChanged(int);
		void selectionPubChanged(int);
		
		void dataChanged();
		void selectionChanged();

	signals:
		void changed(QClipboard::Mode);

	protected:
		ClipboardPublicControl();
		bool m_clipboardPublic;
		bool m_selectionPublic;
};

#endif
