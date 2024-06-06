#include "fieldviewtable.h"

FieldViewTable::FieldViewTable(std::map<QString, ExamInfo> &copies,
                               QWidget *parent)
    : SortTable(copies, parent)
{
  setSortingEnabled(true);
}

void FieldViewTable::insertField(int &line, ExamInfo &exam,
                                 CopyInfo &copy, PageInfo &page,
                                 FieldInfo &field)
{
  int progress = std::rand() % 101;
  field.setSyntax(progress);
  field.setValue("value");

  QTableWidgetItem *examItem =
      new QTableWidgetItem(exam.getExamName());
  examItem->setData(Qt::UserRole, QVariant::fromValue(&exam));
  QTableWidgetItem *copyItem = new QTableWidgetItem(copy.getCopyName());
  copyItem->setData(Qt::UserRole, QVariant::fromValue(&copy));
  QTableWidgetItem *pageItem = new QTableWidgetItem(page.getPageName());
  pageItem->setData(Qt::UserRole, QVariant::fromValue(&page));
  QTableWidgetItem *fieldItem = new QTableWidgetItem(field.getFieldName());
  fieldItem->setData(Qt::UserRole, QVariant::fromValue(&page));
  ProgressCell *progression = new ProgressCell(progress, this);

  insertRow(line);
  setItem(line, COL_EXAM, examItem);
  setItem(line, COL_COPY, copyItem);
  setItem(line, COL_PAGE, pageItem);
  setItem(line, COL_FIELD, fieldItem);
  setCellWidget(line, COL_SYNTAX, progression);
  setItem(line, COL_SYNTAX, progression);
  line++;
}

void FieldViewTable::insertPage(int &line, ExamInfo &exam, CopyInfo &copy,
                                PageInfo &page)
{
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

    // Error message
    QString fileError = "The file : " + exam.getExamName() + "-" + copy.getCopyName() + "-" +
                        page.getPageName() +
                        " is not associated with any JSON file.";
    addErrors(fileError);
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

    // Error message
    QString fileError = "The file : " + exam.getExamName() + "-" + copy.getCopyName() + "-" +
                        page.getPageName() +
                        " was specified in a JSON file but was not selected.";
    addErrors(fileError);
  }
  else
  {
    for (auto it = page.begin(); it != page.end(); it++)
    {
      FieldInfo &field = it->second;
      insertField(line, exam, copy, page, field);
    }
  }
}

void FieldViewTable::insertCopy(int &line, ExamInfo &exam, CopyInfo &copy)
{
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
    // Error message
    QString fileError = "The file : " + exam.getExamName() + "-" +
                        copy.getCopyName() +
                        " is not associated with any JSON file.";
    addErrors(fileError);
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

    // Error message
    QString fileError = "The file : " + exam.getExamName() + "-" +
                        copy.getCopyName() +
                        " was specified in a JSON file but was not selected.";
    addErrors(fileError);
  }
  else
  {
    for (auto it = copy.begin(); it != copy.end(); it++)
    {
      PageInfo &page = it->second;
      insertPage(line, exam, copy, page);
    }
  }
}

void FieldViewTable::insertExam(int &line, ExamInfo &exam)
{
  for (auto examIt = exam.begin(); examIt != exam.end();
       examIt++)
  {
    CopyInfo &copy = examIt->second;
    insertCopy(line, exam, copy);
  }
}

void FieldViewTable::initSortTable()
{
  std::srand(std::time(nullptr));
  int line(0);
  for (auto it = examMap.begin(); it != examMap.end(); it++)
  {
    ExamInfo &exam = it->second;
    insertExam(line, exam);
  }
  resizeColumnsToContents();
}

void FieldViewTable::addErrors(QString &newErrors)
{

  errors.push_back(newErrors);
  // getErrors()).push_back(newErrors);
}

QStringList FieldViewTable::getErrors()
{
  return errors;
}
