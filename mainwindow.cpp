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
  fileMenu = new QMenu(tr("&Fichier"),mainWindowMenuBar);
  editMenu = new QMenu(tr("&Édition"),mainWindowMenuBar);
  viewMenu = new QMenu(tr("&Affichage"),mainWindowMenuBar);
  helpMenu = new QMenu(tr("A&ide"),mainWindowMenuBar);
  mainWindowMenuBar->addMenu(fileMenu);
  mainWindowMenuBar->addMenu(editMenu);
  mainWindowMenuBar->addMenu(viewMenu);
  mainWindowMenuBar->addMenu(helpMenu);
}

void MainWindow::createSearchableColumns()
{
  // placeholder QLabel
  placeholderSearchableColumns = new QLabel("Not my problem, WIP",this);
  // workingView->addWidget(placeholderSearchableColumns);
  placeholderSearchableColumns->setMargin(50);
  placeholderSearchableColumns->setMinimumSize(minPanelSize);
}

void MainWindow::createPreviewPanel()
{
  previewPanelandButtons = new QGroupBox("the preview + the buttons here",this);
  previewPanelandButtons->setMinimumSize(minPanelSize);
}

void MainWindow::createWorkingView()
{
  // TODO : add children to this box (preview, buttons and columns)
  // TODO : check if the parent is correct
  workingView = new QSplitter(this); //WIP
  // workingView takes ownership with addWidget (see QSplitter docs)
  workingView->addWidget(placeholderSearchableColumns);
  workingView->addWidget(previewPanelandButtons);
  workingView->setChildrenCollapsible(false);
}

void MainWindow::createMainStack()
{
  mainStack = new QStackedWidget(this);
  mainStack->addWidget(workingView);
  setCentralWidget(mainStack); // as it should be
}

MainWindow::~MainWindow(){};

