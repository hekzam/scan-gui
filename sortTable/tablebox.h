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
    TableBox(std::map<QString, CopyInfo>& copies, QWidget *dockParent, QWidget *parent = nullptr);

private:
    QStackedWidget *tableWidget;
    SortTable *groupTable;
    SortTable *fieldTable;
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

    void initTableFilter();
    void initTableView();
    void initRegEx();

    void collectDataGroup(int row, int col);
    void collectDataField(int row, int col);


  signals:
    void sendDataToPreview(QString const&);

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


};

#endif // TABLEBOX_H
