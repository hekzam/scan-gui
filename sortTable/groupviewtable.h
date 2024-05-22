#ifndef GROUPVIEWTABLE_H
#define GROUPVIEWTABLE_H

#include "sorttable.h"

class GroupViewTable : public SortTable
{
public:

    explicit GroupViewTable(std::map<QString, SubjectInfo>& copies, QWidget *parent = nullptr);
    void initSortTable() override;

private:

    void insertSubject(int& ligne, SubjectInfo& subject);
    void insertCopy(int& line, SubjectInfo& subject, CopyInfo& copy);
    void insertPage(int& line, SubjectInfo& subject, CopyInfo& copy, PageInfo& page);
    void insertField(int& line, SubjectInfo& subject, CopyInfo& copy, PageInfo& page, FieldInfo& field);

};

#endif // GROUPVIEWTABLE_H
