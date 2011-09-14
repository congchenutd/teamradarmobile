#include "SettingDlg.h"
#include "ui_SettingDlg.h"
#include "Setting.h"
#include "Connection.h"
#include <QColorDialog>
#include <QBuffer>

SettingDlg::SettingDlg(QWidget *parent) :
	QDialog(parent), ui(new Ui::SettingDlg)
{
    ui->setupUi(this);

	ui->leUserName->setText(Setting::getInstance()->getUserName());
	ui->leAddress ->setText(Setting::getInstance()->getServerAddress());
	ui->lePort    ->setText(QString::number(Setting::getInstance()->getServerPort()));
	showMaximized();
	setColor(Setting::getInstance()->getColor("DefaultDeveloperColor"));            // color

	connect(ui->leAddress,  SIGNAL(textChanged(QString)), this, SLOT(onShowRestartHint()));
	connect(ui->lePort,     SIGNAL(textChanged(QString)), this, SLOT(onShowRestartHint()));
	connect(ui->leUserName, SIGNAL(textChanged(QString)), this, SLOT(onShowRestartHint()));
	connect(ui->btColor, SIGNAL(clicked()), this, SLOT(onSetColor()));
	connect(Connection::getInstance(), SIGNAL(connectionStatusChanged(bool)), this, SLOT(setLight(bool)));
	setLight(Connection::getInstance()->isReadyForUse());
}

SettingDlg::~SettingDlg() {
	delete ui;
}

void SettingDlg::accept()
{
	// save settings
	Setting* setting = Setting::getInstance();
	setting->setUserName(ui->leUserName->text());
	setting->setServerAddress(ui->leAddress->text());
	setting->setServerPort(ui->lePort->text().toInt());
	setting->setColor("DefaultDeveloperColor", color);

	// register color
	Sender::getInstance()->sendColorRegistration(color);

	// register photo
	QImage image(":/Images/Cellphone.png");
	QBuffer imageBuffer;
	imageBuffer.open(QIODevice::WriteOnly);
	image.save(&imageBuffer, "PNG");
	Sender::getInstance()->sendPhotoRegistration("png", imageBuffer.buffer());
	return QDialog::accept();
}

void SettingDlg::setLight(bool on) {
	ui->labelLight->setPixmap(on ? QPixmap(":/Images/Green.png")
								 : QPixmap(":/Images/Red.png"));
}

void SettingDlg::setColor(const QColor& clr)
{
	if(clr.isValid())
	{
		color = clr;
		QPixmap pixmap(ui->labelColor->size());
		pixmap.fill(color);
		ui->labelColor->setPixmap(pixmap);
	}
}

void SettingDlg::onSetColor() {
	setColor(QColorDialog::getColor(color));
}

void SettingDlg::onShowRestartHint() {
	ui->labelMessage->setText(tr("Restart to activate the changes"));
}