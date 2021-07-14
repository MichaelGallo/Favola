#include <memory>

#include "MainWindow.h"
//#include "Platform.h"
#include <QApplication>
#include <QTranslator>

class CFavola : public QApplication
{
public:
	CFavola(int& argc, char** argv) : QApplication(argc, argv) {}

	const QString& strFileOpenArgument() const	{ return m_strFileOpenArgument; }

protected:
//	bool event(QEvent* poEvent);

private:
	QString m_strFileOpenArgument;
};


int main(int argc, char *argv[])
{

	CFavola oApp(argc, argv);
	
#ifdef Q_OS_MAC
	poApp->processEvents();
	poApp->processEvents();
	
	if (!poApp->strFileOpenArgument().isEmpty())
	{
		//open the game
	}
#endif

	CMainWindow oMainWindow;

	return oApp.exec();
}
