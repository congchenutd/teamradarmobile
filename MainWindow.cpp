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
#include "WelcomeDlg.h"

#include <QtCore/QCoreApplication>
#include <QGestureEvent>
#include <QTapGesture>
#include <QMessageBox>
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
	speed = 1;

    ui->setupUi(this);
	grabGesture(Qt::PinchGesture);
	grabGesture(Qt::TapGesture);
	grabGesture(Qt::TapAndHoldGesture);

	ui->btPlayPause->setIcon(playIcon());
	ui->btEvents   ->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));

	PeerModel::openDB("TeamRadar.db");
	PeerModel::createTables();

	QGraphicsScene *scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene->setSceneRect(-300, -300, 600, 600);
	ui->view->setScene(scene);

	connect(ui->actionOnline,        SIGNAL(triggered()), this, SLOT(onOnline()));
	connect(ui->actionOffline,       SIGNAL(triggered()), this, SLOT(onOffline()));
	connect(ui->actionSelectProject, SIGNAL(triggered()), this, SLOT(onSelectProject()));
	connect(ui->actionSettings,      SIGNAL(triggered()), this, SLOT(onSettings()));
	connect(ui->actionAbout,         SIGNAL(triggered()), this, SLOT(onAbout()));
	connect(ui->btSpeed,             SIGNAL(clicked()),   this, SLOT(onSpeed()));
	connect(PeerManager::getInstance(), SIGNAL(userOnline(TeamRadarEvent)), this, SLOT(onEvent(TeamRadarEvent)));
	connect(Receiver   ::getInstance(), SIGNAL(newEvent  (TeamRadarEvent)), this, SLOT(onEvent(TeamRadarEvent)));
	connect(Receiver   ::getInstance(), SIGNAL(projectsResponse(QStringList)), this, SLOT(onProjects(QStringList)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
	// If the version of Qt on the device is < 4.7.2, that attribute won't work
	if (orientation != ScreenOrientationAuto) {
		const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
		if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
			qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
			return;
		}
	}
#endif // Q_OS_SYMBIAN

	Qt::WidgetAttribute attribute;
	switch (orientation) {
#if QT_VERSION < 0x040702
	// Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
	case ScreenOrientationLockPortrait:
		attribute = static_cast<Qt::WidgetAttribute>(128);
		break;
	case ScreenOrientationLockLandscape:
		attribute = static_cast<Qt::WidgetAttribute>(129);
		break;
	default:
	case ScreenOrientationAuto:
		attribute = static_cast<Qt::WidgetAttribute>(130);
		break;
#else // QT_VERSION < 0x040702
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
#endif // QT_VERSION < 0x040702
	};
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

			QTapAndHoldGesture* tapAndHoldGesture = static_cast<QTapAndHoldGesture*>(gesture);

			// Don't know why one gesture produces TWO events :(
			QPointF position = tapAndHoldGesture->position();
			if(position != lastPosition)
				switchFullScreen();

			lastPosition = position;
			return true;
		}
	}
	return QWidget::event(event);
}

void MainWindow::switchFullScreen()
{
	static bool fullScreen = false;
	if(fullScreen)
	{
		showMaximized();
		ui->fmControls->show();
		fullScreen = false;
	}
	else
	{
		showFullScreen();
		ui->fmControls->hide();
		fullScreen = true;
	}
}

void MainWindow::onOnline()
{
	onSelectProject();
}

void MainWindow::onOffline()
{
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

void MainWindow::onEvent(const TeamRadarEvent& event)
{
	qDebug() << event.eventType;
	play(event);
}

void MainWindow::onSelectProject() {
	Sender::getInstance()->sendProjectsRequest();
}

void MainWindow::onProjects(const QStringList& projectList)
{
	if(projectList.isEmpty())
	{
		QMessageBox::critical(this, tr("Error"), tr("There are no active projects!"));
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
