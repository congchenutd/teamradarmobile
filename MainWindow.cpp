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
#include "../QtCreator/src/plugins/TeamRadar/RequestEventsDlg.h"

#include <QtCore/QCoreApplication>
#include <QGestureEvent>
#include <QTapGesture>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
	speed = 1;
	playing = false;
	currentRow = -1;
	fullScreen = false;
	online = false;

	model = new QStandardItemModel(this);
	model->setColumnCount(4);
	model->setHeaderData(DateTime,  Qt::Horizontal, tr("Time"));
	model->setHeaderData(UserName,  Qt::Horizontal, tr("User"));
	model->setHeaderData(EventType, Qt::Horizontal, tr("Event"));
	model->setHeaderData(Parameter, Qt::Horizontal, tr("Parameters"));

    ui->setupUi(this);
	grabGesture(Qt::PinchGesture);
	grabGesture(Qt::TapGesture);
	grabGesture(Qt::TapAndHoldGesture);

	ui->btPlayPause->setIcon(playIcon());
	ui->btPlaylist ->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));

	PeerModel::openDB("TeamRadar.db");
	PeerModel::createTables();

	QGraphicsScene *scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene->setSceneRect(-300, -300, 600, 600);
	ui->view->setScene(scene);
	PeerManager::getInstance()->setView(ui->view);

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
	connect(receiver, SIGNAL(newEvent(TeamRadarEvent)),       this, SLOT(play(TeamRadarEvent)));
	connect(receiver, SIGNAL(projectsResponse(QStringList)),  this, SLOT(onProjects(QStringList)));
	connect(receiver, SIGNAL(eventsResponse(TeamRadarEvent)), this, SLOT(onEventDownloaded(TeamRadarEvent)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
//#if defined(Q_OS_SYMBIAN)
//	// If the version of Qt on the device is < 4.7.2, that attribute won't work
//	if (orientation != ScreenOrientationAuto) {
//		const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
//		if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
//			qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
//			return;
//		}
//	}
//#endif // Q_OS_SYMBIAN

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
	onSelectProject();
}

void MainWindow::onOffline()
{
	online = false;
	showControls(true);
	onSelectProject();
}

void MainWindow::onSettings()
{
	SettingDlg dlg(this);
	dlg.exec();
}

void MainWindow::onAbout() {
	QMessageBox::about(this, "About",
			tr("<h3><b>Team Radar Mobile</b></h3>"
			   "<p>CongChen@utdallas.edu</a></p>"));
}

QIcon MainWindow::playIcon() const {
	return style()->standardIcon(QStyle::SP_MediaPlay);
}

QIcon MainWindow::pauseIcon() const {
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

	ProjectsDlg dlg(projectList, this);
	if(dlg.exec() == QDialog::Accepted)
	{
		project = dlg.getProject();
		Setting::getInstance()->setRootPath(project);
		Sender::getInstance()->sendJoinProject(project);
		PeerManager::getInstance()->refreshUserList();
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
		ui->view->moveDeveloperTo(event.userName, event.parameters);
	else if(event.eventType == "MODE")
		ui->view->setDeveloperMode(event.userName, event.parameters);
	else if(event.eventType == "DISCONNECTED") {
		ui->view->removeDeveloper(event.userName);
	}
}

void MainWindow::play()
{
	if(!playing)
		return;

	play(++currentRow);

	// ready for the next
	int nextRow = currentRow + 1;
	if(nextRow < model->rowCount())
	{
		QDateTime thisTime = model->data(model->index(currentRow, DateTime)).toDateTime();
		QDateTime nextTime = model->data(model->index(nextRow,    DateTime)).toDateTime();
		int duration = thisTime.secsTo(nextTime);
		QTimer::singleShot(duration * 1000 / speed, this, SLOT(play()));
	}
	else {
		stop();
	}
}

void MainWindow::play(int row)
{
	currentRow = row;
	ui->slider->setValue(row);
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
	RequestEventsDlg dlg(this);
	if(dlg.exec() == QDialog::Accepted)
		Sender::getInstance()->sendEventRequest(dlg.getUserList(),
												dlg.getEventList(),
												dlg.getStartTime(),
												dlg.getEndTime(),
												dlg.getPhases(),
												dlg.getFuzziness());
}

void MainWindow::onEventDownloaded(const TeamRadarEvent& event)
{
	int lastRow = model->rowCount();
	model->insertRow(lastRow);
	model->setData(model->index(lastRow, 0), event.time.toString(Setting::dateTimeFormat));
	model->setData(model->index(lastRow, 1), event.userName);
	model->setData(model->index(lastRow, 2), event.eventType);
	model->setData(model->index(lastRow, 3), event.parameters);

	ui->slider->setMaximum(model->rowCount() - 1);
	model->sort(DateTime);
}

void MainWindow::onPlaylist()
{
	PlaylistDlg dlg(model, this);
	dlg.exec();
}

void MainWindow::onPlayPause()
{
	if(playing)   // pause
	{
		ui->btPlayPause->setIcon(playIcon());
		playing = false;
	}
	else          // start
	{
		ui->btPlayPause->setIcon(pauseIcon());
		playing = true;
		play();
		showFullScreen();
	}
}

void MainWindow::onRewind(int row) {
	currentRow = row;
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
	ui->btPlayPause->setIcon(playIcon());
	ui->slider->setValue(0);
	showMaximized();
}
