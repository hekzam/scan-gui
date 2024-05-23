#include "sorttable.h"

SortTable::SortTable(std::map<QString, SubjectInfo>& copies, QWidget *parent) : subjectMap(copies), QTableWidget(parent){
    setColumnCount(NB_COL);
    hideColumn(COL_MET1);
    hideColumn(COL_MET2);
    headerList << "Subject" <<"Copy" << "Page" << "Field" << "Syntax" << "Semantic" << "Metric 1" << "Metric 2";
    setHorizontalHeaderLabels(headerList);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setShowGrid(true);
    setGridStyle(Qt::SolidLine);
}


void SortTable::editColumn(int checkedState, int column){
    if (checkedState == Qt::Checked)
        showColumn(column);
    else
        hideColumn(column);
}


QStringList SortTable::getHeaderList()
{
    return headerList;
}
