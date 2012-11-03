#ifndef MODEDLG_H
#define MODEDLG_H

#include <QDialog>

namespace Ui {
	class WelcomeDlg;
}

namespace TeamRadar {

// entrance of the app
class WelcomeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDlg(QWidget* parent = 0);
    ~WelcomeDlg();
	bool isOnline() const { return online; }

private slots:
    void onOnline();     // online clicked
    void onOffline();    // offline clicked
    void onSetting();    // setting clicked
    void onConnect();    // connect clicked
    void onConnected(bool success);   // connection status changed

private:
	Ui::WelcomeDlg *ui;
	bool online;
};

}

#endif // MODEDLG_H
