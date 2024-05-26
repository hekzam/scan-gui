#ifndef GROUPVIEWTABLE_H
#define GROUPVIEWTABLE_H

#include "sorttable.h"

class GroupViewTable : public SortTable
{
public:
  explicit GroupViewTable(std::map<QString, SubjectInfo> &copies,
                          QWidget *parent = nullptr);
  void initSortTable() override;

private:
  void insertSubject(int &ligne, SubjectInfo &subject);
  void insertCopy(int &ligne, SubjectInfo &subject, CopyInfo &copy);
  void insertPage(int &line, SubjectInfo &subject, CopyInfo &copy,
                  PageInfo &page, int& maxSyntax);
  void insertField(int &line, SubjectInfo &subject, CopyInfo &copy,
                   PageInfo &page, FieldInfo &field, int& maxSyntax);
};

#endif // GROUPVIEWTABLE_H
