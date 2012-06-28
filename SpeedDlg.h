#ifndef SPEEDDLG_H
#define SPEEDDLG_H

#include <QDialog>

namespace Ui {
    class SpeedDlg;
}

namespace TeamRadar {

class SpeedDlg : public QDialog
{
	Q_OBJECT

public:
    explicit SpeedDlg(QWidget *parent = 0);
    ~SpeedDlg();
	int getSpeed() const;
	void setSpeed(int speed);

private slots:
	void onValueChanged(int speed);

private:
    Ui::SpeedDlg *ui;
};

}

#endif // SPEEDDLG_H
