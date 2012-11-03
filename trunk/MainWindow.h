#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>


namespace Ui {
	class MainWindow;
}

class QStandardItemModel;

namespace TeamRadar {

struct TeamRadarEvent;
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
	~MainWindow();
    void setOrientation(ScreenOrientation orientation);  // handle orientation change
	virtual void showFullScreen();
	virtual void showMaximized();

protected:
    bool event(QEvent* event);    // handle multi-touch events

    // user interactions
public slots:
    void onOnline();
	void onOffline();
	void onEventDownloaded(const TeamRadarEvent &event);

private slots:
	void onSettings();
	void onAbout();
	void onSpeed();
	void onProjects(const QStringList& projectList);
	void onSelectProject();
	void onDownload();
	void onPlaylist();
	void onPlayPause();
	void onRewind(int row);
    void play();                             // start playing
    void play(const TeamRadarEvent& event);  // play a single event

private:
    QIcon getPlayIcon()  const;
    QIcon getPauseIcon() const;
	void toggleFullScreen();
    void play(int row);              // play a row
	void stop();
	void showControls(bool show);

public:
    enum {DateTime, UserName, EventType, Parameter};   // sections of an event

private:
    Ui::MainWindow *ui;

    QStandardItemModel* model;  // for events
    QString project;            // project name
    int  currentRow;            // the row currently playing
    int  speed;                 // playing speed, 1~10
    bool playing;               // is playing?
    bool fullScreen;            // is fullscreen?
    bool online;                // online mode?
};

}

#endif // MAINWINDOW_H
