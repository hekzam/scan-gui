#include "sorttable.h"

SortTable::SortTable(std::map<QString, ExamInfo> &exams, QWidget *parent)
    : examMap(exams), QTableWidget(parent)
{
  setColumnCount(NB_COL);
  hideColumn(COL_MET1);
  hideColumn(COL_MET2);
  headerList << "Exam"
             << "Copy"
             << "Page"
             << "Field"
             << "Syntax"
             << "Metric 1"
             << "Metric 2";
  setHorizontalHeaderLabels(headerList);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setShowGrid(true);
  setGridStyle(Qt::SolidLine);
}

void SortTable::editColumn(int checkedState, int column)
{
  if (checkedState == Qt::Checked)
    showColumn(column);
  else
    hideColumn(column);
}

QStringList SortTable::getHeaderList()
{
  return headerList;
}

QStringList SortTable::getErrors()
{
  return errors;
}

void SortTable::addErrors(QString &newErrors)
{
  getErrors().push_back(newErrors);
}
