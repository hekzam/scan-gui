#include "tablebox.h"

TableBox::TableBox(QStringList const& fileNames,QWidget *parent) : QGroupBox(parent), groupBoxDisplayed(false) {
    setTitle("Tableau d'évalutation");
    sortBox = new QGroupBox(this);
    sortButton = new QPushButton("Tri",this);
    sortTable = new SortTable(this);
    sortDock = new QDockWidget(parent);
    initTableFilter();
    initTableView(fileNames);
}

void TableBox::initTableFilter(){

    sortButton->setFixedSize(50,50);
    sortButton->setStyleSheet("background-color :#E1912F");
    connect(sortButton,&QPushButton::clicked,this,&TableBox::displayTableFilter);

    QCheckBox *nom = new QCheckBox("Nom",sortBox);
    nom->setCheckState(Qt::Checked);
    connect(nom,&QCheckBox::stateChanged,sortTable,&SortTable::editNameColumn);

    QCheckBox *syntax = new QCheckBox("Syntaxe",sortBox);
    syntax->setCheckState(Qt::Checked);
    connect(syntax,&QCheckBox::stateChanged,sortTable,&SortTable::editSyntaxColumn);

    QCheckBox *semantic = new QCheckBox("Semantique",sortBox);
    semantic->setCheckState(Qt::Checked);
    connect(semantic,&QCheckBox::stateChanged,sortTable,&SortTable::editSemanticColumn);

    QCheckBox *metric1 = new QCheckBox("Metric 1",sortBox);
    connect(metric1,&QCheckBox::stateChanged,sortTable,&SortTable::editMetric1Column);

    QCheckBox *metric2 = new QCheckBox("Metric 2",sortBox);
    connect(metric2,&QCheckBox::stateChanged,sortTable,&SortTable::editMetric2Column);

    QVBoxLayout *sortBoxLayout = new QVBoxLayout;
    sortBoxLayout->setSpacing(10);
    sortBoxLayout->addWidget(nom);
    sortBoxLayout->addWidget(syntax);
    sortBoxLayout->addWidget(semantic);
    sortBoxLayout->addWidget(metric1);
    sortBoxLayout->addWidget(metric2);

    sortDock->setFixedSize(350,350);
    sortBox->setLayout(sortBoxLayout);

    sortDock->setWidget(sortBox);
    sortDock->setFloating(true);
    connect(sortDock, &QDockWidget::visibilityChanged, this, &TableBox::updateGroupBoxDisplayed);
    sortDock->hide();
}

void TableBox::displayTableFilter(){
    if (groupBoxDisplayed){
        sortDock->hide();
        groupBoxDisplayed = false;
    }
    else{
        QPoint pos = sortTable->mapToGlobal(sortTable->rect().topRight());
        pos.setX(pos.x() - sortDock->width());
        sortDock->move(pos);
        sortDock->show();
        groupBoxDisplayed = true;
    }
}

void TableBox::updateGroupBoxDisplayed(bool visible){
    groupBoxDisplayed = visible;
}

void TableBox::initTableView(QStringList const& fileNames){

    QHBoxLayout *sortButtonLayout = new QHBoxLayout;
    sortButtonLayout->addStretch();
    sortButtonLayout->addWidget(sortButton);

    QVBoxLayout *evalLayout = new QVBoxLayout;
    evalLayout->addLayout(sortButtonLayout);
    evalLayout->addWidget(sortTable);

    sortTable->initSortTable(fileNames);

    setLayout(evalLayout);
}

