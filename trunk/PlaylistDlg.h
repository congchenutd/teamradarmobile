#ifndef PLAYLISTDLG_H
#define PLAYLISTDLG_H

#include <QDialog>

namespace Ui {
    class PlaylistDlg;
}

class QStandardItemModel;

namespace TeamRadar {

// a dialog showing all the downloaded events
class PlaylistDlg : public QDialog
{
    Q_OBJECT

public:
    // m is the model holding the events
    explicit PlaylistDlg(QStandardItemModel* m, QWidget* parent = 0);
    ~PlaylistDlg();

private:
    Ui::PlaylistDlg *ui;
	QStandardItemModel* model;
};

}

#endif // PLAYLISTDLG_H
