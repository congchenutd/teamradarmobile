#ifndef SETTINGDLG_H
#define SETTINGDLG_H

#include <QDialog>
#include <QColor>

namespace Ui {
    class SettingDlg;
}

namespace TeamRadar {

class Setting;
class SettingDlg : public QDialog
{
	Q_OBJECT

public:
    explicit SettingDlg(QWidget *parent = 0);
    ~SettingDlg();
	void accept();

private slots:
	void setLight(bool on);
	void onSetColor();
	void onShowRestartHint();
	void onSubtlety(int value);
	void onFontSize(int value);

private:
	void setColor(const QColor& color);

private:
    Ui::SettingDlg *ui;
	QColor color;
    TeamRadar::Setting* setting;
};

}

#endif // SETTINGDLG_H
