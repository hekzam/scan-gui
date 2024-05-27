#ifndef FIELDVIEWTABLE_H
#define FIELDVIEWTABLE_H

#include "sorttable.h"

// Items non regroupés

class FieldViewTable : public SortTable
{
public:
  explicit FieldViewTable(std::map<QString, ExamInfo> &copies,
                          QWidget *parent = nullptr);
  void initSortTable() override;

private:
  void insertExam(int &ligne, ExamInfo &exam);
  void insertCopy(int &ligne, ExamInfo &exam, CopyInfo &copy);
  void insertPage(int &line, ExamInfo &exam, CopyInfo &copy,
                          PageInfo &page);
  void insertField(int &line, ExamInfo &exam, CopyInfo &copy,
                           PageInfo &page, FieldInfo &field);

  // SortTable interface
protected:
  void addErrors(QString &newErrors) override;
  QStringList getErrors() override;
};

#endif // FieldViewTable_H
