#include "sorttable.h"

SortTable::SortTable(QWidget *parent) : QTableWidget(parent) {
    setColumnCount(NB_COL);
    hideColumn(COL_PATH);
    hideColumn(COL_MET1);
    hideColumn(COL_MET2);
    headerList << "Paths" << "Copy" << "Page" << "Field" << "Syntax" << "Semantic" << "Metric 1" << "Metric 2";
    setHorizontalHeaderLabels(headerList);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setShowGrid(true);
    setGridStyle(Qt::DotLine);
    setSortingEnabled(true);
}

SortTable::~SortTable(){
    for (QFile *fichier : fileList)
        delete fichier;
}

void SortTable::initSortTable(QList<JsonLinker::fieldInfo> const& fields){
    std::srand(std::time(nullptr));
    int ligne(0);
    setRowCount(fields.size());
    for(JsonLinker::fieldInfo const& field : fields){
        //qDebug() << path.filePath;
        int progress = std::rand() % 101;
        int semantique = std::rand() % 2;
        ProgressCell *progression = new ProgressCell(progress,this);
        setItem(ligne,COL_PATH, new QTableWidgetItem(field.m_filePath));
        setItem(ligne,COL_COPY, new QTableWidgetItem(field.m_copyName));
        setItem(ligne,COL_PAGE,new QTableWidgetItem(field.m_pageName));
        setItem(ligne,COL_FIELD,new QTableWidgetItem(field.m_fieldName));
        setCellWidget(ligne,COL_SYNTAX, progression);
        setItem(ligne,COL_SYNTAX, progression);
        setItem(ligne,COL_SEMANTIC,new QTableWidgetItem(QString::number(semantique)));
        ligne++;
    }
    resizeColumnsToContents();
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
