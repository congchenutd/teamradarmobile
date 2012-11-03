#include "PlaylistDlg.h"
#include "ui_PlaylistDlg.h"
#include <QStandardItemModel>

namespace TeamRadar {

PlaylistDlg::PlaylistDlg(QStandardItemModel* m, QWidget* parent) :
    QDialog(parent), ui(new Ui::PlaylistDlg), model(m)
{
    ui->setupUi(this);
	showMaximized();
	ui->tableView->setModel(model);
	ui->tableView->resizeColumnsToContents();
	ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

PlaylistDlg::~PlaylistDlg() {
    delete ui;
}

}
