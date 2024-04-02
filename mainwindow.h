#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QSplitter>
#include <QStackedWidget>
#include <QDebug>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void deleteChildren(QObject *child);

private:
  void createMenuBar();
  void createStatusBar(); // do we need one

  void createWelcomeView(); // créer importer un projet
  void createConfigView(); // configurer un nouveau projet (QDialog ??)
  void createWorkingView(); // main working window
  void createMainStack(); //holds the different views
  // colonnes
  void createSearchableColumns(); // not my problem XD
  // preview
  void createDockablePreview(); //holy sh*t do we really need this ??
  void createFixedPreview(); // (help)
  void createPreviewStack();
  void createPreviewPanel();

  // Menu Bar and children
  QMenuBar *mainWindowMenuBar;
  QLabel *currentViewTitle;
  QMenu *fileMenu, *editMenu, *viewMenu, *helpMenu;

  /**
   * Ce QStackedWidget nous permettra d'afficher soit: (voir cahier des charges)
   * - un écran nous permettant d'importer ou d'afficher un projet
   * - un écran nous permettant de configurer le projet
   * - un écran nous permettant de configurer
   *    le programme (? ou une boite de dialogue)
   * - la vue principale du programme
   * - d'autres si besoin...
   */
  QStackedWidget *mainStack;

  // Welcome Screen
  QGroupBox *welcomeView; // ????

  // Config Screen
  QGroupBox *configView; // ??????? what to use here ?

  // Main Window with columns and preview
  QSplitter *workingView;

  // part of the GUI dedicated to searching, filtering and displaying
  // columns, placeholder for a real box in the future
  QLabel *placeholderSearchableColumns;

  // In this object we place the previews and the buttons
  QGroupBox *previewPanelandButtons;

  // we place the buttons here
  QGridLayout *Buttons; //QButtonGroup ??

  //Both previews have to be Stacked I guess ??? since we can show both
  // the same time
  QStackedWidget *previewStack;

  // part of the GUI used to preview images, can be made to float
  QDockWidget *floatablePreview;

  /**
   * part of the GUI that will show only when the floatablePreviewWidget
   * is detached from the main window
   */
  QGroupBox *fixedPreview;




private slots:
  // set the QLabel in the MenuBar to the correct title
  // void setCurrentViewTitle();
  // detach the preview
  // void detachPreview();
};
#endif // MAINWINDOW_H
