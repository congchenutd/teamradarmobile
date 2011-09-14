#ifndef SETTINGDLG_H
#define SETTINGDLG_H

#include <QDialog>
#include <QColor>

namespace Ui {
    class SettingDlg;
}

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

private:
	void setColor(const QColor& color);

private:
    Ui::SettingDlg *ui;
	QColor color;
};

#endif // SETTINGDLG_H
