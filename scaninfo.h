#include <QString>
#include <QMap>
#include "json/jsonreader.h"

/* Informations liées a un examen */


/* FieldInfo :
 * Un champ est constitué d'un nom, un booléen grisé ou non (sous forme de QString pour éventuellement
 * l'afficher dans un tableau), une valeur (dans le cas ou le champ n'est pas une case à cocher) et une
 * syntaxe.
*/

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

/* PageInfo :
 * Une page est constituée d'un nom, du chemin du fichier de la page (nécéssaire pour les cas de tests mais
 * pas dans le code final, du numéro de la page, un booléen indiquant si la page est spécifiée dans le JSON,
 * un booléen spécifiant si la page est dans les fichiers séléctionés, une map associant a chaque nom de champ
 * la structure associée et le nombre de champs dans la page.
 */

class PageInfo
{

private:
  QString m_pageName;
  QString m_filePath; // pas dans le code final
  int m_pageNum;
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
  PageInfo(QString const &pageName, int pageNum, int pageInJSON);
  QString const &getPageName() const;
  QString const &getFilePath() const;
  int const getPageNum() const;
  int pageIsInJSON() const;
  int pageIsInFiles() const;
  int getFieldNum() const;
  FieldInfo &getField(QString const &fieldName);
  void setPageInJSON(int pageInJSON);
  std::unordered_map<QString, FieldInfo>::iterator begin();
  std::unordered_map<QString, FieldInfo>::iterator end();
};


/* CopyInfo :
 * Une copie est constituée d'un nom, d'une map associant a chaque nom de page la structure associée,
 * le numéro de la copie un booléen indiquant si la copie est spécifiée dans le JSON,
 * un booléen spécifiant si la copie est dans les fichiers séléctionés et le nombre de pages dans la copie.
 */

class CopyInfo
{
private:
  QString m_copyName;
  std::map<QString, PageInfo> m_copyPageMap;
  int m_copyNum;
  int m_copyInJSON;
  int m_copyInFiles;
  int m_numPages;

  friend class SubjectInfo;

  void addPage(QString const &pageName, int pageNum, int pageInJSON);
  void addFieldToPage(QString const &pageName, QString const &fieldName);
  void setPagePath(QString const &pageName, QString const &path);
  void setCopyInFiles(int copyInFiles);
  void setCopyPageInFiles(QString const &pageName, int pageInFiles);
  bool containsPage(QString const &pageName);

public:
  CopyInfo();
  CopyInfo(QString const &copyName, int copyNum, int copyInJSON);
  QString const &getCopyName() const;
  int const getCopyNum() const;
  int copyIsInJSON() const;
  int getNumPages() const;
  int copyIsInFiles() const;
  QStringList getPagesPathList();
  PageInfo &getPage(QString const &pageName);
  void setCopyInJSON(int copyInJSON);
  std::map<QString, PageInfo>::iterator begin();
  std::map<QString, PageInfo>::iterator end();
};


/* SubjectInfo :
 * Un sujet est constitué d'un nom, d'un pointeur vers les données du JSON, d'une map associant a chaque nom
 * de copie la structure associée et le nombre de copies dans le sujet.
 */

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
  void addCopy(QString const &copyName, int copyNum, int copyInJSON);
  void addPageToCopy(QString const &copyName, QString const &pageName, int pageNum,
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
