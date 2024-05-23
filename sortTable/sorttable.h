#ifndef SORTTABLE_H
#define SORTTABLE_H

#include <QTableWidget>
#include <QList>
#include <QFile>
#include <QHeaderView>
#include "progresscell.h"
#include "../json/jsonlinker.h"

class SortTable : public QTableWidget
{
    Q_OBJECT
public:
  enum Columns
  {
    COL_SUBJECT = 0,
    COL_COPY,
    COL_PAGE,
    COL_FIELD,
    COL_SYNTAX,
    COL_SEMANTIC,
    COL_MET1,
    COL_MET2,
    NB_COL
  };

  SortTable(std::map<QString, SubjectInfo> &copies, QWidget *parent = nullptr);
  virtual void initSortTable() = 0;

private:
    QStringList headerList;

    virtual void insertSubject(int& ligne, SubjectInfo& subject) = 0;
    virtual void insertCopy(int& ligne, SubjectInfo& subject, CopyInfo& copy) = 0;
    virtual void insertPage(int& line, SubjectInfo& subject, CopyInfo& copy, PageInfo& page) = 0;
    virtual void insertField(int& line, SubjectInfo& subject, CopyInfo& copy, PageInfo& page, FieldInfo& field) = 0;

protected:
    std::map<QString, SubjectInfo>& subjectMap;

public slots:
    void editColumn(int checkState, int column);
    QStringList getHeaderList();
};

#endif // SORTTABLE_H
