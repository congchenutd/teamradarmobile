#ifndef PROJECTSDLG_H
#define PROJECTSDLG_H

#include <QDialog>
#include <QStringListModel>
#include <QStandardItemModel>

namespace Ui {
	class ProjectsDlg;
}

namespace TeamRadar {

// a dialog letting users select from available projects
class ProjectsDlg : public QDialog
{
    Q_OBJECT

public:
	explicit ProjectsDlg(const QStringList& list, QWidget *parent = 0);
    ~ProjectsDlg();
    QString getSelectedProject() const;

private:
    Ui::ProjectsDlg *ui;
	QStandardItemModel model;
};

}

#endif // PROJECTSDLG_H
