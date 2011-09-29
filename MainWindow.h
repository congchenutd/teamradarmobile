#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

struct TeamRadarEvent;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
	void setOrientation(ScreenOrientation orientation);

protected:
	bool event(QEvent* event);

private:
	void switchFullScreen();
	QIcon playIcon()  const;
	QIcon pauseIcon() const;
	void play(const TeamRadarEvent &event);
	void showControls(bool show);

public slots:
	void onOnline();
	void onOffline();

private slots:
	void onSettings();
	void onAbout();
	void onSpeed();
	void onEvent(const TeamRadarEvent& event);
	void onProjects(const QStringList& projectList);
	void onSelectProject();
	void onDownload();

private:
    Ui::MainWindow *ui;
	QString project;
	int speed;
};

#endif // MAINWINDOW_H
