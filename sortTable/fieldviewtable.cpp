#include "FieldViewTable.h"

FieldViewTable::FieldViewTable(std::map<QString, CopyInfo>& copies, QWidget *parent) : SortTable(copies, parent){}

void FieldViewTable::insertField(int& line, FieldInfo const& field, PageInfo const& page, CopyInfo& copy){
    int progress = std::rand() % 101;
    int semantique = std::rand() % 2;
    QTableWidgetItem *copyItem = new QTableWidgetItem(copy.m_copyName);
    copyItem->setData(Qt::UserRole,QVariant::fromValue(&copy));
    QTableWidgetItem *pageItem = new QTableWidgetItem(page.m_pageName);
    QTableWidgetItem *fieldItem = new QTableWidgetItem(field.m_fieldName);
    ProgressCell *progression = new ProgressCell(progress,this);

    insertRow(line);
    setItem(line,COL_COPY, copyItem);
    setItem(line,COL_PAGE, pageItem);
    setItem(line,COL_FIELD, fieldItem);
    setCellWidget(line,COL_SYNTAX, progression);
    setItem(line,COL_SYNTAX, progression);
    setItem(line,COL_SEMANTIC,new QTableWidgetItem(QString::number(semantique)));
}


void FieldViewTable::insertPage(int& line, PageInfo const& page, CopyInfo& copy){
    if (!page.m_inJSON){ //This will occur if a selected page was not mentionned in any JSON file
        insertRow(line);
        QTableWidgetItem *copyItem = new QTableWidgetItem(copy.m_copyName);
        copyItem->setData(Qt::UserRole,QVariant::fromValue(&copy));
        setItem(line,COL_COPY, copyItem);
        setItem(line,COL_PAGE, new QTableWidgetItem(QString(page.m_pageName + " not in JSON data!")));
        line ++;
    }
    else if (page.m_filePath == ""){ //This will occur if a page was mentionned in a JSON file but was never selected
        insertRow(line);
        QTableWidgetItem *copyItem = new QTableWidgetItem(copy.m_copyName);
        copyItem->setData(Qt::UserRole,QVariant::fromValue(&copy));
        setItem(line,COL_COPY, copyItem);
        setItem(line,COL_PAGE, new QTableWidgetItem(QString(page.m_pageName + " not found!")));
        line ++;
    }
    else{
        for (auto it = page.m_pageFieldMap.cbegin(); it != page.m_pageFieldMap.cend(); it++){
            FieldInfo const& field = it->second;
            insertField(line,field,page,copy);
            line++;
        }
    }
}


void FieldViewTable::insertCopy(int& line, CopyInfo& copy){
    for(auto it = copy.m_copyPageMap.cbegin(); it != copy.m_copyPageMap.cend(); it++){
        PageInfo const& page = it->second;
        insertPage(line,page,copy);
    }

}


void FieldViewTable::initSortTable(){
    std::srand(std::time(nullptr));
    int line(0);
    for (auto it = copyMap.begin(); it != copyMap.end(); it++) {
        CopyInfo& copy = it->second;
        insertCopy(line,copy);
    }
    resizeColumnsToContents();
}
