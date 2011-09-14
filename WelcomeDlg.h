#ifndef MODEDLG_H
#define MODEDLG_H

#include <QDialog>

namespace Ui {
	class WelcomeDlg;
}

class WelcomeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDlg(QWidget *parent = 0);
    ~WelcomeDlg();
	bool isOnline() const { return online; }

private slots:
	void onOnline();
	void onOffline();
	void onSetting();

private:
	Ui::WelcomeDlg *ui;
	bool online;
};

#endif // MODEDLG_H
