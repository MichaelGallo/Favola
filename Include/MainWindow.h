#pragma once

#include <QMainWindow>
#include <QTextEdit>

class CMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	CMainWindow();

	virtual ~CMainWindow();

	static CMainWindow* poGlobal() { return m_poMainWindow; }

protected:
//	void resizeEvent(QResizeEvent* poEvent) override;

//	void showEvent(QShowEvent* poEvent) override;
	
private:
	QTextEdit* m_poStory;
	QTextEdit* m_poActions;

	static CMainWindow* m_poMainWindow;
};
