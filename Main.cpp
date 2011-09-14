#include "MainWindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	QFont font = app.font();
	font.setPointSize(10);
	app.setFont(font);

    MainWindow mainWindow;
	mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
	mainWindow.showMaximized();

    return app.exec();
}
