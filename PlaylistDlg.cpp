#include "PlaylistDlg.h"
#include "ui_PlaylistDlg.h"
#include <QStandardItemModel>

PlaylistDlg::PlaylistDlg(QStandardItemModel* m, QWidget *parent) :
	QDialog(parent), model(m), ui(new Ui::PlaylistDlg)
{
    ui->setupUi(this);
	showMaximized();
	ui->tableView->setModel(model);
	ui->tableView->resizeColumnsToContents();
	ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

PlaylistDlg::~PlaylistDlg()
{
    delete ui;
}
