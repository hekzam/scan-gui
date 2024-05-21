#include "groupviewtable.h"

GroupViewTable::GroupViewTable(std::map<QString, CopyInfo>& copies, QWidget *parent) : SortTable(copies, parent) {}

void GroupViewTable::insertField(int line, FieldInfo& field, PageInfo& page, CopyInfo& copy){
    int progress = std::rand() % 101;
    int semantique = std::rand() % 2;
    field.m_syntax = progress;
    field.m_value = "value";

    QTableWidgetItem *copyItem = new QTableWidgetItem(copy.m_copyName);
    copyItem->setData(Qt::UserRole,QVariant::fromValue(&copy));
    QTableWidgetItem *pageItem = new QTableWidgetItem(page.m_pageName);
    QTableWidgetItem *fieldItem = new QTableWidgetItem(field.m_fieldName);
    ProgressCell *progression = new ProgressCell(progress,this);

    setItem(line,COL_COPY, copyItem);
    setItem(line,COL_PAGE, pageItem);
    setItem(line,COL_FIELD, fieldItem);
    setCellWidget(line,COL_SYNTAX, progression);
    setItem(line,COL_SYNTAX, progression);
    setItem(line,COL_SEMANTIC,new QTableWidgetItem(QString::number(semantique)));
}


void GroupViewTable::insertPage(int& line, PageInfo& page, CopyInfo& copy){
    //qDebug() << "Line : " << line << "Page : " << copy.m_copyName + "-" + page.m_pageName << "Page span : " << page.m_pageSpan;
    if (page.m_pageSpan >= 2)
       setSpan(line,COL_PAGE,page.m_pageSpan,1);

    if (!page.m_inJSON){ //This will occur if a selected page was not mentionned in any JSON file
        QTableWidgetItem *copyItem = new QTableWidgetItem(copy.m_copyName);
        copyItem->setData(Qt::UserRole,QVariant::fromValue(&copy));
        setItem(line,COL_COPY, copyItem);
        setItem(line,COL_PAGE, new QTableWidgetItem(QString(page.m_pageName + " not in JSON data!")));
        line += (page.m_pageSpan) ? page.m_pageSpan : 1;

        //Error message
        qDebug() << "The file " << page.m_filePath.section("/",-1) << " is not associated with any JSON file.";
    }
    else if (page.m_filePath == ""){ //This will occur if a page was mentionned in a JSON file but was never selected
        QTableWidgetItem *copyItem = new QTableWidgetItem(copy.m_copyName);
        copyItem->setData(Qt::UserRole,QVariant::fromValue(&copy));
        setItem(line,COL_COPY, copyItem);
        setItem(line,COL_PAGE, new QTableWidgetItem(QString(page.m_pageName + " not found!")));
        line += (page.m_pageSpan) ? page.m_pageSpan : 1;

        //Error message
        qDebug() << "The file " << copy.m_copyName + "-" + page.m_pageName << " was specified in a JSON file but was not selected.";
    }
    else{
        auto& pageFieldMap = page.m_pageFieldMap;
        for (auto it = pageFieldMap.begin(); it != pageFieldMap.end(); it++){
            FieldInfo& field = it->second;
            insertField(line,field,page,copy);
            line++;
        }
    }
}


void GroupViewTable::insertCopy(int line, CopyInfo& copy){
    //qDebug() << "Line : " << line << "Copy index : " << copyIndex << "Copy span : " << copy.m_copySpan;
    //qDebug() << "Copy : " << copy.m_copyName << "Copy span : " << copy.m_copySpan;
    if (copy.m_copySpan >= 2)
        setSpan(line,COL_COPY,copy.m_copySpan,1);

    auto& copyPageMap = copy.m_copyPageMap;

    for (auto it = copyPageMap.begin(); it != copyPageMap.end(); it++) {
        PageInfo& page = it->second;
        insertPage(line, page, copy);
    }


}


void GroupViewTable::initSortTable(){
    std::srand(std::time(nullptr));
    int line(0);
    int totalCells(0);
    for (auto it = copyMap.begin(); it != copyMap.end(); it++) {
        CopyInfo& copy = it->second;
        totalCells += copy.m_copySpan;
        setRowCount(totalCells);
        insertCopy(line,copy);
        line = totalCells;
    }
    resizeColumnsToContents();
}




