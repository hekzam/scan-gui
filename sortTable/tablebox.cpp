/*TO-DO
 *       -> ajouter les recherches avancées (avec des balises à la façon de Gmail
 *       ex:  student: Emilien,Marco ; id: 221 )
*/

#include "tablebox.h"

TableBox::TableBox(QStringList const& fileNames, QWidget *dockParent, QWidget *parent) : QGroupBox(parent), firstAppearence(true) {
    setTitle("Evaluation table");
    sortBox = new QGroupBox(this);

    textZone = new QLineEdit(this);
    textZone->setPlaceholderText("Enter your search and click enter.");
    connect(textZone, &QLineEdit::textChanged, this, &TableBox::cleanSortTable);
    connect(textZone, &QLineEdit::returnPressed, this, &TableBox::searchProcessing);


    sortButton = new QPushButton("Sort",this);
    searchInfo = new QLabel(this) ;

    sortTable = new SortTable(this);
    sortDock = new QDockWidget(dockParent);
    sortDock->hide();


    initRegEx();
    initTableFilter();
    initTableView(fileNames);
}

void TableBox::initTableFilter(){

    sortButton->setFixedSize(50,50);
    sortButton->setStyleSheet("background-color :#E1912F");
    connect(sortButton,&QPushButton::clicked,this,&TableBox::displayTableFilter);

    QCheckBox *nom = new QCheckBox("Name",sortBox);
    nom->setCheckState(Qt::Checked);
    connect(nom,&QCheckBox::stateChanged,sortTable,&SortTable::editNameColumn);

    QCheckBox *syntax = new QCheckBox("Syntax",sortBox);
    syntax->setCheckState(Qt::Checked);
    connect(syntax,&QCheckBox::stateChanged,sortTable,&SortTable::editSyntaxColumn);

    QCheckBox *semantic = new QCheckBox("Semantic",sortBox);
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

    sortDock->setMinimumSize(250,250);
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

void TableBox::initRegEx()
{
    //init of the regex for the test of the pattern
    regexTestPattern.setPattern(combinedPattern);
    regexTestPattern.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
}


void TableBox::searchProcessing(){
    text = input.trimmed();
    if (text.isEmpty()){
        for (int i = 0; i < sortTable->rowCount(); ++i)
            sortTable->setRowHidden(i, false);
        return;
    }
    if (regexTestPattern.match(text).hasMatch()){
        qDebug()<<"Le pattern est bon";
        if (text.contains(":")){
            qDebug()<<"tag";
            tagsProcessing(text);
        }
        else if (text.contains(",")){
            qDebug()<<"multiple";
            multipleTextProcessing(text);
        }
        else{
            qDebug()<<"simple";
            simpleTextProcessing(text);
        }
    }
    else{
        qDebug()<<"Le pattern est PAS BON";
        return;
    }
}


void TableBox::filterTextRows(QRegularExpression regex)
{
    for (int i = 0; i < sortTable->rowCount(); ++i) {
        bool match = false;
        for (int j = 0; j < sortTable->columnCount(); ++j) {
            QTableWidgetItem *item = sortTable->item(i, j);
            if (item && regex.match(item->text()).hasMatch()) {
                match = true;
                break;
            }
        }
        sortTable->setRowHidden(i, !match);
    }
}




void TableBox::filterTaggedTextRows(QList <QRegularExpression> regexList, QList<int> selectedColumns)
{
    for (int i = 0; i < sortTable->rowCount(); ++i) {
        bool match = false;

        for (int &j : selectedColumns) {
            QTableWidgetItem *item = sortTable->item(i, j);
            if (item && regexList[j].match(item->text()).hasMatch()) {
                match = true;
                break;
            }
        }
        sortTable->setRowHidden(i, !match);
    }
}


void TableBox::simpleTextProcessing(QString query)
{
    QRegularExpression regex(query, QRegularExpression::CaseInsensitiveOption);
    filterTextRows(regex);
}


void TableBox::multipleTextProcessing(QString query)
{
    queriesList= query.split(",", Qt::SkipEmptyParts);

    //cleaning elements
    for (QString &elem : queriesList) {
        elem = elem.trimmed();
        qDebug()<<elem;
    }

    QString pattern = queriesList.join("|");
    qDebug()<<pattern;
    QRegularExpression regex(pattern, QRegularExpression::CaseInsensitiveOption);

    filterTextRows(regex);
}

void TableBox::tagsProcessing(QString query)
{
    //on vérifie que les tags sont bon
    //ensuite on prend seulement les colonnes qui nous intéressent
    //

    initSelectedColumns(true);

    queriesList = query.split(";", Qt::SkipEmptyParts);
    qDebug()<<queriesList;


    QStringList searchedTags;
    QList<QRegularExpression> regexList;

    for (QString &elem : queriesList){
        QStringList elemList = elem.split(":");
        searchedTags.append(elemList[0].trimmed());

        QRegularExpression regex(elemList[1].split(",", Qt::SkipEmptyParts).join("|").trimmed(), QRegularExpression::CaseInsensitiveOption);
        regexList.append(regex);
    }
    qDebug()<<searchedTags;


    //version sans recup de l'indice

    for (QString &elem : searchedTags){
        if (!(sortTable->getHeaderList().contains(elem, Qt::CaseInsensitive))){
            qDebug()<<"Tu le sors d'où ce tag ??";
            return ;
        }
    }


    //Version avec recup de l'indice


    for (int var = 0; var < sortTable->getHeaderList().size(); var++) {
        if (searchedTags.contains( sortTable->getHeaderList()[var])){
            selectedColumns.append(var);
        }
    }
    // si la taille de selectedColumn < searchedTags -> un tag n'est pas bon
    if(selectedColumns.size()!= searchedTags.size()){
        qDebug()<<"Tu le sors d'où ce tag ??";
        return;
    }


}


void TableBox::cleanSortTable()
{    
    input = textZone->text();
    if (input.trimmed() == "") {
        searchProcessing();
    }
}

void TableBox::initSelectedColumns(bool isTagSearch)
{
    selectedColumns.clear();
}


