#include <QtWidgets>
#include "mainwindow.h"
static const QSize minPanelSize(250,500);

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  setMinimumSize(1280,720);
  createMenuBar();
  createSearchableColumns();
  // createPreviewStack();
  // createDockablePreview(); // floating preview
  // createFixedPreview(); //docked preview
  createPreviewPanel();
  createWorkingView();
  createMainStack();
}

void MainWindow::createMenuBar()
{
  // TODO : add QActions for each item of the menuBar
  mainWindowMenuBar = menuBar(); // menuBar is not in any layout ?
  currentViewTitle = new QLabel(tr("Name of the Window"),mainWindowMenuBar);
  currentViewTitle->setIndent(10);
  mainWindowMenuBar->setCornerWidget(currentViewTitle, Qt::TopLeftCorner);
  fileMenu = new QMenu(tr("&Fichier"),this);
  editMenu = new QMenu(tr("&Édition"),this);
  viewMenu = new QMenu(tr("&Affichage"),this);
  helpMenu = new QMenu(tr("A&ide"),this);
  mainWindowMenuBar->addMenu(fileMenu);
  mainWindowMenuBar->addMenu(editMenu);
  mainWindowMenuBar->addMenu(viewMenu);
  mainWindowMenuBar->addMenu(helpMenu);
}

void MainWindow::createSearchableColumns()
{
  // placeholder QLabel
  placeholderSearchableColumns = new QLabel("Not my problem, WIP",this);
  placeholderSearchableColumns->setMargin(50);
  placeholderSearchableColumns->setMinimumSize(minPanelSize);
}

void MainWindow::createPreviewPanel()
{
  previewPanelandButtons = new QGroupBox("the preview + the buttons here", this);
  previewPanelandButtons->setMinimumSize(minPanelSize);
}

void MainWindow::createWorkingView()
{
  // TODO : add children to this box (preview, buttons and columns)
  workingView = new QSplitter(this);
  workingView->addWidget(placeholderSearchableColumns);
  workingView->addWidget(previewPanelandButtons);
  workingView->setChildrenCollapsible(false);
}

void MainWindow::createMainStack()
{
  mainStack = new QStackedWidget;
  mainStack->addWidget(workingView);
  setCentralWidget(mainStack); // as it should be
}

// TODO : delete les pointeurs, free la mémoire etc...
MainWindow::~MainWindow(){};
// MainWindow::~MainWindow() {
//   for(QObject *child : mainStack->children()){
//     qDebug() << " Label deleted : " << child;
//     deleteChildren(child);
//   }
// }

// void MainWindow::deleteChildren(QObject *parent)
// {
//   for(QObject *child : parent->children()){
//     qDebug() << " Label deleted : " << child;
//     delete child;
//   }
// }
