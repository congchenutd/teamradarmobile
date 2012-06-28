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
	void setOrientation(ScreenOrientation orientation);
	virtual void showFullScreen();
	virtual void showMaximized();

protected:
	bool event(QEvent* event);

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
	void play();
    void play(const TeamRadarEvent &event);

private:
	QIcon playIcon()  const;
	QIcon pauseIcon() const;
	void toggleFullScreen();
	void play(int row);
	void stop();
	void showControls(bool show);

public:
	enum {DateTime, UserName, EventType, Parameter};

private:
    Ui::MainWindow *ui;

	QStandardItemModel* model;
	QString project;
	int speed;
	bool playing;
	int currentRow;
	bool fullScreen;
	bool online;
};

}

#endif // MAINWINDOW_H
