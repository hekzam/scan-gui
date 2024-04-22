/*TO-DO
 *      -> ajouter les recherches avancées (avec des balises à la façon de Gmail
                        ex:  student: Emilien,Marco ; id: 221 )

 * lorsque qu'on clique sur un elem, une pop-up s'ouvre (pour donner une idée de la preview)
*/

#include "tablebox.h"

TableBox::TableBox(QStringList const& fileNames,QWidget *parent) : QGroupBox(parent), firstAppearence(true) {
    setTitle("Tableau d'évalutation");
    sortBox = new QGroupBox(this);

    textZone = new QLineEdit(this);
    textZone->setPlaceholderText("Rentrez votre recherche et cliquez sur entrée. Pour effectuer une recherche par balise, consultez l'aide.");
    connect(textZone, &QLineEdit::textChanged, this, &TableBox::cleanSearchBar);
    connect(textZone, &QLineEdit::returnPressed, this, &TableBox::searchProcessing);

    sortButton = new QPushButton("Tri",this);
    searchInfo = new QLabel(this) ;

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
    sortButtonLayout->addWidget(textZone);

    sortButtonLayout->addWidget(searchInfo);

    sortButtonLayout->addWidget(sortButton);

    QVBoxLayout *evalLayout = new QVBoxLayout;
    evalLayout->addLayout(sortButtonLayout);

    evalLayout->addWidget(sortTable);

    sortTable->initSortTable(fileNames);

    setLayout(evalLayout);
}


void TableBox::searchProcessing(){

    bool istextSearchedEmpty = textSearched.trimmed().isEmpty();

    if((textSearched.contains(":"))||(textSearched.contains(";"))){
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
    //qDebug()<< textSearched;
    //textSearched = textSearched.trimmed(); // attention, trimmed ne supprime que les espaces en trop (pas ceux à l'intérieur de la chaîne)
    //qDebug()<< textSearched;

    QStringList textSearchedList = textSearched.split(";");

    for (int var = 0; var < textSearchedList.size(); ++var) {
        //qDebug()<< textSearchedList[var];
        QStringList tagsAndWordsList = textSearchedList[var].split(":");
        searchedTags.append(tagsAndWordsList[0].trimmed());
        searchedConditions.append(tagsAndWordsList[1].trimmed());
        //qDebug()<< tags;
        //qDebug()<< words;
    }

    for (int var = 0; var < searchedTags.size(); ++var) {
        //si headerList était publique, on aurait pu le réutiliser
        if((knownTags).contains(searchedTags[var])){
            //qDebug()<<"Faire la recherche en lien avec la (ou même les) balise(s)";
        }

    }
}

void TableBox::cleanSearchBar()
{
    textSearched = textZone->text();
    if (textSearched.trimmed() == "") {
        searchProcessing();
    }
}


