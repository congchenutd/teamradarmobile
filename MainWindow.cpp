#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "PeerModel.h"
#include "SettingDlg.h"
#include "Connection.h"
#include "Setting.h"
#include "PeerManager.h"
#include "SpeedDlg.h"
#include "TeamRadarEvent.h"
#include "ProjectsDlg.h"
#include "PlaylistDlg.h"
#include "RequestEventsDlg.h"

#include <QtCore/QCoreApplication>
#include <QGestureEvent>
#include <QTapGesture>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QStandardItemModel>

namespace TeamRadar {

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
    currentRow = -1;
    speed      = 1;
    playing    = false;
	fullScreen = false;
    online     = false;

    // model
	model = new QStandardItemModel(this);
	model->setColumnCount(4);
	model->setHeaderData(DateTime,  Qt::Horizontal, tr("Time"));
	model->setHeaderData(UserName,  Qt::Horizontal, tr("User"));
	model->setHeaderData(EventType, Qt::Horizontal, tr("Event"));
	model->setHeaderData(Parameter, Qt::Horizontal, tr("Parameters"));

    // view
	ui->setupUi(this);
    ui->btPlayPause->setIcon(getPlayIcon());
    ui->btPlaylist ->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));

    // what types gestures supported
	grabGesture(Qt::PinchGesture);
	grabGesture(Qt::TapGesture);
	grabGesture(Qt::TapAndHoldGesture);

    // graphics view
	QGraphicsScene *scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene->setSceneRect(-300, -300, 600, 600);
	ui->view->setScene(scene);
	PeerManager::getInstance()->setView(ui->view);

    // connections
	connect(ui->actionOnline,        SIGNAL(triggered()), this, SLOT(onOnline()));
	connect(ui->actionOffline,       SIGNAL(triggered()), this, SLOT(onOffline()));
	connect(ui->actionSelectProject, SIGNAL(triggered()), this, SLOT(onSelectProject()));
	connect(ui->actionSettings,      SIGNAL(triggered()), this, SLOT(onSettings()));
	connect(ui->actionAbout,         SIGNAL(triggered()), this, SLOT(onAbout()));
	connect(ui->btSpeed,             SIGNAL(clicked()),   this, SLOT(onSpeed()));
	connect(ui->btDownload,          SIGNAL(clicked()),   this, SLOT(onDownload()));
	connect(ui->btPlaylist,          SIGNAL(clicked()),   this, SLOT(onPlaylist()));
	connect(ui->btPlayPause,         SIGNAL(clicked()),   this, SLOT(onPlayPause()));
	connect(ui->slider,        SIGNAL(valueChanged(int)), this, SLOT(onRewind(int)));
	connect(PeerManager::getInstance(), SIGNAL(userOnline(TeamRadarEvent)), this, SLOT(onEvent(TeamRadarEvent)));

	Receiver* receiver = Receiver::getInstance();
	connect(receiver, SIGNAL(newEvent(TeamRadarEvent)),    this, SLOT(play(TeamRadarEvent)));
	connect(receiver, SIGNAL(projectsReply(QStringList)),  this, SLOT(onProjects(QStringList)));
	connect(receiver, SIGNAL(eventsReply(TeamRadarEvent)), this, SLOT(onEventDownloaded(TeamRadarEvent)));
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
	Qt::WidgetAttribute attribute;
	switch(orientation)
	{
	case ScreenOrientationLockPortrait:
		attribute = Qt::WA_LockPortraitOrientation;
		break;
	case ScreenOrientationLockLandscape:
		attribute = Qt::WA_LockLandscapeOrientation;
		break;
	default:
	case ScreenOrientationAuto:
		attribute = Qt::WA_AutoOrientation;
		break;
	}
	setAttribute(attribute, true);
}

bool MainWindow::event(QEvent* event)
{
	if(event->type() == QEvent::Gesture)
	{
		QGestureEvent* gestureEvent = static_cast<QGestureEvent*>(event);

        // tap and hold to toggle fullscreen
		if(QGesture* gesture = gestureEvent->gesture(Qt::TapAndHoldGesture))
		{
			static QPointF lastPosition;

			// Don't know why one gesture produces TWO events :(
			QPointF position = static_cast<QTapAndHoldGesture*>(gesture)->position();
			if(position != lastPosition)   // ignore the 2nd event on the same position
				toggleFullScreen();

			lastPosition = position;
			return true;
		}
	}
	return QWidget::event(event);
}

void MainWindow::toggleFullScreen()
{
	if(fullScreen)	showMaximized();
	else			showFullScreen();
}

void MainWindow::onOnline()
{
	online = true;
	showControls(false);
    onSelectProject();   // restart
}

void MainWindow::onOffline()
{
	online = false;
	showControls(true);
    onSelectProject();   // restart
}

void MainWindow::onSettings()
{
	SettingDlg dlg(this);
    dlg.exec();         // dlg will save the settings
}

void MainWindow::onAbout() {
	QMessageBox::about(this, "About",
			tr("<h3><b>Team Radar Mobile</b></h3>"
			   "<p>CongChen@utdallas.edu</a></p>"
			   "<p>Built on %1</p>").arg(Setting::getInstance()->getCompileDate()));
}

QIcon MainWindow::getPlayIcon() const {
	return style()->standardIcon(QStyle::SP_MediaPlay);
}

QIcon MainWindow::getPauseIcon() const {
	return style()->standardIcon(QStyle::SP_MediaPause);
}

void MainWindow::onSpeed()
{
	SpeedDlg dlg(this);
	dlg.setSpeed(speed);
	if(dlg.exec() == QDialog::Accepted)
		speed = dlg.getSpeed();
}

void MainWindow::onSelectProject() {
	Sender::getInstance()->sendProjectsRequest();
}

void MainWindow::onProjects(const QStringList& projectList)
{
	if(projectList.isEmpty())
	{
		QMessageBox::critical(this, tr("Error"), tr("No active projects!"));
		close();
		return;
	}

    // show available project and let the user select one
	ProjectsDlg dlg(projectList, this);
	if(dlg.exec() == QDialog::Accepted)
	{
		project = dlg.getSelectedProject();
		Setting::getInstance()->setRootPath(project);
		Sender::getInstance()->sendJoinProject(project);
		Sender::getInstance()->sendTeamMemberRequest();
		ui->view->loadDir(project);
	}
}

void MainWindow::play(const TeamRadarEvent& event)
{
//	if(online && PeerModel::isBlocked(event.userName))
//		return;

	// ignore itself
	if(event.userName == Setting::getInstance()->getUserName())
		return;

	if(!ui->view->humanExists(event.userName))   // this overrides CONNECTED event
		ui->view->addDeveloper(event.userName, PeerManager::getInstance()->getImage(event.userName));

	if(event.eventType == "SAVE")
    {
		ui->view->moveDeveloperTo(event.userName, event.parameters);
//        DirtyType dirtyType = (event.userName == Setting::getInstance()->getUserName()) ? LocalDirty
//                                                                                        : RemoteDirty;
        ui->view->setDirty(event.parameters, event.userName);
    }
	else if(event.eventType == "MODE")
		ui->view->setDeveloperMode(event.userName, event.parameters);
    else if(event.eventType == "DISCONNECTED")
		ui->view->removeDeveloper(event.userName);
    else if(event.eventType == "SCM_COMMIT")
        ui->view->setDirty(event.parameters);
}

void MainWindow::play()
{
	if(!playing)
		return;

	play(++currentRow);

    // get ready for the next event
	int nextRow = currentRow + 1;
	if(nextRow < model->rowCount())
	{
		QDateTime thisTime = model->data(model->index(currentRow, DateTime)).toDateTime();
		QDateTime nextTime = model->data(model->index(nextRow,    DateTime)).toDateTime();
		int duration = thisTime.secsTo(nextTime);
		QTimer::singleShot(duration * 1000 / speed, this, SLOT(play()));
	}
	else {
        stop();   // the end
	}
}

void MainWindow::play(int row)
{
    if(row < 0 || row > model->rowCount()-1)
        return;
	currentRow = row;
	ui->slider->setValue(row);

    // find the event and play it
	play(TeamRadarEvent(model->data(model->index(row, UserName)) .toString(),
						model->data(model->index(row, EventType)).toString(),
						model->data(model->index(row, Parameter)).toString()));
}

void MainWindow::showControls(bool show)
{
	ui->btPlayPause->setShown(show);
	ui->slider     ->setShown(show);
	ui->btSpeed    ->setShown(show);
	ui->btDownload ->setShown(show);
	ui->btPlaylist ->setShown(show);
}

void MainWindow::onDownload()
{
    // show download dialog
	RequestEventsDlg dlg(this);
    if(dlg.exec() == QDialog::Accepted)  // send download request
		Sender::getInstance()->sendEventRequest(dlg.getUserList(),
												dlg.getEventList(),
												dlg.getStartTime(),
												dlg.getEndTime(),
												dlg.getPhases(),
												dlg.getFuzziness());
}

void MainWindow::onEventDownloaded(const TeamRadarEvent& event)
{
    // add the event to playlist
	int lastRow = model->rowCount();
	model->insertRow(lastRow);
	model->setData(model->index(lastRow, 0), event.time.toString(Setting::dateTimeFormat));
	model->setData(model->index(lastRow, 1), event.userName);
	model->setData(model->index(lastRow, 2), event.eventType);
	model->setData(model->index(lastRow, 3), event.parameters);

	ui->slider->setMaximum(model->rowCount() - 1);
    model->sort(DateTime);   // reorder by time
}

void MainWindow::onPlaylist()
{
    PlaylistDlg dlg(model, this);   // show the playlist
	dlg.exec();
}

void MainWindow::onPlayPause()
{
    if(playing)   // go pause
	{
        ui->btPlayPause->setIcon(getPlayIcon());
		playing = false;
	}
    else          // go play
	{
        ui->btPlayPause->setIcon(getPauseIcon());
		playing = true;
		play();
		showFullScreen();
	}
}

void MainWindow::onRewind(int row) {
    currentRow = row;    // go to row
}

void MainWindow::showFullScreen()
{
	QMainWindow::showFullScreen();
	ui->fmControls->hide();
	fullScreen = true;
}

void MainWindow::showMaximized()
{
	QMainWindow::showMaximized();
	ui->fmControls->show();
	fullScreen = false;
}

void MainWindow::stop()
{
	currentRow = -1;
	playing = false;
    ui->btPlayPause->setIcon(getPlayIcon());
	ui->slider->setValue(0);
	showMaximized();
}

}
