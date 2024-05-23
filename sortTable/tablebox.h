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
#include <iostream>


class TableBox : public QGroupBox
{
    Q_OBJECT
public:
    TableBox(std::map<QString, SubjectInfo>& copies, QWidget *dockParent, QWidget *parent = nullptr);
    ~TableBox();

    QList<SortTable *> *getSortTableList() const;

    SortTable *getFieldTable();

  private:
    QStackedWidget *tableWidget;
    QCheckBox *fieldViewToggle;
    SortTable *groupTable;
    SortTable *fieldTable;
    QList<SortTable *> sortTableList;
    SortTable *actualTable;
    QDockWidget *sortDock;
    QGroupBox *sortBox;
    QPushButton *sortButton;

    //search zone
    QLineEdit *textZone;
    QLabel *searchInfo;

    //patterns and regex
    QString simpleOrMultipleTextPattern = "^\\s*(?:[\\w.-]+(?:\\s*,\\s*[\\w.-]+)*)\\s*$";
    QString tagPattern = "^\\s*(?:[\\w.-]+\\s*:\\s*[\\w.-]+(?:\\s*,\\s*[\\w.-]+)*)\\s*(?:;\\s*[\\w.-]+\\s*:\\s*[\\w.-]+(?:\\s*,\\s*[\\w.-]+)*)*\\s*$";

    QString combinedPattern = simpleOrMultipleTextPattern + "|" + tagPattern;
    QRegularExpression regexTestPattern;

    //user input
    QString input;

    // selected columns for search
    QList<int> selectedColumns;

    bool firstAppearence;

    // to delete ??
    bool emptySearchRes;
    QStringList meantSearchesList;

    int actualView = 1;

    void initTableFilter();
    void initTableView();
    void initRegEx();

    void connectFieldViewToggle();

  signals:
    void sendDataToPreview(const QStringList &imagePaths,
                           mJSON::dataCopieJSON *data,
                           const int pageNumberToDisplay,
                           const QString &fieldName);

  private slots:
    void displayTableFilter();

    // process the user search
    void searchProcessing();

    //clean the sort table by printing all the rows and delete the warning message if necessary
    void cleanSortTable();

    // process the different type of search
    //the simple search
    void simpleTextProcessing(QString& querylocale);
    //the multiple text search
    void multipleTextProcessing(QString& querylocale);
    //the tag search
    void tagsProcessing(QString& querylocale);

    //filter the rows
    void filterRows(QList<QRegularExpression> regexList);
    void filterTextRows(QRegularExpression regex);
    void filterTaggedTextRows(QList <QRegularExpression> regexList);

    //initialize the needed columns for the search
    void initSelectedColumns(bool isTagSearch);

    //Fuzzy search
    int levenshteinDistance(QString str1, QString str2);
    QStringList fuzzySearch(QStringList meantSearchesList, QString cellText, QRegularExpression regex, int threshold);
    void printFuzzySearchRes(QStringList meantSearchesList);

    void collectData(int row, int col);
};

#endif // TABLEBOX_H
