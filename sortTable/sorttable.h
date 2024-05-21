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
        COL_COPY = 0,
        COL_PAGE,
        COL_FIELD,
        COL_SYNTAX,
        COL_SEMANTIC,
        COL_MET1,
        COL_MET2,
        NB_COL
    };

    SortTable(std::map<QString, CopyInfo>& copies, QWidget *parent=nullptr);
    virtual void initSortTable() = 0;

private:
    QStringList headerList;

protected:
    std::map<QString, CopyInfo>& copyMap;

public slots:
    void editColumn(int checkState, int column);
    QStringList getHeaderList();
};

#endif // SORTTABLE_H
