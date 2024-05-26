#include "groupviewtable.h"

GroupViewTable::GroupViewTable(std::map<QString, SubjectInfo> &copies,
                               QWidget *parent)
    : SortTable(copies, parent)
{
  setSortingEnabled(false);
}

void GroupViewTable::insertField(int &line, SubjectInfo &subject,
                                 CopyInfo &copy, PageInfo &page,
                                 FieldInfo &field, int& maxSyntax)
{
  int syntax = std::rand() % 101;
  int semantique = std::rand() % 2;
  field.setSyntax(syntax);
  field.setValue("value");

  if (syntax > maxSyntax)
    maxSyntax = syntax;

  QTableWidgetItem *subjectItem =
      new QTableWidgetItem(subject.getSubjectName());
  subjectItem->setData(Qt::UserRole, QVariant::fromValue(&subject));
  QTableWidgetItem *copyItem = new QTableWidgetItem(copy.getCopyName());
  copyItem->setData(Qt::UserRole, QVariant::fromValue(&copy));
  QTableWidgetItem *pageItem = new QTableWidgetItem(page.getPageName());
  pageItem->setData(Qt::UserRole, QVariant::fromValue(&page));
  QTableWidgetItem *fieldItem = new QTableWidgetItem(field.getFieldName());
  fieldItem->setData(Qt::UserRole, QVariant::fromValue(&page));

  insertRow(line);
  setItem(line, COL_SUBJECT, subjectItem);
  setItem(line, COL_COPY, copyItem);
  setItem(line, COL_PAGE, pageItem);
  setItem(line, COL_FIELD, fieldItem);
  line++;
}

void GroupViewTable::insertPage(int &line, SubjectInfo &subject, CopyInfo &copy,
                                PageInfo &page, int& maxSyntax)
{
  int lineRefPage(line);

  if (!page.pageIsInJSON())
  { // This will occur if a selected page was not mentionned in any JSON file
    QTableWidgetItem *subjectItem =
        new QTableWidgetItem(subject.getSubjectName());
    subjectItem->setData(Qt::UserRole, QVariant::fromValue(&subject));
    QTableWidgetItem *copyItem = new QTableWidgetItem(copy.getCopyName());
    copyItem->setData(Qt::UserRole, QVariant::fromValue(&copy));
    QTableWidgetItem *pageItem =
        new QTableWidgetItem(page.getPageName() + " not in JSON data!");
    pageItem->setData(Qt::UserRole, QVariant::fromValue(&page));

    insertRow(line);
    setItem(line, COL_SUBJECT, subjectItem);
    setItem(line, COL_COPY, copyItem);
    setItem(line, COL_PAGE, pageItem);
    line++;

    // Error message
    qDebug() << "The file " << copy.getCopyName() + "-" + page.getPageName()
             << " is not associated with any JSON file.";
  }
  else if (!page.pageIsInFiles())
  { // This will occur if a page was mentionned in a JSON file but was never
    // selected
    QTableWidgetItem *subjectItem =
        new QTableWidgetItem(subject.getSubjectName());
    subjectItem->setData(Qt::UserRole, QVariant::fromValue(&subject));
    QTableWidgetItem *copyItem = new QTableWidgetItem(copy.getCopyName());
    copyItem->setData(Qt::UserRole, QVariant::fromValue(&copy));
    QTableWidgetItem *pageItem =
        new QTableWidgetItem(page.getPageName() + " not found!");
    pageItem->setData(Qt::UserRole, QVariant::fromValue(&page));

    insertRow(line);
    setItem(line, COL_SUBJECT, subjectItem);
    setItem(line, COL_COPY, copyItem);
    setItem(line, COL_PAGE, pageItem);
    line++;

    // Error message
    qDebug() << "The file " << copy.getCopyName() + "-" + page.getPageName()
             << " was specified in a JSON file but was not selected.";
  }
  else
  {
    for (auto it = page.begin(); it != page.end(); it++)
    {
      FieldInfo &field = it->second;
      insertField(line, subject, copy, page, field, maxSyntax);
    }
  }
  if (line - lineRefPage >= 2)
    setSpan(lineRefPage, COL_PAGE, line - lineRefPage, 1);
}

void GroupViewTable::insertCopy(int &line, SubjectInfo &subject, CopyInfo &copy)
{
  int lineRefCopySyntax(line);

  if (!copy.copyIsInJSON())
  { // This will occur if a selected page was not mentionned in any JSON file
    QTableWidgetItem *subjectItem =
        new QTableWidgetItem(subject.getSubjectName());
    subjectItem->setData(Qt::UserRole, QVariant::fromValue(&subject));
    QTableWidgetItem *copyItem =
        new QTableWidgetItem(copy.getCopyName() + " not in JSON data!");
    copyItem->setData(Qt::UserRole, QVariant::fromValue(&copy));

    insertRow(line);
    setItem(line, COL_SUBJECT, subjectItem);
    setItem(line, COL_COPY, copyItem);
    line++;

    // Error message
    qDebug() << "The file "
             << subject.getSubjectName() + "-" + copy.getCopyName()
             << " is not associated with any JSON file.";
  }
  else if (!copy.copyIsInFiles())
  { // This will occur if a page was mentionned in a JSON file but was never
    // selected
    QTableWidgetItem *subjectItem =
        new QTableWidgetItem(subject.getSubjectName());
    subjectItem->setData(Qt::UserRole, QVariant::fromValue(&subject));
    QTableWidgetItem *copyItem =
        new QTableWidgetItem(copy.getCopyName() + " not found!");
    copyItem->setData(Qt::UserRole, QVariant::fromValue(&copy));

    insertRow(line);
    setItem(line, COL_SUBJECT, subjectItem);
    setItem(line, COL_COPY, copyItem);
    line++;

    // Error message
    qDebug() << "The file "
             << subject.getSubjectName() + "-" + copy.getCopyName()
             << " was specified in a JSON file but was not selected.";
  }
  else
  {
    int maxSyntax = 0;
    for (auto it = copy.begin(); it != copy.end(); it++)
    {
      PageInfo &page = it->second;
      insertPage(line, subject, copy, page, maxSyntax);
    }
    ProgressCell *progression = new ProgressCell(maxSyntax, this);
    setCellWidget(lineRefCopySyntax, COL_SYNTAX, progression);
    setItem(lineRefCopySyntax, COL_SYNTAX, progression);
  }
  if (line - lineRefCopySyntax >= 2){
    setSpan(lineRefCopySyntax, COL_COPY, line - lineRefCopySyntax, 1);
    setSpan(lineRefCopySyntax, COL_SYNTAX, line - lineRefCopySyntax, 1);
  }
}

void GroupViewTable::insertSubject(int &line, SubjectInfo &subject)
{
  int lineRefSubject(line);

  for (auto subjectIt = subject.begin(); subjectIt != subject.end();
       subjectIt++)
  {
    CopyInfo &copy = subjectIt->second;
    insertCopy(line, subject, copy);
  }

  if (line - lineRefSubject >= 2)
    setSpan(lineRefSubject, COL_SUBJECT, line - lineRefSubject, 1);
}

void GroupViewTable::initSortTable()
{
  std::srand(std::time(nullptr));
  int line(0);
  for (auto it = subjectMap.begin(); it != subjectMap.end(); it++)
  {
    SubjectInfo &subject = it->second;
    insertSubject(line, subject);
  }
  resizeColumnsToContents();
  setColumnWidth(COL_SYNTAX,96);
}
