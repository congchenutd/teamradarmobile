#include "SpeedDlg.h"
#include "ui_SpeedDlg.h"

SpeedDlg::SpeedDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpeedDlg)
{
    ui->setupUi(this);
	connect(ui->slider, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
}

SpeedDlg::~SpeedDlg() {
	delete ui;
}

void SpeedDlg::onValueChanged(int speed) {
	ui->label->setText(tr("Speed = %1 X").arg(speed));
}

int SpeedDlg::getSpeed() const {
	return ui->slider->value();
}

void SpeedDlg::setSpeed(int speed)
{
	ui->slider->setValue(speed);
	onValueChanged(speed);
}
