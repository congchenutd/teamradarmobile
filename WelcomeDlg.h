#ifndef MODEDLG_H
#define MODEDLG_H

#include <QDialog>

namespace Ui {
	class WelcomeDlg;
}

namespace TeamRadar {

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
	void onConnected(bool success);
	void onConnect();

private:
	Ui::WelcomeDlg *ui;
	bool online;
};

}

#endif // MODEDLG_H
