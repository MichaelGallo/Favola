#pragma once

#include "LogFile.h"

#include <QJsonObject>

class CPlatform
{
public:
	static CPlatform& oGlobal();

public:
	//location of executables and dynamic libraries
	const QString& strExecutablePath()		{ return m_strExecutablePath; }

	//location of resources (skin XML and images)
	const QString& strResourcePath()		{ return m_strResourcePath; }

	//location of translation files
	const QString& strTranslationsPath()	{ return m_strTranslationsPath; }

	//where settings files can be stored
	const QString& strSettingsPath()		{ return m_strSettingsPath; }

	//default path for documents
	const QString& strDocumentPath()		{ return m_strDocumentPath; }

	//where log files can be written
	const QString strLogPath()				{ return m_strLogPath; }

public:
	//settings
	//note: we're not using the (very powerful) QSettings object on purpose,
	//because we want to be able to gather settings files from users for inspection if necessary
	static QJsonObject& oSettings();

public:
	//logging
	static CLogFile& oLogFile();

protected:
	CPlatform();

	QString	m_strExecutablePath;
	QString m_strResourcePath;
	QString m_strTranslationsPath;
	QString m_strSettingsPath;
	QString m_strDocumentPath;
	QString m_strLogPath;
};

#define LOG		CPlatform::oLogFile().Log

#ifdef QT_DEBUG
#define DEBUGLOG	LOG
#else
#define DEBUGLOG	true ? (void(0)) : LOG
#endif

const QString& strEmpty();