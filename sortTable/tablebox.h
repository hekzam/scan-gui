#ifndef TABLEBOX_H
#define TABLEBOX_H

#include <QGroupBox>
#include <QStackedLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>
#include <QDockWidget>
#include <QLineEdit>
#include <QLabel>
#include <QRegularExpression>
#include <QDebug>
#include <QStackedWidget>
#include <QScrollBar>
#include "sorttable.h"
#include "../json/jsonlinker.h"
#include "fieldviewtable.h"
#include "groupviewtable.h"
#include <vector>
#include <algorithm>

/* Layout of the table module
*
* QVBoxLayout :   * QHBoxLayout contenant la barre de recherche (QLineEdit),
*                   zone d'affichage de texte(QLabel) et le bouton de tri (QButton) :
*                   - QLineEdit récupère le texte de l'utilisateur afin d'éxcuter la recherche dans le
*                   tableau concerné et filtre en conséquence les lignes.
*                   - QLabel affiche les résultats de la fuzzySearch ou encore des messages d'erreurs sur
*                   le format de la recherche.
*                   - QButton fait apparaître un QDockWidget qui contient un QGroupBox avec des QCheckBox
*                     alignées verticalement avec un QVBoxLayoutqui représentent les colonnes à faire
*                     apparaître ou disparaître des tableaux
*
*                 * QStackedWidget contenant deux tableaux (QTableWidget) représentant deux vue différentes et une
*                   checkbox pour switch entre les deux (QCheckBox fieldViewToggle)
*                   La deuxième QCheckBox permet de passer dans le mode recherche atomique
*
*                 * Un pattern de recherche en QString (combinedPattern) et un regex (cf. Doc Regex)
*
*/
class TableBox : public QGroupBox
{
  Q_OBJECT
public:
  TableBox(std::map<QString, ExamInfo> &exams, QWidget *dockParent,
           QWidget *parent = nullptr);

  SortTable *getFieldTable();

private:
  QStackedWidget *tableWidget;
  QCheckBox *fieldViewToggle;
  QCheckBox *atomicSearchToggle;
  SortTable *groupTable;
  SortTable *fieldTable;
  QList<SortTable *> sortTableList;
  SortTable *actualTable;
  QDockWidget *sortDock;
  QGroupBox *sortBox;
  QPushButton *sortButton;

  // search zone
  QLineEdit *textZone;
  QLabel *searchInfo;

  // patterns and regex
  QString simpleOrMultipleTextPattern =
      "^\\s*(?:[\\w.-]+(?:\\s*,\\s*[\\w.-]+)*)\\s*$";
  QString tagPattern =
      "^\\s*(?:[\\w.-]+\\s*:\\s*[\\w.-]+(?:\\s*,\\s*[\\w.-]+)*)\\s*(?:;\\s*["
      "\\w.-]+\\s*:\\s*[\\w.-]+(?:\\s*,\\s*[\\w.-]+)*)*\\s*$";

  QString combinedPattern = simpleOrMultipleTextPattern + "|" + tagPattern;
  QRegularExpression regexTestPattern;

  // user input
  QString input;

  // selected columns for search
  QList<int> selectedColumns;

  int typeOfSearch = 0 ;

  bool firstAppearence;

  // to delete ??
  bool emptySearchRes;
  QStringList meantSearchesList;

  int actualView = 1;

  void initTableFilter();
  void initTableView();
  void initRegEx();

  void connectFieldViewToggle();
  void connectAtomicSearchToggle();
  bool searchMethod(const int typeOfSearch, QRegularExpression &regex, QString &cellText);
  int containAndIndexOf(const QString &str, const QStringList &searchedTag);

signals:
  void sendDataToPreview(const QStringList &imagePaths,
                         mJSON::dataCopieJSON *data,
                         const int pageNumberToDisplay,
                         const QString &fieldName);

private slots:
  void displayTableFilter();

  // process the user search
  void searchProcessing();

  // clean the sort table by printing all the rows and delete the warning
  // message if necessary
  void cleanSortTable();

  // process the different type of search
  // the simple search
  void simpleTextProcessing(QString &querylocale);
  // the multiple text search
  void multipleTextProcessing(QString &querylocale);
  // the tag search
  void tagsProcessing(QString &querylocale);

  //filter the rows
  void filterTextRows(QRegularExpression regex);
  void filterTaggedTextRows(QList <QRegularExpression> regexList);

  // initialize the needed columns for the search
  void initSelectedColumns(bool isTagSearch);

  // Fuzzy search
  int levenshteinDistance(QString str1, QString str2);
  QStringList fuzzySearch(QStringList meantSearchesList, QString cellText,
                          QRegularExpression regex, int threshold);
  void printFuzzySearchRes(QStringList meantSearchesList);

  void synchronizeColumnWidth(int colIndex, int oldSize, int newSize);

  void collectData(int row, int col);
};

#endif // TABLEBOX_H
