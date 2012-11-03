#include "ProjectsDlg.h"
#include "ui_ProjectsDlg.h"
#include <QApplication>
#include <QDesktopWidget>

namespace TeamRadar {

ProjectsDlg::ProjectsDlg(const QStringList& list, QWidget *parent) :
	QDialog(parent), ui(new Ui::ProjectsDlg)
{
    ui->setupUi(this);
	showMaximized();

    // load the model
	model.setColumnCount(1);
	model.setHeaderData(0, Qt::Horizontal, tr("Select a project"));
	model.setRowCount(list.size());
	for(int row=0; row<list.size(); ++row)
		model.setData(model.index(row, 0), list[row]);

    // associate the view with the model
	ui->tableView->setModel(&model);
	ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

ProjectsDlg::~ProjectsDlg() {
	delete ui;
}

QString ProjectsDlg::getSelectedProject() const
{
	QString result;
	QModelIndexList rows = ui->tableView->selectionModel()->selectedRows();
	if(!rows.isEmpty())
		result = model.data(rows.front()).toString();
	return result;
}

}
