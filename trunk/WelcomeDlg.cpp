#include "WelcomeDlg.h"
#include "ui_WelcomeDlg.h"
#include "SettingDlg.h"
#include "Connection.h"
#include <QApplication>

namespace TeamRadar {

WelcomeDlg::WelcomeDlg(QWidget *parent) :
	QDialog(parent), ui(new Ui::WelcomeDlg)
{
    ui->setupUi(this);
	showMaximized();

	connect(ui->btConnect, SIGNAL(clicked()), this, SLOT(onConnect()));
	connect(ui->btOnline,  SIGNAL(clicked()), this, SLOT(onOnline()));
	connect(ui->btOffline, SIGNAL(clicked()), this, SLOT(onOffline()));
	connect(ui->btSetting, SIGNAL(clicked()), this, SLOT(onSetting()));
	connect(ui->btQuit,    SIGNAL(clicked()), this, SLOT(reject()));
	connect(Connection::getInstance(), SIGNAL(connectionStatusChanged(bool)), this, SLOT(onConnected(bool)));

	onConnected(false);
	onConnect();
}

WelcomeDlg::~WelcomeDlg() {
	delete ui;
}

void WelcomeDlg::onOnline()
{
	online = true;
	accept();
}

void WelcomeDlg::onOffline()
{
	online = false;
	accept();
}

void WelcomeDlg::onSetting()
{
	SettingDlg dlg(this);
	dlg.exec();
}

void WelcomeDlg::onConnected(bool success)
{
	ui->btOnline ->setShown(success);
	ui->btOffline->setShown(success);
	ui->btConnect->setShown(!success);
}

void WelcomeDlg::onConnect() {
	Connection::getInstance()->connectToHost(Setting::getInstance()->getServerAddress(),
											 Setting::getInstance()->getServerPort());
}

}
