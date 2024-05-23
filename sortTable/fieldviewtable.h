#ifndef FIELDVIEWTABLE_H
#define FIELDVIEWTABLE_H

#include "sorttable.h"

class FieldViewTable : public SortTable
{
public:

    explicit FieldViewTable(std::map<QString, SubjectInfo>& copies, QWidget *parent = nullptr);
    void initSortTable() override;

private:

    void insertSubject(int& ligne, SubjectInfo& subject) override;
    void insertCopy(int& ligne, SubjectInfo& subject, CopyInfo& copy) override;
    void insertPage(int& line, SubjectInfo& subject, CopyInfo& copy, PageInfo& page) override;
    void insertField(int& line, SubjectInfo& subject, CopyInfo& copy, PageInfo& page, FieldInfo& field) override;


    // SortTable interface
  protected:
    void addErrors(QString &newErrors) override;
    QStringList getErrors() override;
};

#endif // FieldViewTable_H
