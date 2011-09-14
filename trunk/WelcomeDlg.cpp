#include "WelcomeDlg.h"
#include "ui_WelcomeDlg.h"
#include "SettingDlg.h"
#include <QApplication>

WelcomeDlg::WelcomeDlg(QWidget *parent) :
	QDialog(parent), ui(new Ui::WelcomeDlg)
{
    ui->setupUi(this);
	showMaximized();

	connect(ui->btOnline,  SIGNAL(clicked()), this, SLOT(onOnline()));
	connect(ui->btOffline, SIGNAL(clicked()), this, SLOT(onOffline()));
	connect(ui->btSetting, SIGNAL(clicked()), this, SLOT(onSetting()));
	connect(ui->btQuit,    SIGNAL(clicked()), qApp, SLOT(quit()));
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
