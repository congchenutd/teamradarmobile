#include "Defines.h"
#include "MainWindow.h"
#include "WelcomeDlg.h"
#include "PeerManager.h"
#include "Setting.h"
#include <QtGui/QApplication>

using namespace TeamRadar;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	PeerModel::openDB("TeamRadar.db");
	PeerModel::createTables();
	PeerManager::getInstance();

	// font
	QFont font = app.font();
	font.setPointSize(Setting::getInstance()->getFontSize());
	app.setFont(font);

    // show the entrance
	WelcomeDlg welcome;
    if(welcome.exec() != QDialog::Accepted)   // user clicked quit
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
