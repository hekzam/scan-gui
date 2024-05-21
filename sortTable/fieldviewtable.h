#ifndef FIELDVIEWTABLE_H
#define FIELDVIEWTABLE_H

#include "sorttable.h"

class FieldViewTable : public SortTable
{
public:

    explicit FieldViewTable(std::map<QString, CopyInfo>& copies, QWidget *parent = nullptr);
    void initSortTable() override;

private:

    void insertCopy(int& ligne, CopyInfo& copy);
    void insertPage(int& line, PageInfo const& page, CopyInfo& copy);
    void insertField(int& line, FieldInfo const& field, PageInfo const& page, CopyInfo& copy);

};

#endif // FieldViewTable_H
