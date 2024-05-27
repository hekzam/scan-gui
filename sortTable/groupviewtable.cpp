#include "groupviewtable.h"

GroupViewTable::GroupViewTable(std::map<QString, ExamInfo> &copies,
                               QWidget *parent)
    : SortTable(copies, parent)
{
  setSortingEnabled(false);
}

void GroupViewTable::insertField(int &line, ExamInfo &exam,
                                 CopyInfo &copy, PageInfo &page,
                                 FieldInfo &field, int& maxSyntax)
{
  int syntax = std::rand() % 101;
  int semantique = std::rand() % 2;
  field.setSyntax(syntax);
  field.setValue("value");

  if (syntax > maxSyntax)
    maxSyntax = syntax;

  QTableWidgetItem *examItem =
      new QTableWidgetItem(exam.getExamName());
  examItem->setData(Qt::UserRole, QVariant::fromValue(&exam));
  QTableWidgetItem *copyItem = new QTableWidgetItem(copy.getCopyName());
  copyItem->setData(Qt::UserRole, QVariant::fromValue(&copy));
  QTableWidgetItem *pageItem = new QTableWidgetItem(page.getPageName());
  pageItem->setData(Qt::UserRole, QVariant::fromValue(&page));
  QTableWidgetItem *fieldItem = new QTableWidgetItem(field.getFieldName());
  fieldItem->setData(Qt::UserRole, QVariant::fromValue(&page));

  insertRow(line);
  setItem(line, COL_EXAM, examItem);
  setItem(line, COL_COPY, copyItem);
  setItem(line, COL_PAGE, pageItem);
  setItem(line, COL_FIELD, fieldItem);
  line++;
}

void GroupViewTable::insertPage(int &line, ExamInfo &exam, CopyInfo &copy,
                                PageInfo &page, int& maxSyntax)
{
  int lineRefPage(line);

  if (!page.pageIsInJSON())
  { // This will occur if a selected page was not mentionned in any JSON file
    QTableWidgetItem *examItem =
        new QTableWidgetItem(exam.getExamName());
    examItem->setData(Qt::UserRole, QVariant::fromValue(&exam));
    QTableWidgetItem *copyItem = new QTableWidgetItem(copy.getCopyName());
    copyItem->setData(Qt::UserRole, QVariant::fromValue(&copy));
    QTableWidgetItem *pageItem =
        new QTableWidgetItem(page.getPageName() + " not in JSON data!");
    pageItem->setData(Qt::UserRole, QVariant::fromValue(&page));

    insertRow(line);
    setItem(line, COL_EXAM, examItem);
    setItem(line, COL_COPY, copyItem);
    setItem(line, COL_PAGE, pageItem);
    line++;
  }
  else if (!page.pageIsInFiles())
  { // This will occur if a page was mentionned in a JSON file but was never
    // selected
    QTableWidgetItem *examItem =
        new QTableWidgetItem(exam.getExamName());
    examItem->setData(Qt::UserRole, QVariant::fromValue(&exam));
    QTableWidgetItem *copyItem = new QTableWidgetItem(copy.getCopyName());
    copyItem->setData(Qt::UserRole, QVariant::fromValue(&copy));
    QTableWidgetItem *pageItem =
        new QTableWidgetItem(page.getPageName() + " not found!");
    pageItem->setData(Qt::UserRole, QVariant::fromValue(&page));

    insertRow(line);
    setItem(line, COL_EXAM, examItem);
    setItem(line, COL_COPY, copyItem);
    setItem(line, COL_PAGE, pageItem);
    line++;
  }
  else
  {
    for (auto it = page.begin(); it != page.end(); it++)
    {
      FieldInfo &field = it->second;
      insertField(line, exam, copy, page, field, maxSyntax);
    }
  }
  if (line - lineRefPage >= 2)
    setSpan(lineRefPage, COL_PAGE, line - lineRefPage, 1);
}

void GroupViewTable::insertCopy(int &line, ExamInfo &exam, CopyInfo &copy)
{
  int lineRefCopySyntax(line);

  if (!copy.copyIsInJSON())
  { // This will occur if a selected page was not mentionned in any JSON file
    QTableWidgetItem *examItem =
        new QTableWidgetItem(exam.getExamName());
    examItem->setData(Qt::UserRole, QVariant::fromValue(&exam));
    QTableWidgetItem *copyItem =
        new QTableWidgetItem(copy.getCopyName() + " not in JSON data!");
    copyItem->setData(Qt::UserRole, QVariant::fromValue(&copy));

    insertRow(line);
    setItem(line, COL_EXAM, examItem);
    setItem(line, COL_COPY, copyItem);
    line++;
  }
  else if (!copy.copyIsInFiles())
  { // This will occur if a page was mentionned in a JSON file but was never
    // selected
    QTableWidgetItem *examItem =
        new QTableWidgetItem(exam.getExamName());
    examItem->setData(Qt::UserRole, QVariant::fromValue(&exam));
    QTableWidgetItem *copyItem =
        new QTableWidgetItem(copy.getCopyName() + " not found!");
    copyItem->setData(Qt::UserRole, QVariant::fromValue(&copy));

    insertRow(line);
    setItem(line, COL_EXAM, examItem);
    setItem(line, COL_COPY, copyItem);
    line++;
  }
  else
  {
    int maxSyntax = 0;
    for (auto it = copy.begin(); it != copy.end(); it++)
    {
      PageInfo &page = it->second;
      insertPage(line, exam, copy, page, maxSyntax);
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

void GroupViewTable::insertExam(int &line, ExamInfo &exam)
{
  int lineRefExam(line);

  for (auto examIt = exam.begin(); examIt != exam.end();
       examIt++)
  {
    CopyInfo &copy = examIt->second;
    insertCopy(line, exam, copy);
  }

  if (line - lineRefExam >= 2)
    setSpan(lineRefExam, COL_EXAM, line - lineRefExam, 1);
}

void GroupViewTable::initSortTable()
{
  std::srand(std::time(nullptr));
  int line(0);
  for (auto it = examMap.begin(); it != examMap.end(); it++)
  {
    ExamInfo &exam = it->second;
    insertExam(line, exam);
  }
  resizeColumnsToContents();
  setColumnWidth(COL_SYNTAX,96);
}
