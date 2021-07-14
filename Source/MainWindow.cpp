#include "MainWindow.h"
#include "Platform.h"
#include <QSplitter>

CMainWindow* CMainWindow::m_poMainWindow = nullptr;

namespace
{

bool bRectFromString(const QString& str,
					 OUT QRect* prc)
{
	int i = 0;
	bool bOK = true;
	QVector<QStringRef> oValues = str.splitRef(',');

	if (oValues.size() != 4)
	{
		return false;
	}

	int aiValues[4];

	for (auto it = oValues.begin(); it != oValues.end() && i < 4; it++, i++)
	{
		aiValues[i] = it->trimmed().toLongLong(&bOK);

		if (!bOK)
		{
			break;
		}
	}

	if (bOK)
	{
		*prc = QRect(aiValues[0], aiValues[1], aiValues[2], aiValues[3]);
	}

	return bOK;
}

}	//anonymous namespace

CMainWindow::CMainWindow()
{
	m_poMainWindow = this;

	setWindowTitle("Favola");

	bool bPositionRestored = false;
	QJsonObject oMainWindowSettings = CPlatform::oSettings().value("MainWindow").toObject();

	if (!oMainWindowSettings.isEmpty())
	{
		QString strState = oMainWindowSettings["State"].toString("maximized");

		if (STREQ(strState, "fullscreen"))
		{
			showFullScreen();
			bPositionRestored = true;
		}
		else if (STREQ(strState, "maximized"))
		{
			showMaximized();
			bPositionRestored = true;
		}
		else
		{
			QString strPosition = oMainWindowSettings["Position"].toString();
			QRect rcPosition;

			if (!strPosition.isEmpty() && bRectFromString(strPosition, &rcPosition))
			{
				//QRect rcQDesktop = QApplication::desktop()->rect();
				//TODO: make sure window is visible in its entirety

				resize(rcPosition.width(), rcPosition.height());
				move(rcPosition.left(), rcPosition.top());
				show();
				bPositionRestored = true;
			}
		}
	}

	if (!bPositionRestored)
	{
		showMaximized();
	}

	m_poStory = new QTextEdit(this);
	m_poActions = new QTextEdit(this);
	
	QSplitter* poSplitter = new QSplitter(this);

	//TODO: restore splitter position

	QRect rc = contentsRect();

	poSplitter->setGeometry(rc);
	poSplitter->setOrientation(Qt::Vertical);
	poSplitter->addWidget(m_poStory);
	poSplitter->addWidget(m_poActions);

	poSplitter->setCollapsible(0, false);	//disallow collapsing either part
	poSplitter->setCollapsible(1, false);
	poSplitter->setStretchFactor(0, 9);		//story area will be roughly 9 x action area
	poSplitter->setStretchFactor(1, 1);
}

CMainWindow::~CMainWindow()
{
	//save window state and position
	QJsonObject oMainWindowSettings = CPlatform::oSettings().value("MainWindow").toObject();

	oMainWindowSettings["State"] = isFullScreen() ? "fullscreen" : (isMaximized() ? "maximized" : "normal");
	oMainWindowSettings["Position"] = QString::asprintf("%d,%d,%d,%d", pos().x(), pos().y(), size().width(), size().height());

	CPlatform::oSettings()["MainWindow"] = oMainWindowSettings;

	m_poMainWindow = nullptr;
}
