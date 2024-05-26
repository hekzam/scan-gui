#include "tablebox.h"

using namespace std;

TableBox::TableBox(std::map<QString, SubjectInfo> &copies, QWidget *dockParent,
                   QWidget *parent)
    : QGroupBox(parent), firstAppearence(true)
{
  setTitle("Evaluation table");
  sortBox = new QGroupBox(this);

  textZone = new QLineEdit(this);
  textZone->setPlaceholderText("Enter your search and click enter.");
  connect(textZone, &QLineEdit::textChanged, this, &TableBox::cleanSortTable);
  connect(textZone, &QLineEdit::returnPressed, this,
          &TableBox::searchProcessing);

  sortButton = new QPushButton("Sort", this);

  searchInfo = new QLabel(this);

  fieldViewToggle = new QCheckBox("Enable field view", sortBox);
  groupTable = new GroupViewTable(copies, this);
  fieldTable = new FieldViewTable(copies, this);

  // ajout à la liste de vue
  sortTableList.push_back(fieldTable);
  sortTableList.push_back(groupTable);

  // Stack widget to store both tables
  tableWidget = new QStackedWidget;
  tableWidget->addWidget(groupTable);
  tableWidget->addWidget(fieldTable);
  tableWidget->setCurrentWidget(groupTable);

  sortDock = new QDockWidget(dockParent);
  sortDock->hide();

  initRegEx();
  initTableFilter();
  initTableView();
  actualTable = (sortTableList.at(actualView));
  connect(groupTable, &QTableWidget::cellClicked, this, &TableBox::collectData);
  connect(fieldTable, &QTableWidget::cellClicked, this, &TableBox::collectData);
}

SortTable *TableBox::getFieldTable()
{
  return fieldTable;
}

void TableBox::initTableFilter()
{

  sortButton->setFixedSize(50, 50);
  sortButton->setStyleSheet("background-color :#E1912F");
  connect(sortButton, &QPushButton::clicked, this,
          &TableBox::displayTableFilter);

  QCheckBox *subject = new QCheckBox("Exam", sortBox);
  subject->setCheckState(Qt::Checked);
  connect(subject, &QCheckBox::stateChanged, this,
          [this](int state)
          {
            groupTable->editColumn(state, groupTable->COL_SUBJECT);
            fieldTable->editColumn(state, fieldTable->COL_SUBJECT);
          });

  QCheckBox *copy = new QCheckBox("Copy", sortBox);
  copy->setCheckState(Qt::Checked);
  connect(copy, &QCheckBox::stateChanged, this,
          [this](int state)
          {
            groupTable->editColumn(state, groupTable->COL_COPY);
            fieldTable->editColumn(state, fieldTable->COL_COPY);
          });

  QCheckBox *page = new QCheckBox("Page", sortBox);
  page->setCheckState(Qt::Checked);
  connect(page, &QCheckBox::stateChanged, this,
          [this](int state)
          {
            groupTable->editColumn(state, groupTable->COL_PAGE);
            fieldTable->editColumn(state, fieldTable->COL_PAGE);
          });

  QCheckBox *field = new QCheckBox("Field", sortBox);
  field->setCheckState(Qt::Checked);
  connect(field, &QCheckBox::stateChanged, this,
          [this](int state)
          {
            groupTable->editColumn(state, groupTable->COL_FIELD);
            fieldTable->editColumn(state, fieldTable->COL_FIELD);
          });

  QCheckBox *syntax = new QCheckBox("Syntax", sortBox);
  syntax->setCheckState(Qt::Checked);
  connect(syntax, &QCheckBox::stateChanged, this,
          [this](int state)
          {
            groupTable->editColumn(state, groupTable->COL_SYNTAX);
            fieldTable->editColumn(state, fieldTable->COL_SYNTAX);
          });

  QCheckBox *semantic = new QCheckBox("Semantic", sortBox);
  semantic->setCheckState(Qt::Checked);
  connect(semantic, &QCheckBox::stateChanged, this,
          [this](int state)
          {
            groupTable->editColumn(state, groupTable->COL_SEMANTIC);
            fieldTable->editColumn(state, fieldTable->COL_SEMANTIC);
          });

  QCheckBox *metric1 = new QCheckBox("Metric 1", sortBox);
  connect(metric1, &QCheckBox::stateChanged, this,
          [this](int state)
          {
            groupTable->editColumn(state, groupTable->COL_MET1);
            fieldTable->editColumn(state, fieldTable->COL_MET1);
          });

  QCheckBox *metric2 = new QCheckBox("Metric 2", sortBox);
  connect(metric2, &QCheckBox::stateChanged, this,
          [this](int state)
          {
            groupTable->editColumn(state, groupTable->COL_MET2);
            fieldTable->editColumn(state, fieldTable->COL_MET2);
          });

  QVBoxLayout *sortBoxLayout = new QVBoxLayout;
  sortBoxLayout->setSpacing(10);
  sortBoxLayout->addWidget(subject);
  sortBoxLayout->addWidget(copy);
  sortBoxLayout->addWidget(page);
  sortBoxLayout->addWidget(field);
  sortBoxLayout->addWidget(syntax);
  sortBoxLayout->addWidget(semantic);
  sortBoxLayout->addWidget(metric1);
  sortBoxLayout->addWidget(metric2);

  sortDock->setMinimumSize(250, 250);
  sortBox->setLayout(sortBoxLayout);
  sortDock->setWidget(sortBox);
  sortDock->setFloating(true);
}

void TableBox::displayTableFilter()
{
  if (!sortDock->isHidden())
  {
    sortDock->hide();
  }
  else
  {
    if (firstAppearence)
    {
      QPoint pos = groupTable->mapToGlobal(groupTable->rect().topRight());
      pos.setX(pos.x() - sortDock->width());
      sortDock->move(pos);
      firstAppearence = false;
    }
    sortDock->show();
  }
}

void TableBox::connectFieldViewToggle()
{
  connect(fieldViewToggle, &QCheckBox::stateChanged, this,
          [this](int state)
          {
            QScrollBar *fieldScrollX = fieldTable->horizontalScrollBar();
            QScrollBar *fieldScrollY = fieldTable->verticalScrollBar();
            QScrollBar *groupScrollX = groupTable->horizontalScrollBar();
            QScrollBar *groupScrollY = groupTable->verticalScrollBar();

            int otherView = !actualView;

            for (int column = 0; column < sortTableList.at(actualView)->model()->columnCount(); ++column)
            {
              sortTableList.at(otherView)->setColumnWidth(column, sortTableList.at(actualView)->columnWidth(column));
            }

            if (state)
            {
              fieldScrollX->setValue(groupScrollX->value());
              if (groupScrollY->value() == groupScrollY->maximum())
                fieldScrollY->setValue(fieldScrollY->maximum());
              else
                fieldScrollY->setValue(groupScrollY->value());
              tableWidget->setCurrentWidget(fieldTable);
              actualView = 0;
            }
            else
            {
              groupScrollX->setValue(fieldScrollX->value());
              if (fieldScrollY->value() == fieldScrollY->maximum())
                groupScrollY->setValue(groupScrollY->maximum());
              else
                groupScrollY->setValue(fieldScrollY->value());
              tableWidget->setCurrentWidget(groupTable);
              actualView = 1;
            }
            actualTable = (sortTableList.at(actualView));
          });
}

void TableBox::initTableView()
{

  QHBoxLayout *sortButtonLayout = new QHBoxLayout;
  sortButtonLayout->addWidget(textZone);

  sortButtonLayout->addWidget(sortButton);

  QVBoxLayout *evalLayout = new QVBoxLayout;

  connectFieldViewToggle();

  // Checkbox to display one of the tables
  evalLayout->addLayout(sortButtonLayout);
  evalLayout->addWidget(searchInfo);
  evalLayout->addWidget(fieldViewToggle);
  evalLayout->addWidget(tableWidget);

  groupTable->initSortTable();
  fieldTable->initSortTable();

  setLayout(evalLayout);
}

void TableBox::initRegEx()
{
  // init of the regex for the test of the pattern
  regexTestPattern.setPattern(combinedPattern);
  regexTestPattern.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
}

void TableBox::collectData(int row, int col)
{
  QTableWidgetItem *item = sortTableList.at(actualView)->item(row, col);
  if (!item)
    return;

  QTableWidgetItem *subjectItem =
      sortTableList.at(actualView)->item(row, SortTable::COL_SUBJECT);
  QVariant subjectVariant = subjectItem->data(Qt::UserRole);
  if (!subjectVariant.isValid())
    return;

  SubjectInfo *subject = subjectVariant.value<SubjectInfo *>();
  dataCopieJSON *data = subject->getData();
  if (!data)
    return;

  // preview needs the page number, 1 by default
  int pageNumberToDisplay = 1;

  QStringList paths;
  switch (col)
  {
  case (SortTable::COL_COPY):
  {
    QVariant copyVariant = item->data(Qt::UserRole);
    if (!copyVariant.isValid())
      return;
    CopyInfo *copy = copyVariant.value<CopyInfo *>();
    if (!copy->getCopyInFiles())
      return;
    paths.append(copy->getPagesPathList());
    break;
  }
  case (SortTable::COL_PAGE):
  case (SortTable::COL_FIELD):
  {
    QVariant pageVariant = item->data(Qt::UserRole);
    if (!pageVariant.isValid())
      return;
    PageInfo *page = pageVariant.value<PageInfo *>();
    if (!page->getPageInFiles())
      return;
    paths.append(page->getFilePath());
    pageNumberToDisplay = page->getPageInJSON(); // this is the pagenumber
    break;
  }
  default:
  {
    return;
  }
  };

  QString fieldName = (col == SortTable::COL_FIELD) ? item->text() : "";
  paths.removeAll("");
  qDebug() << paths;
  emit sendDataToPreview(paths, data, pageNumberToDisplay, fieldName);
}

void TableBox::searchProcessing()
{
  searchInfo->setText("");

  input = textZone->text();
  emptySearchRes = true;

  QString text = input.remove(" ");

  if (text.isEmpty())
    return;

  if (regexTestPattern.match(text).hasMatch())
  {
    // qDebug()<<"Le pattern est bon";
    if (text.contains(":"))
    {
      tagsProcessing(text);
    }
    else if (text.contains(","))
    {
      initSelectedColumns(false);
      multipleTextProcessing(text);
    }
    else
    {
      initSelectedColumns(false);
      simpleTextProcessing(text);
    }
  }
  else
  {
    // qDebug()<<"Le pattern n'est pas bon";
    searchInfo->setText(
        "Incorrect search. For further informations, head to the help.");
    return;
  }
}

void TableBox::cleanSortTable()
{
  if (((textZone->text()).remove(" ")).isEmpty())
  {
    for (int i = 0; i < groupTable->rowCount(); ++i)
    {
      groupTable->setRowHidden(i, false);
      fieldTable->setRowHidden(i, false);
    }
    searchInfo->setText("");
  }
}

void TableBox::simpleTextProcessing(QString &querylocale)
{
  QRegularExpression regex(querylocale,
                           QRegularExpression::CaseInsensitiveOption);
  filterTextRows(regex);
}

void TableBox::multipleTextProcessing(QString &querylocale)
{
  QStringList queriesList = querylocale.split(",", Qt::SkipEmptyParts);
  QString pattern = queriesList.join("|");
  QRegularExpression regex(pattern, QRegularExpression::CaseInsensitiveOption);
  filterTextRows(regex);
}

void TableBox::tagsProcessing(QString &querylocale)
{
  QStringList queriesList = querylocale.split(";", Qt::SkipEmptyParts);
  QStringList *searchedTags = new QStringList();
  QList<QRegularExpression> regexList;

  for (QString &elem : queriesList)
  {
    QStringList *elemList = new QStringList(elem.split(":"));
    searchedTags->append((*elemList)[0].trimmed());
    QString pattern = (*elemList)[1].split(",", Qt::SkipEmptyParts).join("|");
    QRegularExpression regex(pattern,
                             QRegularExpression::CaseInsensitiveOption);
    regexList.append(regex);

    delete elemList;
  }

  // column selection for tagged search
  initSelectedColumns(true);
  for (int var = 0; var < actualTable->getHeaderList().size(); var++)
  {

    QString *word = new QString((actualTable->getHeaderList()).at(var));

    if (searchedTags->contains(*word, Qt::CaseInsensitive))
    {
      selectedColumns.append(var);
    }
    delete word;
  }
  if (selectedColumns.size() != searchedTags->size())
  {
    searchInfo->setText(
        "Incorrect search. For further informations, head to the help.");
    delete searchedTags;
    return;
  }
  delete searchedTags;

  filterTaggedTextRows(regexList);
}

void TableBox::initSelectedColumns(bool isTagSearch)
{
  selectedColumns.clear();

  if (!(isTagSearch))
  {
    for (int var = 0; var < actualTable->columnCount(); var++)
    {
      if (!(actualTable->isColumnHidden(var)))
      {
        selectedColumns.append(var);
      }
    }
  }
}

void TableBox::filterTextRows(QRegularExpression regex)
{
  // first we clean our list of meant words for the fuzzy search
  meantSearchesList.clear();

  for (int i = 0; i < actualTable->rowCount(); ++i)
  {
    bool match = false;
    for (int j = 0; j < selectedColumns.size(); j++)
    {

      int selectedJIndex = selectedColumns[j];
      QTableWidgetItem *item = actualTable->item(i, selectedJIndex);

      if (item)
      {
        QString cellText = (selectedJIndex == SortTable::COL_SYNTAX)
                               ? item->data(Qt::UserRole).toString()
                               : item->text();
        if (regex.match(cellText).hasMatch())
        {
          match = true;
          emptySearchRes = false;
          break;
        }
        // we want to limit to the three nearest words of our input
        if (meantSearchesList.size() < 3)
        {
          // qDebug()<<"size ça passe";
          meantSearchesList =
              fuzzySearch(meantSearchesList, cellText, regex, 3);
        }
      }
    }
    actualTable->setRowHidden(i, !match);
    // groupTable->setRowHidden(i, !match);
    // fieldTable->setRowHidden(i, !match);
  }

  // print the resultat of the fuzzy search
  printFuzzySearchRes(meantSearchesList);
}

// to-do : try to factorize with the function above
void TableBox::filterTaggedTextRows(QList<QRegularExpression> regexList)
{

  for (int i = 0; i < actualTable->rowCount(); i++)
  {
    bool match = false;

    for (int j = 0; j < selectedColumns.size(); j++)
    {

      int selectedIndex = selectedColumns[j];
      QTableWidgetItem *item = actualTable->item(i, selectedIndex);

      if (item)
      {
        QString cellText = (selectedIndex == SortTable::COL_SYNTAX)
                               ? item->data(Qt::UserRole).toString()
                               : item->text();
        if (regexList[j].match(cellText).hasMatch())
        {
          match = true;
        }
        else
        {
          match = false;
          break;
        }
      }
    }
    emptySearchRes = false;
    actualTable->setRowHidden(i, !match);
    // groupTable->setRowHidden(i, !match);
    // fieldTable->setRowHidden(i, !match);
  }
}

// a try to refactor the two methods above
// isn't actualise with table system
void TableBox::filterRows(QList<QRegularExpression> regexList)
{
  meantSearchesList.clear();
  int regexListSize = regexList.size();
  for (int i = 0; i < groupTable->rowCount(); i++)
  {
    bool match = false;

    for (int j = 0; j < selectedColumns.size(); j++)
    {
      int selectedJIndex = selectedColumns[j];
      QTableWidgetItem *item = groupTable->item(i, selectedJIndex);

      if (item)
      {
        QString *cellText =
            new QString((selectedJIndex == SortTable::COL_SYNTAX)
                            ? item->data(Qt::UserRole).toString()
                            : item->text());

        // cas de la recherche simple ou multiple
        if ((regexListSize == 0) && (regexList[0].match(*cellText).hasMatch()))
        {
          match = true;
          emptySearchRes = false;
          delete cellText;
          break;
        }

        // cas du tagged search -> liste de regex
        else if ((regexListSize > 0) &&
                 (regexList[j].match(*cellText).hasMatch()))
        {
          match = true;
        }
        else
        {
          match = false;
          delete cellText;
          break;
        }

        // we want to limit to the *three* nearest words of our input
        if (meantSearchesList.size() < 3)
        {
          meantSearchesList = fuzzySearch(
              meantSearchesList, *cellText,
              ((regexListSize == 0) ? regexList[0] : regexList[j]), 3);
        }

        delete cellText;
      }
    }
    groupTable->setRowHidden(i, !match);
    fieldTable->setRowHidden(i, !match);
  }
  // print the resultat of the fuzzy search
  printFuzzySearchRes(meantSearchesList);
}

int TableBox::levenshteinDistance(QString str1, QString str2)
{

  int substitutionCost, len1(str1.size()), len2(str2.size());

  // Create a matrix to store the distances
  vector<vector<int>> dist(len1 + 1, vector<int>(len2 + 1));

  // matrice init
  for (int i = 0; i <= len1; ++i)
    dist[i][0] = i;

  for (int j = 0; j <= len2; ++j)
    dist[0][j] = j;

  for (int i = 1; i <= len1; ++i)
  {
    for (int j = 1; j <= len2; ++j)
    {
      if (str1[i - 1] == str2[j - 1])
      {
        substitutionCost = 0;
      }
      else
      {
        substitutionCost = 1;
      }

      dist[i][j] =
          min(dist[i - 1][j] + 1,
              min(dist[i][j - 1] + 1, dist[i - 1][j - 1] + substitutionCost));
    }
  }
  return dist[len1][len2];
}

QStringList TableBox::fuzzySearch(QStringList meantSearchesList,
                                  QString cellText, QRegularExpression regex,
                                  int threshold)
{
  if (!(meantSearchesList.contains(cellText)))
  {
    if (levenshteinDistance(cellText, regex.pattern()) <= threshold)
    {
      meantSearchesList.push_back(cellText);
    }
  }
  return meantSearchesList;
}

void TableBox::printFuzzySearchRes(QStringList meantSearchesList)
{
  if ((emptySearchRes) && (meantSearchesList.size()))
  {
    QString sentence = "Did you mean : ";
    int sizeList = meantSearchesList.size();
    for (int var = 0; var < sizeList; var++)
    {
      sentence += meantSearchesList[var];
      if (var < sizeList - 1)
      {
        sentence += " or ";
      }
    }
    sentence += " ?";
    searchInfo->setText(sentence);
  }
}
