#ifndef PLAYLISTDLG_H
#define PLAYLISTDLG_H

#include <QDialog>

namespace Ui {
    class PlaylistDlg;
}

class QStandardItemModel;

namespace TeamRadar {

class PlaylistDlg : public QDialog
{
    Q_OBJECT

public:
	explicit PlaylistDlg(QStandardItemModel* m, QWidget *parent = 0);
    ~PlaylistDlg();

private:
    Ui::PlaylistDlg *ui;
	QStandardItemModel* model;
};

}

#endif // PLAYLISTDLG_H
