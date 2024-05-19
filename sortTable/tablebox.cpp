#include "tablebox.h"

using namespace std;


TableBox::TableBox(QMap<QString, CopyInfo> const& copies, QWidget *dockParent, QWidget *parent) : QGroupBox(parent), firstAppearence(true) {
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
    initTableView(copies);
    connect(sortTable, &QTableWidget::cellClicked, this, &TableBox::sendNewFilePaths);
}

void TableBox::initTableFilter(){

    sortButton->setFixedSize(50,50);
    sortButton->setStyleSheet("background-color :#E1912F");
    connect(sortButton,&QPushButton::clicked,this,&TableBox::displayTableFilter);

    QCheckBox *copy = new QCheckBox("Copy",sortBox);
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

void TableBox::initTableView(QMap<QString, CopyInfo> const& copies){

    QHBoxLayout *sortButtonLayout = new QHBoxLayout;
    sortButtonLayout->addWidget(textZone);

    sortButtonLayout->addWidget(sortButton);

    QVBoxLayout *evalLayout = new QVBoxLayout;
    evalLayout->addLayout(sortButtonLayout);

    evalLayout->addWidget(searchInfo);

    evalLayout->addWidget(sortTable);

    sortTable->initSortTable(copies);

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
    //QString filePath(sortTable->item(row,SortTable::COL_PATH)->text());
    //QString syntaxVal(sortTable->item(row,SortTable::COL_SYNTAX)->data(Qt::UserRole).toString());
    //QString fileIdentifier(sortTable->item(row,SortTable::COL_COPY)->text());
    //qDebug() << "filePath" << filePath;
    //qDebug() << syntaxVal;
    /*for (coordinates const& coordinate : *data.documentFields){
        qDebug() << coordinate.clef << coordinate.x << coordinate.y << coordinate.h << coordinate.w;
    }*/
    //emit sendDataToPreview(filePath, data, col);
}

void TableBox::searchProcessing(){
    searchInfo->setText("");

    input = textZone->text();
    emptySearchRes = true;

    QString text = input.remove(" ");

    if (regexTestPattern.match(text).hasMatch()){
        //qDebug()<<"Le pattern est bon";
        if (text.contains(":")){
            tagsProcessing(text);
        }
        else if (text.contains(",")){
            initSelectedColumns(false);
            multipleTextProcessing(text);
        }
        else{
            initSelectedColumns(false);
            simpleTextProcessing(text);
        }
    }
    else{
        //qDebug()<<"Le pattern n'est pas bon";
        searchInfo->setText("Incorrect search. For further informations, head to the help.");
        return;
    }
}

void TableBox::cleanSortTable()
{
    if (((textZone->text()).remove(" ")).isEmpty()) {
        for (int i = 0; i < sortTable->rowCount(); ++i)
        {
            sortTable->setRowHidden(i, false);
        }
        searchInfo->setText("");
    }
}


void TableBox::simpleTextProcessing(QString& querylocale)
{
    QRegularExpression regex(querylocale, QRegularExpression::CaseInsensitiveOption);
    filterTextRows(regex);
}


void TableBox::multipleTextProcessing(QString& querylocale)
{
    QStringList queriesList= querylocale.split(",", Qt::SkipEmptyParts);
    QString pattern = queriesList.join("|");
    QRegularExpression regex(pattern, QRegularExpression::CaseInsensitiveOption);
    filterTextRows(regex);
}

void TableBox::tagsProcessing(QString& querylocale)
{
    QStringList queriesList = querylocale.split(";", Qt::SkipEmptyParts);
    QStringList *searchedTags = new QStringList();
    QList<QRegularExpression> regexList;

    for (QString &elem : queriesList){
        QStringList *elemList = new QStringList(elem.split(":"));
        searchedTags->append((*elemList)[0].trimmed());
        QString pattern = (*elemList)[1].split(",", Qt::SkipEmptyParts).join("|");
        QRegularExpression regex(pattern, QRegularExpression::CaseInsensitiveOption);
        regexList.append(regex);

        delete elemList;
    }

    // column selection for tagged search
    initSelectedColumns(true);
    for (int var = 0; var < sortTable->getHeaderList().size(); var++) {

        QString *word = new QString((sortTable->getHeaderList()).at(var));

        if (searchedTags->contains(*word, Qt::CaseInsensitive)){
            selectedColumns.append(var);
        }
        delete word;
    }
    if(selectedColumns.size()!= searchedTags->size()){
        searchInfo->setText("Incorrect search. For further informations, head to the help.");
        delete searchedTags;
        return;
    }
    delete searchedTags;
    filterTaggedTextRows(regexList);
}

void TableBox::initSelectedColumns(bool isTagSearch)
{
    selectedColumns.clear();

    if(!(isTagSearch)){
        for (int var = 0; var < sortTable->columnCount(); var++) {
            if(!(sortTable->isColumnHidden(var))){
                selectedColumns.append(var);
            }
        }
    }

    qDebug()<< selectedColumns;
}


void TableBox::filterTextRows(QRegularExpression regex)
{
    // first we clean our list of meant words
    meantSearchesList.clear();


    for (int i = 0; i < sortTable->rowCount(); ++i) {
        bool match = false;
        for (int j = 0; j < selectedColumns.size(); j++) {

            int selectedJIndex = selectedColumns[j];
            QTableWidgetItem *item = sortTable->item(i, selectedJIndex);

            if (item){
                QString cellText = (selectedJIndex == SortTable::COL_SYNTAX) ? item->data(Qt::UserRole).toString() : item->text();
                if (regex.match(cellText).hasMatch()) {
                    match = true;
                    emptySearchRes = false;
                    break;
                }
                //we want to limit to the three nearest words of our input
                if(meantSearchesList.size()<3){
                    //qDebug()<<"size ça passe";
                    meantSearchesList = fuzzySearch(meantSearchesList, cellText, regex, 3);
                }
            }

        }
        sortTable->setRowHidden(i, !match);
    }

    //print the resultat of the fuzzy search
    printFuzzySearchRes(meantSearchesList);
}



// to-do : try to factorize with the function above
void TableBox::filterTaggedTextRows(QList <QRegularExpression> regexList)
{

    for (int i = 0; i < sortTable->rowCount(); i++) {
        bool match = false;

        for (int j = 0; j < selectedColumns.size(); j++) {

            int selectedIndex = selectedColumns[j];
            QTableWidgetItem *item = sortTable->item(i, selectedIndex);

            if(item){
                QString cellText = (selectedIndex == SortTable::COL_SYNTAX) ? item->data(Qt::UserRole).toString() : item->text();
                if (regexList[j].match(cellText).hasMatch()){
                    match = true;

                }
                else{
                    match = false;
                    break;
                }
            }
        }
        emptySearchRes = false;
        sortTable->setRowHidden(i, !match);
    }
}



void TableBox::filterRows(QList<QRegularExpression> regexList){
    meantSearchesList.clear();
    int regexListSize = regexList.size();
    for (int i = 0; i < sortTable->rowCount(); i++)
    {
        bool match = false;

        for (int j = 0; j < selectedColumns.size(); j++)
        {
            int selectedJIndex = selectedColumns[j];
            QTableWidgetItem *item = sortTable->item(i, selectedJIndex);

            if(item)
            {
                QString *cellText = new QString((selectedJIndex == SortTable::COL_SYNTAX) ? item->data(Qt::UserRole).toString() : item->text());

                // cas de la recherche simple ou multiple
                if ( (regexListSize==0) && (regexList[0].match(*cellText).hasMatch()) ) {
                    match = true;
                    emptySearchRes = false;
                    delete cellText;
                    break;
                }

                // cas du tagged search -> liste de regex
                else if((regexListSize>0)&&(regexList[j].match(*cellText).hasMatch()))
                {
                    match = true;
                }
                else{
                    match = false;
                    delete cellText;
                    break;
                }

                //we want to limit to the *three* nearest words of our input
                if(meantSearchesList.size()<3){
                    meantSearchesList = fuzzySearch(meantSearchesList, *cellText, ((regexListSize==0)?regexList[0]:regexList[j]), 3);
                }

                delete cellText;
            }
        }
        sortTable->setRowHidden(i, !match);
    }
    //print the resultat of the fuzzy search
    printFuzzySearchRes(meantSearchesList);
}




int TableBox::levenshteinDistance(QString str1, QString str2){

    int substitutionCost, len1(str1.size()), len2(str2.size());

    // Create a matrix to store the distances
    vector< vector<int> > dist(len1+1, vector<int>(len2+1));

    //matrice init
    for (int i = 0; i <= len1; ++i)
        dist[i][0] =i;

    for (int j = 0; j <= len2; ++j)
        dist[0][j] = j;


    for (int i = 1; i <= len1; ++i)
    {
        for (int j = 1; j <= len2; ++j)
        {
            if (str1[i-1]== str2[j-1])
            {
                substitutionCost =0;
            }
            else
            {
                substitutionCost = 1;
            }

            dist[i][j] = min(dist[i - 1][j] + 1,
                             min(dist[i][j - 1] + 1,
                                 dist[i - 1][j - 1] + substitutionCost));

        }
    }
    return dist[len1][len2];
}

QStringList TableBox::fuzzySearch(QStringList meantSearchesList, QString cellText, QRegularExpression regex, int threshold)
{
    if(!(meantSearchesList.contains(cellText)))
    {
        if(levenshteinDistance(cellText, regex.pattern())<=threshold)
        {
            meantSearchesList.push_back(cellText);
        }
    }
    return meantSearchesList;
}

void TableBox::printFuzzySearchRes(QStringList meantSearchesList)
{
    if ((emptySearchRes)&&(meantSearchesList.size()))
    {
        QString sentence = "Did you mean : ";
        int sizeList = meantSearchesList.size();
        for (int var = 0; var < sizeList; var++)
        {
            sentence += meantSearchesList[var];
            if(var < sizeList - 1)
            {
                sentence += " or ";
            }

        }
        sentence += " ?";
        searchInfo->setText(sentence);
    }
}


