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

	// load settings
	setting = Setting::getInstance();
	ui->leUserName->setText(setting->getUserName());
	ui->leAddress ->setText(setting->getServerAddress());
	ui->lePort    ->setText(QString::number(Setting::getInstance()->getServerPort()));
	setColor(setting->getColor("DefaultDeveloperColor"));            // color
	onSubtlety(1.0 / setting->getThreshold());
	onFontSize(setting->getFontSize());
	ui->cbLightTrail->setChecked(setting->showLightTrail());
	ui->cbAfterImage->setChecked(setting->showAfterImage());
	showMaximized();

	connect(ui->leAddress,  SIGNAL(textChanged(QString)), this, SLOT(onShowRestartHint()));
	connect(ui->lePort,     SIGNAL(textChanged(QString)), this, SLOT(onShowRestartHint()));
	connect(ui->leUserName, SIGNAL(textChanged(QString)), this, SLOT(onShowRestartHint()));
	connect(ui->btColor,    SIGNAL(clicked()), this, SLOT(onSetColor()));
	connect(ui->sliderSubtlety, SIGNAL(valueChanged(int)), this, SLOT(onSubtlety(int)));
	connect(ui->sliderFontSize, SIGNAL(valueChanged(int)), this, SLOT(onFontSize(int)));
	connect(Connection::getInstance(), SIGNAL(connectionStatusChanged(bool)), this, SLOT(setLight(bool)));
	setLight(Connection::getInstance()->isReadyForUse());
}

SettingDlg::~SettingDlg() {
	delete ui;
}

void SettingDlg::accept()
{
	// save settings
	setting->setUserName(ui->leUserName->text());
	setting->setServerAddress(ui->leAddress->text());
	setting->setServerPort(ui->lePort->text().toInt());
	setting->setColor("DefaultDeveloperColor", color);
	setting->setEngineSubtlety(ui->sliderSubtlety->value());
	setting->setFontSize(ui->sliderFontSize->value());
	setting->setShowLightTrail(ui->cbLightTrail->isChecked());
	setting->setShowAfterImage(ui->cbAfterImage->isChecked());

	// register color
	Sender::getInstance()->sendColorRegistration(color);

	// register photo
	QImage image(":/Images/Cellphone.png");
	QBuffer imageBuffer;
	imageBuffer.open(QIODevice::WriteOnly);
	image.save(&imageBuffer, "PNG");   // convert the image to QByteArray
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

void SettingDlg::onSubtlety(int value)
{
	ui->sliderSubtlety->setValue(value);
	ui->labelSubtlety->setText(tr("Animation subtlety = %1").arg(value));
}

void SettingDlg::onFontSize(int value)
{
	ui->sliderFontSize->setValue(value);
	ui->labelFontSize->setText(tr("Font size = %1").arg(value));
}
