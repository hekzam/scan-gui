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
  FieldInfo(){};

  FieldInfo(QString const &fieldName) : m_fieldName(fieldName){};

  QString const &getFieldName() const
  {
    return m_fieldName;
  }

  QString getChecked() const
  {
    return m_checked;
  }

  QString getValue() const
  {
    return m_value;
  }

  int getSyntax() const
  {
    return m_syntax;
  }

  void setChecked(QString const &checked)
  {
    m_checked = checked;
  }

  void setValue(QString const &value)
  {
    m_value = value;
  }

  void setSyntax(int syntax)
  {
    m_syntax = syntax;
  }
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

  void addField(QString const &fieldName)
  {
    m_pageFieldMap[fieldName] = FieldInfo(fieldName);
    m_fieldNum++;
  }

  void setPath(QString const &path)
  {
    m_filePath = path;
  }

  void setPageInFiles(int pageInFiles)
  {
    m_pageInFiles = pageInFiles;
  }

  bool containsField(QString const &fieldName) const
  {
    return m_pageFieldMap.find(fieldName) != m_pageFieldMap.end();
  }

public:
  PageInfo(){};

  PageInfo(QString const &pageName, int pageInJSON)
      : m_pageName(pageName), m_pageInJSON(pageInJSON), m_fieldNum(0),
        m_pageInFiles(0){};

  QString const &getPageName() const
  {
    return m_pageName;
  }

  QString const &getFilePath() const
  {
    return m_filePath;
  }

  int getPageInJSON() const
  {
    return m_pageInJSON;
  }

  int getPageInFiles() const
  {
    return m_pageInFiles;
  }

  int getFieldNum() const
  {
    return m_fieldNum;
  }

  FieldInfo &getField(QString const &fieldName)
  {
    static FieldInfo defaultFieldInfo("");
    if (!containsField(fieldName))
      return defaultFieldInfo;
    return m_pageFieldMap[fieldName];
  }

  void setPageInJSON(int pageInJSON)
  {
    m_pageInJSON = pageInJSON;
  }

  auto begin()
  {
    return m_pageFieldMap.begin();
  }

  auto end()
  {
    return m_pageFieldMap.end();
  }
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

  void addPage(QString const &pageName, int pageInJSON)
  {
    if (!containsPage(pageName))
    {
      m_copyPageMap[pageName] = PageInfo(pageName, pageInJSON);
      m_numPages++;
    }
  }

  void addFieldToPage(QString const &pageName, QString const &fieldName)
  {
    if (containsPage(pageName))
      m_copyPageMap[pageName].addField(fieldName);
  }

  void setPagePath(QString const &pageName, QString const &path)
  {
    if (containsPage(pageName))
      m_copyPageMap[pageName].setPath(path);
  }

  void setCopyInFiles(int copyInFiles)
  {
    m_copyInFiles = copyInFiles;
  }

  void setCopyPageInFiles(QString const &pageName, int pageInFiles)
  {
    if (containsPage(pageName))
      m_copyPageMap[pageName].setPageInFiles(pageInFiles);
  }

  bool containsPage(QString const &pageName)
  {
    return m_copyPageMap.find(pageName) != m_copyPageMap.end();
  }

public:
  CopyInfo(){};

  CopyInfo(QString const &copyName, int copyInJSON)
      : m_copyName(copyName), m_copyInJSON(copyInJSON), m_numPages(0),
        m_copyInFiles(0){};

  QString const &getCopyName() const
  {
    return m_copyName;
  }

  int getCopyInJSON() const
  {
    return m_copyInJSON;
  }

  int getNumPages() const
  {
    return m_numPages;
  }

  int getCopyInFiles() const
  {
    return m_copyInFiles;
  }

  QStringList getPagesPathList()
  {
    QStringList paths;
    for (auto itPage = m_copyPageMap.begin(); itPage != m_copyPageMap.end();
         itPage++)
      paths.append(itPage->second.getFilePath());
    return paths;
  }

  PageInfo &getPage(QString const &pageName)
  {
    static PageInfo defaultPageInfo("", 0);
    if (!containsPage(pageName))
      return defaultPageInfo;
    return m_copyPageMap[pageName];
  }

  void setCopyInJSON(int copyInJSON)
  {
    m_copyInJSON = copyInJSON;
  }

  auto begin()
  {
    return m_copyPageMap.begin();
  }

  auto end()
  {
    return m_copyPageMap.end();
  }
};

class SubjectInfo
{
private:
  QString m_subjectName;
  mJSON::dataCopieJSON *m_data = nullptr;
  std::map<QString, CopyInfo> m_subjectCopyMap;
  int m_numCopies;

public:
  SubjectInfo(){};

  SubjectInfo(QString const &subjectName, mJSON::dataCopieJSON *data = nullptr)
      : m_subjectName(subjectName), m_data(data), m_numCopies(0){};

  QString const &getSubjectName() const
  {
    return m_subjectName;
  }

  mJSON::dataCopieJSON *getData() const
  {
    return m_data;
  }

  int getNumCopies() const
  {
    return m_numCopies;
  }

  void addCopy(QString const &copyName, int copyInJSON)
  {
    if (!containsCopy(copyName))
    {
      m_subjectCopyMap[copyName] = CopyInfo(copyName, copyInJSON);
      m_numCopies++;
    }
  }

  void addPageToCopy(QString const &copyName, QString const &pageName,
                     int pageInJSON)
  {
    if (containsCopy(copyName))
      m_subjectCopyMap[copyName].addPage(pageName, pageInJSON);
  }

  void addFieldToCopyPage(QString const &copyName, QString const &pageName,
                          QString const &fieldName)
  {
    if (containsCopy(copyName))
      m_subjectCopyMap[copyName].addFieldToPage(pageName, fieldName);
  }

  CopyInfo &getCopy(QString const &copyName)
  {
    static CopyInfo defaultCopy("", 0);
    if (!containsCopy(copyName))
      return defaultCopy;
    return m_subjectCopyMap[copyName];
  }

  QStringList getCopiesPathList()
  {
    QStringList paths;
    for (auto itCopy = m_subjectCopyMap.begin();
         itCopy != m_subjectCopyMap.end(); itCopy++)
    {
      paths.append(itCopy->second.getPagesPathList());
    }
    return paths;
  }

  void setPagePath(QString const &copyName, QString const &pageName,
                   QString const &path)
  {
    if (containsCopy(copyName))
      m_subjectCopyMap[copyName].setPagePath(pageName, path);
  }

  void setCopyInFiles(QString const &copyName, int copyInFiles)
  {
    if (containsCopy(copyName))
      m_subjectCopyMap[copyName].setCopyInFiles(copyInFiles);
  }

  void setCopyPageInFiles(QString const &copyName, QString const &pageName,
                          int copyInFiles)
  {
    if (containsCopy(copyName))
      m_subjectCopyMap[copyName].setCopyPageInFiles(pageName, copyInFiles);
  }

  bool containsCopy(QString const &copyName)
  {
    return m_subjectCopyMap.find(copyName) != m_subjectCopyMap.end();
  }

  bool copyContainsPage(QString const &copyName, QString const &pageName)
  {
    if (containsCopy(copyName))
      return m_subjectCopyMap[copyName].containsPage(pageName);
    return false;
  }

  auto begin()
  {
    return m_subjectCopyMap.begin();
  }

  auto end()
  {
    return m_subjectCopyMap.end();
  }
};

Q_DECLARE_METATYPE(SubjectInfo *)
Q_DECLARE_METATYPE(CopyInfo *)
Q_DECLARE_METATYPE(PageInfo *)
Q_DECLARE_METATYPE(FieldInfo *)
