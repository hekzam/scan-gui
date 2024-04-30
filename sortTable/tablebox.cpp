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

    searchInfo = new QLabel(this);

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

    evalLayout->addWidget(searchInfo);

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
    text = input.remove(" ");

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
            initSelectedColumns(false);
            qDebug()<<"multiple";
            multipleTextProcessing(text);
        }
        else{
            initSelectedColumns(false);
            qDebug()<<"simple";
            simpleTextProcessing(text);
        }
    }
    else{
        qDebug()<<"Le pattern est PAS BON";
        return;
    }
}


void TableBox::filterTextRows(QRegularExpression regex, QList<int> selectedColumns)
{
    for (int i = 0; i < sortTable->rowCount(); ++i) {

        bool match = false;



        for (int j = 0; j < selectedColumns.size(); j++) {

            int selectedIndex = selectedColumns[j];


            QTableWidgetItem *item = sortTable->item(i, selectedIndex);



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
    for (int i = 0; i < sortTable->rowCount(); i++) {
        bool match = false;

        for (int j = 0; j < selectedColumns.size(); j++) {

            int selectedIndex = selectedColumns[j];

            QTableWidgetItem *item = sortTable->item(i, selectedIndex);

            if (item && regexList[j].match(item->text()).hasMatch()){
                match = true;
            }
            else{
                match = false;
                break;
            }
        }
        sortTable->setRowHidden(i, !match);
    }
}


void TableBox::simpleTextProcessing(QString query)
{

    QRegularExpression regex(query, QRegularExpression::CaseInsensitiveOption);
    filterTextRows(regex, selectedColumns);
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

    filterTextRows(regex, selectedColumns);
}

void TableBox::tagsProcessing(QString query)
{
    //on vérifie que les tags sont bon
    //ensuite on prend seulement les colonnes qui nous intéressent
    //



    queriesList = query.split(";", Qt::SkipEmptyParts);
    qDebug()<<queriesList;


    QStringList searchedTags;
    QList<QRegularExpression> regexList;

    for (QString &elem : queriesList){
        QStringList elemList = elem.split(":");
        searchedTags.append(elemList[0].trimmed());
        QString pattern = elemList[1].split(",", Qt::SkipEmptyParts).join("|");
        QRegularExpression regex(pattern, QRegularExpression::CaseInsensitiveOption);
        regexList.append(regex);
        qDebug()<<pattern;
    }

    qDebug()<<searchedTags;

    qDebug()<<sortTable->getHeaderList();


    initSelectedColumns(true);

    for (int var = 0; var < sortTable->getHeaderList().size(); var++) {
        if (searchedTags.contains(sortTable->getHeaderList()[var],Qt::CaseInsensitive)){
            selectedColumns.append(var);
        }
    }
    if(selectedColumns.size()!= searchedTags.size()){
        qDebug()<<"tag pas bon";
        searchInfo->setText("Le format des tags n'est pas bon !");
        return;
    }

    qDebug()<<selectedColumns;
    qDebug()<<regexList;

    filterTaggedTextRows(regexList, selectedColumns);

}


void TableBox::cleanSortTable()
{    
    input = textZone->text();
    if (input.trimmed() == "") {
        searchProcessing();
    }
    searchInfo->setText("");
}

void TableBox::initSelectedColumns(bool isTagSearch)
{

    selectedColumns.clear();

    if(isTagSearch){
        return;
    }
    else{
        for (int var = 0; var < sortTable->rowCount(); var++) {
            if(!(sortTable->isColumnHidden(var))){
                selectedColumns.append(var);
            }
        }
    }

}


