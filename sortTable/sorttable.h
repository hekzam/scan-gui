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
    enum Columns {
        COL_PATH = 0,
        COL_COPY,
        COL_PAGE,
        COL_FIELD,
        COL_SYNTAX,
        COL_SEMANTIC,
        COL_MET1,
        COL_MET2,
        NB_COL
    };

    explicit SortTable(QWidget *parent=nullptr);
    void initSortTable(QList<JsonLinker::fieldInfo> const& fields);
    ~SortTable();

  private:
    QList<QFile*> fileList;
    QStringList headerList;

public slots:
    void editColumn(int checkState, int column);
    QStringList getHeaderList();
};

#endif // SORTTABLE_H
