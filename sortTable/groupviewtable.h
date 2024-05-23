#ifndef GROUPVIEWTABLE_H
#define GROUPVIEWTABLE_H

#include "sorttable.h"

class GroupViewTable : public SortTable
{
public:

    explicit GroupViewTable(std::map<QString, SubjectInfo>& copies, QWidget *parent = nullptr);
    void initSortTable() override;

private:

    void insertSubject(int& ligne, SubjectInfo& subject) override;
    void insertCopy(int& line, SubjectInfo& subject, CopyInfo& copy) override;
    void insertPage(int& line, SubjectInfo& subject, CopyInfo& copy, PageInfo& page) override;
    void insertField(int& line, SubjectInfo& subject, CopyInfo& copy, PageInfo& page, FieldInfo& field) override;

};

#endif // GROUPVIEWTABLE_H
