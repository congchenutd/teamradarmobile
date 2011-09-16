#include "MainWindow.h"
#include "WelcomeDlg.h"
#include "PeerManager.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	PeerManager::getInstance();

	// font
	QFont font = app.font();
	font.setPointSize(10);
	app.setFont(font);

	// user clicked quit
	WelcomeDlg welcome;
	if(welcome.exec() != QDialog::Accepted)
		return 1;

	// main window
    MainWindow mainWindow;
	mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
	mainWindow.showMaximized();

	if(welcome.isOnline())
		mainWindow.onOnline();
	else
		mainWindow.onOffline();

    return app.exec();
}
