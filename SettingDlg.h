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
    void setConnectionLightOn(bool on);  // green or red
    void showRestartHint();
    void onSetColor();             // set color clicked
    void onSubtlety(int value);    // subtlety changed
    void onFontSize(int value);    // font size changed

private:
	void setColor(const QColor& color);

private:
    Ui::SettingDlg *ui;
    QColor color;                  // color of this user
    TeamRadar::Setting* setting;
};

}

#endif // SETTINGDLG_H
