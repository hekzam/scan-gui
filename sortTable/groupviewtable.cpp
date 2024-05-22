#include "groupviewtable.h"

GroupViewTable::GroupViewTable(std::map<QString, SubjectInfo>& copies, QWidget *parent) : SortTable(copies, parent){
    setSortingEnabled(false);
}

void GroupViewTable::insertField(int& line, SubjectInfo& subject, CopyInfo& copy, PageInfo& page, FieldInfo& field){
    int progress = std::rand() % 101;
    int semantique = std::rand() % 2;
    field.setSyntax(progress);
    field.setValue("value");

    QTableWidgetItem *subjectItem = new QTableWidgetItem(subject.getSubjectName());
    subjectItem->setData(Qt::UserRole,QVariant::fromValue(&subject));
    QTableWidgetItem *copyItem = new QTableWidgetItem(copy.getCopyName());
    copyItem->setData(Qt::UserRole,QVariant::fromValue(&copy));
    QTableWidgetItem *pageItem = new QTableWidgetItem(page.getPageName());
    pageItem->setData(Qt::UserRole,QVariant::fromValue(&page));
    QTableWidgetItem *fieldItem = new QTableWidgetItem(field.getFieldName());
    fieldItem->setData(Qt::UserRole,QVariant::fromValue(&field));
    ProgressCell *progression = new ProgressCell(progress,this);

    insertRow(line);
    setItem(line,COL_SUBJECT, subjectItem);
    setItem(line,COL_COPY, copyItem);
    setItem(line,COL_PAGE, pageItem);
    setItem(line,COL_FIELD, fieldItem);
    setCellWidget(line,COL_SYNTAX, progression);
    setItem(line,COL_SYNTAX, progression);
    setItem(line,COL_SEMANTIC,new QTableWidgetItem(QString::number(semantique)));
    line++;
}


void GroupViewTable::insertPage(int& line, SubjectInfo& subject, CopyInfo& copy, PageInfo& page){
    int lineRefPage(line);

    if (!page.getPageInJSON()){ //This will occur if a selected page was not mentionned in any JSON file        
        QTableWidgetItem *copyItem = new QTableWidgetItem(copy.getCopyName());
        copyItem->setData(Qt::UserRole,QVariant::fromValue(&copy));
        QTableWidgetItem *pageItem = new QTableWidgetItem(page.getPageName() + " not in JSON data!");
        pageItem->setData(Qt::UserRole,QVariant::fromValue(&page));

        insertRow(line);
        setItem(line,COL_COPY, copyItem);
        setItem(line,COL_PAGE, pageItem);
        line ++;

        //Error message
        qDebug() << "The file " << copy.getCopyName() + "-" + page.getPageName() << " is not associated with any JSON file.";
    }
    else if (!page.getPageInFiles()){ //This will occur if a page was mentionned in a JSON file but was never selected
        QTableWidgetItem *copyItem = new QTableWidgetItem(copy.getCopyName());
        copyItem->setData(Qt::UserRole,QVariant::fromValue(&copy));
        QTableWidgetItem *pageItem = new QTableWidgetItem(page.getPageName() + " not found!");
        pageItem->setData(Qt::UserRole,QVariant::fromValue(&page));

        insertRow(line);
        setItem(line,COL_COPY, copyItem);
        setItem(line,COL_PAGE, pageItem);
        line ++;

        //Error message
        qDebug() << "The file " << copy.getCopyName() + "-" + page.getPageName() << " was specified in a JSON file but was not selected.";
    }
    else{
        for (auto it = page.begin(); it != page.end(); it++){
            FieldInfo& field = it->second;
            insertField(line,subject,copy,page,field);
        }
    }
    if (line - lineRefPage >= 2)
        setSpan(lineRefPage,COL_PAGE,line-lineRefPage,1);
}


void GroupViewTable::insertCopy(int& line, SubjectInfo& subject, CopyInfo& copy){
    int lineRefCopy(line);

    if (!copy.getCopyInJSON()){ //This will occur if a selected page was not mentionned in any JSON file
        QTableWidgetItem *subjectItem = new QTableWidgetItem(subject.getSubjectName());
        subjectItem->setData(Qt::UserRole,QVariant::fromValue(&subject));
        QTableWidgetItem *copyItem = new QTableWidgetItem(copy.getCopyName() + " not in JSON data!");
        copyItem->setData(Qt::UserRole,QVariant::fromValue(&copy));

        insertRow(line);
        setItem(line,COL_SUBJECT, subjectItem);
        setItem(line,COL_COPY, copyItem);
        line ++;

        //Error message
        qDebug() << "The file " << subject.getSubjectName() + "-" + copy.getCopyName() << " is not associated with any JSON file.";
    }
    else if (!copy.getCopyInFiles()){ //This will occur if a page was mentionned in a JSON file but was never selected
        QTableWidgetItem *subjectItem = new QTableWidgetItem(subject.getSubjectName());
        subjectItem->setData(Qt::UserRole,QVariant::fromValue(&subject));
        QTableWidgetItem *copyItem = new QTableWidgetItem(copy.getCopyName() + " not found!");
        copyItem->setData(Qt::UserRole,QVariant::fromValue(&copy));

        insertRow(line);
        setItem(line,COL_SUBJECT, subjectItem);
        setItem(line,COL_COPY, copyItem);
        line ++;

        //Error message
        qDebug() << "The file " << subject.getSubjectName() + "-" + copy.getCopyName() << " was specified in a JSON file but was not selected.";
    }
    else{
        for (auto it = copy.begin(); it != copy.end(); it++){
            PageInfo& page = it->second;
            insertPage(line,subject, copy, page);
        }
    }
    if (line - lineRefCopy >= 2)
        setSpan(lineRefCopy,COL_COPY,line-lineRefCopy,1);

}

void GroupViewTable::insertSubject(int& line, SubjectInfo& subject){
    int lineRefSubject(line);

    for (auto subjectIt = subject.begin(); subjectIt != subject.end(); subjectIt++) {
        CopyInfo& copy = subjectIt->second;
        insertCopy(line, subject, copy);
    }

    if (line - lineRefSubject >= 2)
        setSpan(lineRefSubject,COL_SUBJECT,line-lineRefSubject,1);
}

void GroupViewTable::initSortTable(){
    std::srand(std::time(nullptr));
    int line(0);
    for (auto it = subjectMap.begin(); it != subjectMap.end(); it++) {
        SubjectInfo& subject = it->second;
        insertSubject(line,subject);
    }
    resizeColumnsToContents();
}




