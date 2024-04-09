#ifndef SORTTABLE_H
#define SORTTABLE_H

#include <QTableWidget>
#include <QList>
#include <QFile>
#include <QHeaderView>

class SortTable : public QTableWidget
{
    Q_OBJECT
public:
    static int const NB_COL = 5;
    static int const COL_NOM = 0;
    static int const COL_SYNTAXE = 1;
    static int const COL_SEMANTIQUE = 2;
    static int const COL_MET1 = 3;
    static int const COL_MET2 = 4;

    explicit SortTable(QWidget *parent=nullptr);
    void initSortTable(QStringList const& listeFichiers);
    ~SortTable();

private:
    QList<QFile*> fileList;
    QStringList headerList;

public slots:
    void editNameColumn(int checkedState);
    void editSyntaxColumn(int checkedState);
    void editSemanticColumn(int checkedState);
    void editMetric1Column(int checkedState);
    void editMetric2Column(int checkedState);
};

#endif // SORTTABLE_H
