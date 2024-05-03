#include "sorttable.h"

SortTable::SortTable(QWidget *parent) : QTableWidget(parent) {
    setColumnCount(NB_COL);
    headerList << "Paths" << "Name" << "Syntax" << "Semantic" << "Metric 1" << "Metric 2";
    hideColumn(COL_PATH);
    hideColumn(COL_MET1);
    hideColumn(COL_MET2);
    setHorizontalHeaderLabels(headerList);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setShowGrid(true);
    setGridStyle(Qt::DotLine);
    setSortingEnabled(true);
    connect(this,&QTableWidget::cellClicked,this,[this](int row, int col){
        QString fileAndJsonPath(item(row,COL_PATH)->text());
        QStringList paths = fileAndJsonPath.split("$$$");
        qDebug() << paths[0];
        qDebug() << paths[1];
    });
}

SortTable::~SortTable(){
    for (QFile *fichier : fileList)
        delete fichier;
}

void SortTable::initSortTable(std::vector<JsonLinker::infoPage> paths){
    std::srand(std::time(nullptr));
    int ligne(0);
    setRowCount(paths.size());
    for(JsonLinker::infoPage const& path : paths){
        //qDebug() << path.filePath;
        int progress = std::rand() % 101;
        int semantique = std::rand() % 2;
        ProgressCell *progression = new ProgressCell(progress,this);
        setItem(ligne,COL_PATH, new QTableWidgetItem(path.filePath + "$$$" + path.jsonPath));
        setItem(ligne,COL_NAME,new QTableWidgetItem(path.fileName));
        setCellWidget(ligne,COL_SYNTAX, progression);
        setItem(ligne,COL_SYNTAX, progression);
        setItem(ligne,COL_SEMANTIC,new QTableWidgetItem(QString::number(semantique)));
        ligne++;
    }
    resizeColumnsToContents();
}


void SortTable::editColumn(int checkedState, int column){
    if (checkedState == Qt::Checked)
        showColumn(column);
    else
        hideColumn(column);
}


QStringList SortTable::getHeaderList()
{
    return headerList;
}
