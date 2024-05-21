#ifndef GROUPVIEWTABLE_H
#define GROUPVIEWTABLE_H

#include "sorttable.h"

class GroupViewTable : public SortTable
{
public:

    explicit GroupViewTable(std::map<QString, CopyInfo>& copies, QWidget *parent = nullptr);
    void initSortTable() override;

private:

    void insertCopy(int ligne, CopyInfo& copy);
    void insertPage(int& line, PageInfo& page, CopyInfo& copy);
    void insertField(int line, FieldInfo& field, PageInfo& page, CopyInfo& copy);

};

#endif // GROUPVIEWTABLE_H
