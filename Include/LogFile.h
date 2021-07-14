#pragma once

#include "Common.h"
#include <QString>
#include <QFile>
#include <mutex>

class CLogFile
{
public:
	CLogFile(const QString& strFileName);

	~CLogFile();

	void Log(const char* pszModule, const char* pszFormat, ...);

protected:
	QFile		m_oFile;
	std::mutex	m_mtx;

	void Output(const QString& str);
};
