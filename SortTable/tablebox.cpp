/*TO-DO
            * faire le tri via le headerTitle (fonction qui exsiste déjà ? -> to check)
               => FAIT

            * faire la recherche dans le champs
                    -> faire à la façon d'un cntrl+f -> et si c'était une balise ? (#copie: 1, #exercice : 2, ...
                                                             -> permet de réduire le temps de calcul non ?)
                         - récupérer la chaine
                         - faire un highlight de la chaine recup dans les autres mots -> peut être faire un stackWidget
                                                        (on change de widget dès que le champs est focus/cliqué)
                                                        avec un tableau vide qui se remplit au fur et/ou se vide au fur et à mesure
                                                        que l'user écrit.
                         - actualisé en temps réel l'affichage

                    -> ajouter plus tard les recherches avancées (avec des balises à la façon de Gmail ex:  #student: Emilien, #id: 221... )

            * lorsque qu'on clique sur un elem, une pop-up s'ouvre (pour donner une idée de la preview)
            *



*/
#include "tablebox.h"


TableBox::TableBox(QStringList const& fileNames,QWidget *parent) : QGroupBox(parent), firstAppearence(true) {
    setTitle("Tableau d'évalutation");
    sortBox = new QGroupBox(this);

    textZone = new QLineEdit(this);
    textZone->set
    textZone->setPlaceholderText("Rentrez votre recherche et cliquez sur entrée. Pour effectuer une recherche par balise, consultez l'aide.");
    connect(textZone, &QLineEdit::textChanged, this, &TableBox::cleanSearchBar);
    connect(textZone, &QLineEdit::returnPressed, this, &TableBox::searchProcessing);

    sortButton = new QPushButton("Tri",this);
    sortTable = new SortTable(this);
    sortDock = new QDockWidget(parent);
    sortDock->hide();
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
}

void TableBox::displayTableFilter(){
    if (!sortDock->isHidden()){
        sortDock->hide();
    }
    else{
        if (firstAppearence){
            QPoint pos = sortTable->mapToGlobal(sortTable->rect().topRight());
            pos.setX(pos.x() - sortDock->width());
            sortDock->move(pos);
            firstAppearence = false;
        }
        sortDock->show();
    }
}

void TableBox::initTableView(QStringList const& fileNames){

    QHBoxLayout *sortButtonLayout = new QHBoxLayout;
    sortButtonLayout->addStretch();
    sortButtonLayout->addWidget(textZone);
    sortButtonLayout->addWidget(sortButton);

    QVBoxLayout *evalLayout = new QVBoxLayout;
    evalLayout->addLayout(sortButtonLayout);
    evalLayout->addWidget(sortTable);

    sortTable->initSortTable(fileNames);

    setLayout(evalLayout);
}


void TableBox::searchProcessing(){

    bool istextSearchedEmpty = textSearched.trimmed().isEmpty();

    if(textSearched.contains(":")){
        //qDebug()<< "Balise détecté !!";
        tagsProcessing();
    }

    for (int i = 0; i < sortTable->rowCount(); ++i) {
        if (istextSearchedEmpty) {
            sortTable->setRowHidden(i, false);
            continue;
        }

        bool match = false;
        for (int j = 0; j < sortTable->columnCount(); ++j) {
            QTableWidgetItem *item = sortTable->item(i, j);
            if (item && item->text().contains(textSearched, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        sortTable->setRowHidden(i, !match);
    }
}

void TableBox::tagsProcessing()
{

    QStringList tagAndWord = textSearched.trimmed().split(":");
    QString tag;
    QString word;

    tag = tagAndWord[0];

    word = tagAndWord[1];
    //qDebug()<< tag <<word;

}

void TableBox::cleanSearchBar()
{
    textSearched = textZone->text();
    if (textSearched.trimmed() == "") {
        searchProcessing();
    }
}


