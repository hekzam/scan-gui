#ifndef SORTTABLE_H
#define SORTTABLE_H

#include <QTableWidget>
#include <QList>
#include <QFile>
#include <QHeaderView>
#include "progresscell.h"
#include "../json/jsonlinker.h"

class SortTable : public QTableWidget
{
    Q_OBJECT
public:
    static int const NB_COL = 6;
    static int const COL_PATH = 0;
    static int const COL_NAME = 1;
    static int const COL_SYNTAX = 2;
    static int const COL_SEMANTIC = 3;
    static int const COL_MET1 = 4;
    static int const COL_MET2 = 5;

    explicit SortTable(QWidget *parent=nullptr);
    void initSortTable(std::vector<JsonLinker::infoPage> paths);
    ~SortTable();

private:
    QList<QFile*> fileList;
    QStringList headerList;

public slots:
    void editColumn(int checkState, int column);
    QStringList getHeaderList();
};

#endif // SORTTABLE_H
