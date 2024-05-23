#include <QString>
#include <QMap>
#include "json/jsonreader.h"

class FieldInfo
{

private:
  QString m_fieldName;
  QString m_checked;
  QString m_value;
  int m_syntax;

public:
  FieldInfo();
  FieldInfo(QString const &fieldName);
  QString const &getFieldName() const;
  QString getChecked() const;
  QString getValue() const;
  int getSyntax() const;
  void setChecked(QString const &checked);
  void setValue(QString const &value);
  void setSyntax(int syntax);
};

class PageInfo
{

private:
  QString m_pageName;
  QString m_filePath; // pas dans le code final
  int m_pageInJSON;
  int m_pageInFiles;
  std::unordered_map<QString, FieldInfo> m_pageFieldMap;
  int m_fieldNum;

  friend class CopyInfo;

  void addField(QString const &fieldName);
  void setPath(QString const &path);
  void setPageInFiles(int pageInFiles);
  bool containsField(QString const &fieldName) const;

public:
  PageInfo();
  PageInfo(QString const &pageName, int pageInJSON);
  QString const &getPageName() const;
  QString const &getFilePath() const;
  int getPageInJSON() const;
  int getPageInFiles() const;
  int getFieldNum() const;
  FieldInfo &getField(QString const &fieldName);
  void setPageInJSON(int pageInJSON);
  std::unordered_map<QString, FieldInfo>::iterator begin();
  std::unordered_map<QString, FieldInfo>::iterator end();
};

class CopyInfo
{
private:
  QString m_copyName;
  std::map<QString, PageInfo> m_copyPageMap;
  int m_copyInJSON;
  int m_copyInFiles;
  int m_numPages;

  friend class SubjectInfo;

  void addPage(QString const &pageName, int pageInJSON);
  void addFieldToPage(QString const &pageName, QString const &fieldName);
  void setPagePath(QString const &pageName, QString const &path);
  void setCopyInFiles(int copyInFiles);
  void setCopyPageInFiles(QString const &pageName, int pageInFiles);
  bool containsPage(QString const &pageName);

public:
  CopyInfo();
  CopyInfo(QString const &copyName, int copyInJSON);
  QString const &getCopyName() const;
  int getCopyInJSON() const;
  int getNumPages() const;
  int getCopyInFiles() const;
  QStringList getPagesPathList();
  PageInfo &getPage(QString const &pageName);
  void setCopyInJSON(int copyInJSON);
  std::map<QString, PageInfo>::iterator begin();
  std::map<QString, PageInfo>::iterator end();
};

class SubjectInfo
{
private:
  QString m_subjectName;
  mJSON::dataCopieJSON *m_data = nullptr;
  std::map<QString, CopyInfo> m_subjectCopyMap;
  int m_numCopies;

public:
  SubjectInfo();
  SubjectInfo(QString const &subjectName, mJSON::dataCopieJSON *data = nullptr);
  QString const &getSubjectName() const;
  mJSON::dataCopieJSON *getData() const;
  int getNumCopies() const;
  void addCopy(QString const &copyName, int copyInJSON);
  void addPageToCopy(QString const &copyName, QString const &pageName,
                     int pageInJSON);
  void addFieldToCopyPage(QString const &copyName, QString const &pageName,
                          QString const &fieldName);
  CopyInfo &getCopy(QString const &copyName);
  QStringList getCopiesPathList();
  void setPagePath(QString const &copyName, QString const &pageName,
                   QString const &path);
  void setCopyInFiles(QString const &copyName, int copyInFiles);
  void setCopyPageInFiles(QString const &copyName, QString const &pageName,
                          int copyInFiles);
  bool containsCopy(QString const &copyName);
  bool copyContainsPage(QString const &copyName, QString const &pageName);
  std::map<QString, CopyInfo>::iterator begin();
  std::map<QString, CopyInfo>::iterator end();
};

Q_DECLARE_METATYPE(SubjectInfo *)
Q_DECLARE_METATYPE(CopyInfo *)
Q_DECLARE_METATYPE(PageInfo *)
