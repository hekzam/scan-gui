#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <qtablewidget.h>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QCheckBox>
#include <QGroupBox>
#include "sorttable.h"

#pragma once
#include "controller.h"
class Controller;

class Boundary : public QMainWindow
{
  Q_OBJECT
public:
  explicit Boundary(QWidget *parent = nullptr);
  void init();
  ~Boundary();
  void setController(Controller *controller);
  void initTableView(QStringList const &fileNames);

private:
  QWidget *menuWindow;
  QWidget *tableWindow;
  QGroupBox *checkBoxGroup;
  bool groupBoxDisplayed = false;
  QStackedLayout *tableLayout;
  SortTable *sortTable;
  Controller *controlWindow;

  void initTableFilter();
private slots:
  void ouvrirExplorateurFichier();
  void displayTableFilter();

signals:
};

#endif // BOUNDARY_H
