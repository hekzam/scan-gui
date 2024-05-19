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
    setGridStyle(Qt::SolidLine);
    setSortingEnabled(true);
}

SortTable::~SortTable(){
    for (QFile *fichier : fileList)
        delete fichier;
}


void SortTable::insertField(int& line, FieldInfo const& field, PageInfo const& page, CopyInfo const& copy){
    int progress = std::rand() % 101;
    int semantique = std::rand() % 2;
    QTableWidgetItem *copyItem = new QTableWidgetItem(copy.m_copyName);
    copyItem->setData(Qt::UserRole,QVariant::fromValue(copy));
    QTableWidgetItem *pageItem = new QTableWidgetItem(page.m_pageName);
    pageItem->setData(Qt::UserRole,QVariant::fromValue(copy));
    QTableWidgetItem *fieldItem = new QTableWidgetItem(field.m_fieldName);
    fieldItem->setData(Qt::UserRole,QVariant::fromValue(copy));
    ProgressCell *progression = new ProgressCell(progress,this);

    insertRow(line);
    setItem(line,COL_COPY, copyItem);
    setItem(line,COL_PAGE, pageItem);
    setItem(line,COL_FIELD, fieldItem);
    setCellWidget(line,COL_SYNTAX, progression);
    setItem(line,COL_SYNTAX, progression);
    setItem(line,COL_SEMANTIC,new QTableWidgetItem(QString::number(semantique)));
}


int SortTable::insertPage(int& line, PageInfo const& page, CopyInfo const& copy){
    int nbFieldPage(0);


    if (!page.m_inJSON){ //This will occur if a selected page was not mentionned in any JSON file
        insertRow(line);
        QTableWidgetItem *copyItem = new QTableWidgetItem(copy.m_copyName);
        setItem(line,COL_COPY, copyItem);
        setItem(line,COL_PAGE, new QTableWidgetItem(QString(page.m_pageName + " not in JSON data!")));
        line++;
        nbFieldPage++;

        //Error message
        qDebug() << "The file " << page.m_filePath.section("/",-1) << " is not associated with any JSON file.";
    }
    else if (page.m_filePath == ""){ //This will occur if a page was mentionned in a JSON file but was never selected
        insertRow(line);
        QTableWidgetItem *copyItem = new QTableWidgetItem(copy.m_copyName);
        setItem(line,COL_COPY, copyItem);
        setItem(line,COL_PAGE, new QTableWidgetItem(QString(page.m_pageName + " not found!")));
        line++;
        nbFieldPage++;

        //Error message
        qDebug() << "The file " << copy.m_copyName + "-" + page.m_pageName << " was specified in a JSON file but was not selected.";
    }
    else{
        for (FieldInfo const& field : page.m_pageFieldMap){
            insertField(line,field,page,copy);
            line++;
            nbFieldPage++;
        }
        int pageIndex(line - nbFieldPage);
        if (nbFieldPage >= 2)
            setSpan(pageIndex,COL_PAGE,nbFieldPage,1);
    }

    return nbFieldPage;
}


void SortTable::insertCopy(int& line, CopyInfo const& copy){
    int nbField(0);

    for(PageInfo const& page : copy.m_copyPageMap){
        nbField += insertPage(line,page,copy);
    }
    int copyIndex(line - nbField);
    if (nbField >= 2)
        setSpan(copyIndex,COL_COPY,nbField,1);
}


void SortTable::initSortTable(QMap<QString, CopyInfo> const& copies){
    std::srand(std::time(nullptr));
    int line(0);
    for(CopyInfo const& copy : copies){
        insertCopy(line,copy);
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
