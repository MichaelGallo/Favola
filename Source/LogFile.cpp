#include "LogFile.h"
#include <stdarg.h>
#include <QTime>

CLogFile::CLogFile(const QString& strFileName)
:	m_oFile(strFileName)
{
	m_oFile.open(QIODevice::WriteOnly);

	if (m_oFile.isOpen())
	{
		Output("[LogFile     ] Opening log file");
	}
	else
	{
		Q_ASSERT(!"Could not open log file for writing");
	}
}

CLogFile::~CLogFile()
{
	if (m_oFile.isOpen())
	{
		m_oFile.write("Closing log file.\n", 18);
	}
}

void CLogFile::Log(const char* pszModule, const char* pszFormat, ...)
{
	va_list argList;
	va_start(argList, pszFormat);
	QString str;
	str.vsprintf(pszFormat, argList);
	va_end(argList);

	QString strModule(pszModule);

	strModule = strModule.left(12);
	strModule = "[" + strModule + QString(" ").repeated(12 - strModule.length()) + "] ";

	Output(strModule + str);
}

void CLogFile::Output(const QString& str)
{
	if (!m_oFile.isOpen())
	{
		return;
	}

	QString strOutput = QTime::currentTime().toString() + " " + str;

	if (!strOutput.endsWith(10))
	{
		strOutput.append("\n");
	}

	SCOPEDMUTEX(m_mtx);

	m_oFile.write(strOutput.toUtf8().data());
	m_oFile.flush();
}
