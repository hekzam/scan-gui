#include "scaninfo.h"

/*      Field Info      */

FieldInfo::FieldInfo(){};

FieldInfo::FieldInfo(QString const &fieldName) : m_fieldName(fieldName){};

QString const &FieldInfo::getFieldName() const
{
  return m_fieldName;
}

QString FieldInfo::getChecked() const
{
  return m_checked;
}

QString FieldInfo::getValue() const
{
  return m_value;
}

int FieldInfo::getSyntax() const
{
  return m_syntax;
}

void FieldInfo::setChecked(QString const &checked)
{
  m_checked = checked;
}

void FieldInfo::setValue(QString const &value)
{
  m_value = value;
}

void FieldInfo::setSyntax(int syntax)
{
  m_syntax = syntax;
}

/*      Page Info      */

PageInfo::PageInfo(){};

PageInfo::PageInfo(QString const &pageName, int pageNum, int pageInJSON)
    : m_pageName(pageName), m_pageNum(pageNum), m_pageInJSON(pageInJSON), m_fieldNum(0),
      m_pageInFiles(0){};

void PageInfo::addField(QString const &fieldName)
{
  m_pageFieldMap[fieldName] = FieldInfo(fieldName);
  m_fieldNum++;
}

void PageInfo::setPath(QString const &path)
{
  m_filePath = path;
}

void PageInfo::setPageInFiles(int pageInFiles)
{
  m_pageInFiles = pageInFiles;
}

bool PageInfo::containsField(QString const &fieldName) const
{
  return m_pageFieldMap.find(fieldName) != m_pageFieldMap.end();
}

QString const& PageInfo::getPageName() const
{
  return m_pageName;
}

QString const& PageInfo::getFilePath() const
{
  return m_filePath;
}

int const PageInfo::getPageNum() const{
  return m_pageNum;
}

int PageInfo::pageIsInJSON() const
{
  return m_pageInJSON;
}

int PageInfo::pageIsInFiles() const
{
  return m_pageInFiles;
}

int PageInfo::getFieldNum() const
{
  return m_fieldNum;
}

FieldInfo &PageInfo::getField(QString const &fieldName)
{
  static FieldInfo defaultFieldInfo("");
  if (!containsField(fieldName))
    return defaultFieldInfo;
  return m_pageFieldMap[fieldName];
}

void PageInfo::setPageInJSON(int pageInJSON)
{
  m_pageInJSON = pageInJSON;
}

std::unordered_map<QString, FieldInfo>::iterator PageInfo::begin()
{
  return m_pageFieldMap.begin();
}

std::unordered_map<QString, FieldInfo>::iterator PageInfo::end()
{
  return m_pageFieldMap.end();
}

/*      Copy Info      */

CopyInfo::CopyInfo(){};

CopyInfo::CopyInfo(QString const &copyName, int copyNum, int copyInJSON)
    : m_copyName(copyName), m_copyNum(copyNum), m_copyInJSON(copyInJSON), m_numPages(0),
      m_copyInFiles(0){};

void CopyInfo::addPage(QString const &pageName, int pageNum, int pageInJSON)
{
  if (!containsPage(pageName))
  {
    m_copyPageMap[pageName] = PageInfo(pageName, pageNum, pageInJSON);
    m_numPages++;
  }
}

void CopyInfo::addFieldToPage(QString const &pageName, QString const &fieldName)
{
  if (containsPage(pageName))
    m_copyPageMap[pageName].addField(fieldName);
}

void CopyInfo::setPagePath(QString const &pageName, QString const &path)
{
  if (containsPage(pageName))
    m_copyPageMap[pageName].setPath(path);
}

void CopyInfo::setCopyInFiles(int copyInFiles)
{
  m_copyInFiles = copyInFiles;
}

void CopyInfo::setCopyPageInFiles(QString const &pageName, int pageInFiles)
{
  if (containsPage(pageName))
    m_copyPageMap[pageName].setPageInFiles(pageInFiles);
}

bool CopyInfo::containsPage(QString const &pageName)
{
  return m_copyPageMap.find(pageName) != m_copyPageMap.end();
}

QString const& CopyInfo::getCopyName() const
{
  return m_copyName;
}

int const CopyInfo::getCopyNum() const{
  return m_copyNum;
}

int CopyInfo::copyIsInJSON() const
{
  return m_copyInJSON;
}

int CopyInfo::getNumPages() const
{
  return m_numPages;
}

int CopyInfo::copyIsInFiles() const
{
  return m_copyInFiles;
}

QStringList CopyInfo::getPagesPathList()
{
  QStringList paths;
  for (auto itPage = m_copyPageMap.begin(); itPage != m_copyPageMap.end();
       itPage++)
    paths.append(itPage->second.getFilePath());
  return paths;
}

PageInfo &CopyInfo::getPage(QString const &pageName)
{
  static PageInfo defaultPageInfo("", 0, 0);
  if (!containsPage(pageName))
    return defaultPageInfo;
  return m_copyPageMap[pageName];
}

void CopyInfo::setCopyInJSON(int copyInJSON)
{
  m_copyInJSON = copyInJSON;
}

std::map<QString, PageInfo>::iterator CopyInfo::begin()
{
  return m_copyPageMap.begin();
}

std::map<QString, PageInfo>::iterator CopyInfo::end()
{
  return m_copyPageMap.end();
}

/*      Subject Info      */

SubjectInfo::SubjectInfo(){};

SubjectInfo::SubjectInfo(QString const &subjectName, mJSON::dataCopieJSON *data)
    : m_subjectName(subjectName), m_data(data), m_numCopies(0){};

QString const &SubjectInfo::getSubjectName() const
{
  return m_subjectName;
}

mJSON::dataCopieJSON *SubjectInfo::getData() const
{
  return m_data;
}

int SubjectInfo::getNumCopies() const
{
  return m_numCopies;
}

void SubjectInfo::addCopy(QString const &copyName, int copyNum, int copyInJSON)
{
  if (!containsCopy(copyName))
  {
    m_subjectCopyMap[copyName] = CopyInfo(copyName, copyNum, copyInJSON);
    m_numCopies++;
  }
}

void SubjectInfo::addPageToCopy(QString const &copyName,
                                QString const &pageName, int pageNum, int pageInJSON)
{
  if (containsCopy(copyName))
    m_subjectCopyMap[copyName].addPage(pageName, pageNum, pageInJSON);
}

void SubjectInfo::addFieldToCopyPage(QString const &copyName,
                                     QString const &pageName,
                                     QString const &fieldName)
{
  if (containsCopy(copyName))
    m_subjectCopyMap[copyName].addFieldToPage(pageName, fieldName);
}

CopyInfo &SubjectInfo::getCopy(QString const &copyName)
{
  static CopyInfo defaultCopy("", 0, 0);
  if (!containsCopy(copyName))
    return defaultCopy;
  return m_subjectCopyMap[copyName];
}

QStringList SubjectInfo::getCopiesPathList()
{
  QStringList paths;
  for (auto itCopy = m_subjectCopyMap.begin(); itCopy != m_subjectCopyMap.end();
       itCopy++)
  {
    paths.append(itCopy->second.getPagesPathList());
  }
  return paths;
}

void SubjectInfo::setPagePath(QString const &copyName, QString const &pageName,
                              QString const &path)
{
  if (containsCopy(copyName))
    m_subjectCopyMap[copyName].setPagePath(pageName, path);
}

void SubjectInfo::setCopyInFiles(QString const &copyName, int copyInFiles)
{
  if (containsCopy(copyName))
    m_subjectCopyMap[copyName].setCopyInFiles(copyInFiles);
}

void SubjectInfo::setCopyPageInFiles(QString const &copyName,
                                     QString const &pageName, int copyInFiles)
{
  if (containsCopy(copyName))
    m_subjectCopyMap[copyName].setCopyPageInFiles(pageName, copyInFiles);
}

bool SubjectInfo::containsCopy(QString const &copyName)
{
  return m_subjectCopyMap.find(copyName) != m_subjectCopyMap.end();
}

bool SubjectInfo::copyContainsPage(QString const &copyName,
                                   QString const &pageName)
{
  if (containsCopy(copyName))
    return m_subjectCopyMap[copyName].containsPage(pageName);
  return false;
}

std::map<QString, CopyInfo>::iterator SubjectInfo::begin()
{
  return m_subjectCopyMap.begin();
}

std::map<QString, CopyInfo>::iterator SubjectInfo::end()
{
  return m_subjectCopyMap.end();
}
