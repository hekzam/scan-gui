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
    TableBox(QList<JsonLinker::fieldInfo> const& fields, QWidget *dockParent, QMap<QString,dataCopieJSON*> const& fileDataMap, QWidget *parent = nullptr);

private:
    SortTable *sortTable;
    QDockWidget *sortDock;
    QGroupBox *sortBox;
    QPushButton *sortButton;

    QLineEdit *textZone;

    QLabel *searchInfo;

    QString simpleOrMultipleTextPattern = "^\\s*(?:[\\w.-]+(?:\\s*,\\s*[\\w.-]+)*)\\s*$";
    QString tagPattern = "^\\s*(?:[\\w.-]+\\s*:\\s*[\\w.-]+(?:\\s*,\\s*[\\w.-]+)*)\\s*(?:;\\s*[\\w.-]+\\s*:\\s*[\\w.-]+(?:\\s*,\\s*[\\w.-]+)*)*\\s*$";

    QString combinedPattern = simpleOrMultipleTextPattern + "|" + tagPattern;

    QRegularExpression regexTestPattern;

    QString input;
    QString text;


    QStringList queriesList;

    QList<int> selectedColumns;

    QMap<QString,dataCopieJSON *> const& m_fileDataMap;

    bool firstAppearence;

    bool emptySearchRes;
    QList<QString> meantSearchesList;

    void initTableFilter();
    void initTableView(QList<JsonLinker::fieldInfo> const& fields);
    void initRegEx();

    void sendNewFilePaths(int row, int col);

  signals:
    void sendDataToPreview(QString const&, dataCopieJSON const&, const int);

  private slots:
    void displayTableFilter();
    void searchProcessing();

    void tagsProcessing(QString query);
    void multipleTextProcessing(QString query);
    void simpleTextProcessing(QString query);

    void filterTextRows(QRegularExpression regex, QList<int> selectedColumns);
    void filterTaggedTextRows(QList <QRegularExpression> regexList, QList<int> selectedColumns);
    void cleanSortTable();

    void initSelectedColumns(bool isTagSearch);
    int levenshteinDistance(QString str1, QString str2);
};

#endif // TABLEBOX_H
