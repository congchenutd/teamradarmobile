#ifndef PROJECTSDLG_H
#define PROJECTSDLG_H

#include <QDialog>
#include <QStringListModel>
#include <QStandardItemModel>

namespace Ui {
	class ProjectsDlg;
}

namespace TeamRadar {

class ProjectsDlg : public QDialog
{
    Q_OBJECT

public:
	explicit ProjectsDlg(const QStringList& list, QWidget *parent = 0);
    ~ProjectsDlg();
	QString getProject() const;

private:
    Ui::ProjectsDlg *ui;
	QStandardItemModel model;
	QString project;
};

}

#endif // PROJECTSDLG_H
