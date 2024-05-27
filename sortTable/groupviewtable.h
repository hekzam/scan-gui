#ifndef GROUPVIEWTABLE_H
#define GROUPVIEWTABLE_H

#include "sorttable.h"

//Items regroupés

class GroupViewTable : public SortTable
{
public:
  explicit GroupViewTable(std::map<QString, ExamInfo> &copies,
                          QWidget *parent = nullptr);
  void initSortTable() override;

private:
  void insertExam(int &ligne, ExamInfo &exam);
  void insertCopy(int &ligne, ExamInfo &exam, CopyInfo &copy);
  void insertPage(int &line, ExamInfo &exam, CopyInfo &copy,
                  PageInfo &page, int& maxSyntax);
  void insertField(int &line, ExamInfo &exam, CopyInfo &copy,
                   PageInfo &page, FieldInfo &field, int& maxSyntax);
};

#endif // GROUPVIEWTABLE_H
