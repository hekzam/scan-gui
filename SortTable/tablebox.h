#ifndef TABLEBOX_H
#define TABLEBOX_H

#include <QGroupBox>
#include <QStackedLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>
#include <QDockWidget>
#include <QLineEdit>

#include <QDebug>
#include "sorttable.h"

class TableBox : public QGroupBox
{
    Q_OBJECT
public:
    TableBox(QStringList const& fileNames, QWidget *parent = nullptr);

private:
    SortTable *sortTable;
    QDockWidget *sortDock;
    QGroupBox *sortBox;
    QPushButton *sortButton;
    QLineEdit *textZone;
    QString textSearched;
    bool firstAppearence;

    void initTableFilter();
    void initTableView(QStringList const& listeFichiers);

private slots:
    void displayTableFilter();
    void searchProcessing();
    void tagsProcessing();
    void cleanSearchBar();
};

#endif // TABLEBOX_H
