#include "Platform.h"

#include <QStandardPaths>
#include <QThread>
#include <QDir>
#include <QJsonDocument>
#include <QtEndian>

#include <memory>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

#ifdef Q_OS_MAC
#include <CoreFoundation/CoreFoundation.h>
#include <sys/sysctl.h>
#endif

#ifndef APPNAME
#define APPNAME "Favola"
#endif

CPlatform::CPlatform()
{
#ifdef Q_OS_WIN
	wchar_t* pwsz = new wchar_t[_MAX_PATH + 1];

	GetModuleFileNameW(NULL, pwsz, _MAX_PATH);

	m_strExecutablePath		= QString((QChar*)pwsz, _MAX_PATH);
	m_strExecutablePath.replace('\\', '/');
	m_strExecutablePath		= m_strExecutablePath.section('/', 0, -2);

	m_strResourcePath		= m_strExecutablePath + "/Resources";
	m_strTranslationsPath	= m_strExecutablePath + "/translations";

#else
	CFURLRef refURL		= CFBundleCopyExecutableURL(CFBundleGetMainBundle());
	CFStringRef refPath	= CFURLCopyFileSystemPath(refURL, kCFURLPOSIXPathStyle);

	QByteArray oUTF8;
	CFIndex nLength			= CFStringGetLength(refPath);
	oUTF8.resize(int(nLength * 3 + 1));
	CFStringGetCString(refPath, oUTF8.data(), nLength * 3 + 1, kCFStringEncodingUTF8);

	CFRelease(refURL);
	CFRelease(refPath);

	m_strExecutablePath		= QString::fromUtf8(oUTF8);
	m_strExecutablePath		= m_strExecutablePath.section('/', 0, -2);

	m_strResourcePath		= m_strExecutablePath.section('/', 0, -2) + "/Resources";
	m_strTranslationsPath	= m_strExecutablePath.section('/', 0, -2) + "/translations";
#endif

	m_strSettingsPath		= QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).front();
	m_strDocumentPath		= QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).front() + "/" APPNAME;
	m_strLogPath			= QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation).front();

	QDir oLogDir(m_strLogPath);
	oLogDir.mkpath(".");

	QDir oSettingsDir(m_strSettingsPath);
	oSettingsDir.mkpath(".");
}

CPlatform& CPlatform::oGlobal()
{
	static CPlatform oGlobal;
	return oGlobal;
}

QJsonObject& CPlatform::oSettings()
{
	class CAutoSaveSettings
	{
	public:
		CAutoSaveSettings(const QString& strPath)
		:	INIT(strPath)
		{
			QFile oFile(strPath);

			if (oFile.open(QIODevice::ReadOnly))
			{
				QByteArray oData = oFile.readAll();

				QJsonDocument oDocument;

				if (strPath.endsWith(".json", Qt::CaseInsensitive))
				{
					oDocument = QJsonDocument::fromJson(oData);
				}
				else
				{
					oDocument = QJsonDocument::fromBinaryData(oData);
				}

				if (oDocument.isObject())
				{
					m_oJson = oDocument.object();
				}
			}
		}

		~CAutoSaveSettings()
		{
			QFile oFile(m_strPath);

			if (!m_oJson.isEmpty() && oFile.open(QIODevice::WriteOnly))
			{
				QJsonDocument oDocument;
				oDocument.setObject(m_oJson);

				QByteArray oData;
			
				if (m_strPath.endsWith(".json", Qt::CaseInsensitive))
				{
					oData = oDocument.toJson();
				}
				else
				{
					oData = oDocument.toBinaryData();
				}

				oFile.write(oData);
			}
		}

		QJsonObject& oJson()	{ return m_oJson; }

	private:
		QString		m_strPath;
		QJsonObject m_oJson;
	};

	static CAutoSaveSettings oSettings(oGlobal().strSettingsPath() + "/Settings.json");
	return oSettings.oJson();
}

namespace
{
	std::once_flag g_oOnceFlagLog;
}

CLogFile& CPlatform::oLogFile()
{
	static std::unique_ptr<CLogFile> poLogFile;

	std::call_once(g_oOnceFlagLog, [] { poLogFile.reset(new CLogFile(oGlobal().strLogPath() + "/" + APPNAME + ".log")); });

	return *poLogFile.get();
}

const QString& strEmpty()
{
	static QString str;
	return str;
}
