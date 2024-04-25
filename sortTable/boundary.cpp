#include "boundary.h"
using namespace std;
//Boundary
Boundary::Boundary(QWidget *parent) : QMainWindow(parent){
    menuWindow = new QWidget(this);
    tableWindow = new QWidget(this);
    sortTable = new SortTable(tableWindow);
    resize(1080,720);
    show();
    init();
}

Boundary::~Boundary() {}

void Boundary::setController(Controller *controller){
    controlWindow = controller;
}

void Boundary::ouvrirExplorateurFichier(){
    controlWindow->afficherGestionnaire();
}

void Boundary::init(){
    setCentralWidget(menuWindow);
    setStyleSheet("background-color : #404040");

    QPushButton *menuButton = new QPushButton("Import files",menuWindow);
    menuButton->setFixedSize(400,100);
    menuButton->setStyleSheet("background-color : green");
    connect(menuButton, &QPushButton::clicked, this, &Boundary::ouvrirExplorateurFichier);

    QHBoxLayout *menuLayout = new QHBoxLayout;
    menuWindow->setLayout(menuLayout);
    menuLayout->addWidget(menuButton);
}

void Boundary::initTableFilter(){

    QCheckBox *nom = new QCheckBox("Name",checkBoxGroup);
    nom->setCheckState(Qt::Checked);
    connect(nom,&QCheckBox::stateChanged,sortTable,&SortTable::editNameColumn);

    QCheckBox *syntax = new QCheckBox("Syntax",checkBoxGroup);
    syntax->setCheckState(Qt::Checked);
    connect(syntax,&QCheckBox::stateChanged,sortTable,&SortTable::editSyntaxColumn);

    QCheckBox *semantic = new QCheckBox("Semantic",checkBoxGroup);
    semantic->setCheckState(Qt::Checked);
    connect(semantic,&QCheckBox::stateChanged,sortTable,&SortTable::editSemanticColumn);

    QCheckBox *metric1 = new QCheckBox("Metric 1",checkBoxGroup);
    connect(metric1,&QCheckBox::stateChanged,sortTable,&SortTable::editMetric1Column);

    QCheckBox *metric2 = new QCheckBox("Metric 2",checkBoxGroup);
    connect(metric2,&QCheckBox::stateChanged,sortTable,&SortTable::editMetric2Column);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addWidget(nom);
    groupBoxLayout->addWidget(syntax);
    groupBoxLayout->addWidget(semantic);
    groupBoxLayout->addWidget(metric1);
    groupBoxLayout->addWidget(metric2);

    checkBoxGroup = new QGroupBox(tableWindow);
    checkBoxGroup->setFixedSize(350,350);
    checkBoxGroup->setLayout(groupBoxLayout);
    checkBoxGroup->hide();
}

void Boundary::displayTableFilter(){
    if (!groupBoxDisplayed){
        checkBoxGroup->show();
        groupBoxDisplayed = true;
    }
    else{
        checkBoxGroup->hide();
        groupBoxDisplayed = false;
    }
}

void Boundary::initTableView(QStringList const& listeFichiers){
    setCentralWidget(tableWindow);
    initTableFilter();

    QPushButton *checkBoxButton = new QPushButton("Sort",tableWindow);
    checkBoxButton->setFixedSize(50,50);
    checkBoxButton->setStyleSheet("background-color :#E1912F");
    connect(checkBoxButton,&QPushButton::clicked,this,&Boundary::displayTableFilter);

    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addStretch();
    checkBoxLayout->addWidget(checkBoxButton);

    tableLayout = new QStackedLayout;
    tableLayout->addWidget(sortTable);
    tableLayout->addWidget(checkBoxGroup);

    QWidget *tableAndFilterWidget = new QWidget;
    tableAndFilterWidget->setLayout(tableLayout);

    QVBoxLayout *evalLayout = new QVBoxLayout;
    tableWindow->setLayout(evalLayout);
    evalLayout->addLayout(checkBoxLayout);
    evalLayout->addWidget(tableAndFilterWidget);

    sortTable->initSortTable(listeFichiers);
}
