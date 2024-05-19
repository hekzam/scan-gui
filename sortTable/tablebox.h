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
#include "sorttable.h"
#include "../json/jsonlinker.h"
#include <vector>
#include <algorithm>
#include <iostream>


class TableBox : public QGroupBox
{
    Q_OBJECT
public:
    TableBox(QMap<QString, CopyInfo> const& copies, QWidget *dockParent, QWidget *parent = nullptr);

private:
    SortTable *sortTable;
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
    void initTableView(QMap<QString, CopyInfo> const& copies);
    void initRegEx();

    void sendNewFilePaths(int row, int col);

  signals:
    void sendDataToPreview(QString const&, dataCopieJSON const&, const int);

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
