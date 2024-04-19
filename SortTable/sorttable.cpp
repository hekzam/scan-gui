#include "sorttable.h"

SortTable::SortTable(QWidget *parent) : QTableWidget(parent) {
    setColumnCount(NB_COL);
    headerList << "Nom" << "Syntaxe" << "Sémantique" << "Métrique 1" << "Métrique 2";
    hideColumn(COL_MET1);
    hideColumn(COL_MET2);
    setHorizontalHeaderLabels(headerList);
    setStyleSheet("background-color : #2E2F30;"
                  "selection-background-color :#404244;");
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setShowGrid(true);
    setGridStyle(Qt::DotLine);
    setSortingEnabled(true);
}

SortTable::~SortTable(){
    for (QFile *fichier : fileList)
        delete fichier;
}

void SortTable::initSortTable(QStringList const& fileNames){
    std::srand(std::time(nullptr));
    int ligne(0);
    setRowCount(fileNames.size());
    for(QString const& nomFichier : fileNames){
        QFile *file = new QFile(nomFichier);
        fileList.push_back(file);
        int progress = std::rand() % 101;
        int semantique = std::rand() % 2;
        ProgressCell *progression = new ProgressCell(progress,this);
        setItem(ligne,COL_NOM,new QTableWidgetItem(file->fileName().section('/', -1)));
        setCellWidget(ligne,COL_SYNTAXE, progression);
        setItem(ligne,COL_SYNTAXE, progression);
        setItem(ligne,COL_SEMANTIQUE,new QTableWidgetItem(QString::number(semantique)));
        ligne++;
    }
    resizeColumnsToContents();
}

void SortTable::editNameColumn(int checkedState){
    if (checkedState == Qt::Checked)
        showColumn(COL_NOM);
    else
        hideColumn(COL_NOM);
}

void SortTable::editSyntaxColumn(int checkedState){
    if (checkedState == Qt::Checked)
        showColumn(COL_SYNTAXE);
    else
        hideColumn(COL_SYNTAXE);
}

void SortTable::editSemanticColumn(int checkedState){
    if (checkedState == Qt::Checked)
        showColumn(COL_SEMANTIQUE);
    else
        hideColumn(COL_SEMANTIQUE);
}

void SortTable::editMetric1Column(int checkedState){
    if (checkedState == Qt::Checked)
        showColumn(COL_MET1);
    else
        hideColumn(COL_MET1);
}

void SortTable::editMetric2Column(int checkedState){
    if (checkedState == Qt::Checked)
        showColumn(COL_MET2);
    else
        hideColumn(COL_MET2);
}
