/*TO-DO
 *       -> ajouter les recherches avancées (avec des balises à la façon de Gmail
 *       ex:  student: Emilien,Marco ; id: 221 )
*/

#include "tablebox.h"

TableBox::TableBox(QList<JsonLinker::fieldInfo> const& fields, QWidget *dockParent, QMap<QString, dataCopieJSON*> const& fileDataMap, QWidget *parent) : QGroupBox(parent), firstAppearence(true), m_fileDataMap(fileDataMap) {
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
    initTableView(fields);
    connect(sortTable, &QTableWidget::cellClicked, this, &TableBox::sendNewFilePaths);
}

void TableBox::initTableFilter(){

    sortButton->setFixedSize(50,50);
    sortButton->setStyleSheet("background-color :#E1912F");
    connect(sortButton,&QPushButton::clicked,this,&TableBox::displayTableFilter);

    QCheckBox *copy = new QCheckBox("Page",sortBox);
    copy->setCheckState(Qt::Checked);
    connect(copy,&QCheckBox::stateChanged,this,[this](int state){
        sortTable -> editColumn(state, sortTable -> COL_COPY);
    });

    QCheckBox *page = new QCheckBox("Page",sortBox);
    page->setCheckState(Qt::Checked);
    connect(page,&QCheckBox::stateChanged,this,[this](int state){
        sortTable -> editColumn(state, sortTable -> COL_PAGE);
    });

    QCheckBox *field = new QCheckBox("Field",sortBox);
    field->setCheckState(Qt::Checked);
    connect(field,&QCheckBox::stateChanged,this,[this](int state){
        sortTable -> editColumn(state, sortTable -> COL_FIELD);
    });

    QCheckBox *syntax = new QCheckBox("Syntax",sortBox);
    syntax->setCheckState(Qt::Checked);
    connect(syntax,&QCheckBox::stateChanged,this,[this](int state){
        sortTable -> editColumn(state, sortTable -> COL_SYNTAX);
    });

    QCheckBox *semantic = new QCheckBox("Semantic",sortBox);
    semantic->setCheckState(Qt::Checked);
    connect(semantic,&QCheckBox::stateChanged,this,[this](int state){
        sortTable -> editColumn(state, sortTable -> COL_SEMANTIC);
    });

    QCheckBox *metric1 = new QCheckBox("Metric 1",sortBox);
    connect(metric1,&QCheckBox::stateChanged,this,[this](int state){
        sortTable -> editColumn(state, sortTable -> COL_MET1);
    });

    QCheckBox *metric2 = new QCheckBox("Metric 2",sortBox);
    connect(metric2,&QCheckBox::stateChanged,sortTable,[this](int state){
        sortTable -> editColumn(state, sortTable -> COL_MET2);
    });

    QVBoxLayout *sortBoxLayout = new QVBoxLayout;
    sortBoxLayout->setSpacing(10);
    sortBoxLayout->addWidget(copy);
    sortBoxLayout->addWidget(page);
    sortBoxLayout->addWidget(field);
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

void TableBox::initTableView(QList<JsonLinker::fieldInfo> const& fields){

    QHBoxLayout *sortButtonLayout = new QHBoxLayout;
    sortButtonLayout->addWidget(textZone);

    sortButtonLayout->addWidget(searchInfo);

    sortButtonLayout->addWidget(sortButton);

    QVBoxLayout *evalLayout = new QVBoxLayout;
    evalLayout->addLayout(sortButtonLayout);

    evalLayout->addWidget(searchInfo);

    evalLayout->addWidget(sortTable);

    sortTable->initSortTable(fields);

    setLayout(evalLayout);
}

void TableBox::initRegEx()
{
    //init of the regex for the test of the pattern
    regexTestPattern.setPattern(combinedPattern);
    regexTestPattern.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
}

void TableBox::sendNewFilePaths(int row, int col)
{
    QString filePath(sortTable->item(row,SortTable::COL_PATH)->text());
    QString syntaxVal(sortTable->item(row,SortTable::COL_SYNTAX)->data(Qt::UserRole).toString());
    QString fileIdentifier(sortTable->item(row,SortTable::COL_COPY)->text());
    dataCopieJSON const& data = *m_fileDataMap[fileIdentifier];
    qDebug() << "filePath" << filePath;
    qDebug() << syntaxVal;
    for (coordinates const& coordinate : *data.documentFields){
        qDebug() << coordinate.clef << coordinate.x << coordinate.y << coordinate.h << coordinate.w;
    }
    emit sendDataToPreview(filePath, data, col);
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
        searchInfo->setText("Le format n'est pas bon !!!");
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
            if (item){
                QString cellText = (selectedIndex == SortTable::COL_SYNTAX) ? item->data(Qt::UserRole).toString() : item->text();
                if (item && regex.match(cellText).hasMatch()) {
                    match = true;
                    break;
                }
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

            if(item){
                QString cellText = (selectedIndex == SortTable::COL_SYNTAX) ? item->data(Qt::UserRole).toString() : item->text();
                if (item && regexList[j].match(cellText).hasMatch()){
                    match = true;
                }
                else{
                    match = false;
                    break;
                }
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


