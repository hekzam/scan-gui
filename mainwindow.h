/*
 * Layout of the main window module
 *
 * - QStackedWidget : switch entre les 3 vues.
 *  - QVBoxLayout : contenant la vue Menu Principal.
 *    - QPushButton * 2 : permettent de créer ou d'ouvrir un
 *      projet.
 *  - QVBoxLayout : contenant la vue Menu Création.
 *    - QFormLayout : créer un formulaire sur plusieurs niveaux.
 *      - QLineEdit * 4 : contenant chaque champ à remplir du
 *        formulaire.
 *    - QHBoxLayout + QPushButton * 2 : permettent de revenir en
 *      arrière ou de continuer.
 *  - QHBoxLayout : contenant la vue Menu Evaluation.
 *    - QSplitter : premier splitter pour diviser la page en
 *      largeur (côté tableau / preview).
 *      - QSplitter : second splitter pour diviser la partie
 *        tableau en hauteur (côté tableau / informations).
 *        - TableBox : tableau contenant les informations du
 *          scan.
 *        - QVBoxLayout : boîte d'informations et d'erreurs.
 *      - ExamPreview : boîte de preview des pages.
 *
 * - QMenuBar : accéder aux différents menus déroulant.
 *  - QMenu * 4 : accéder à la liste des options du menus.
 *
 * mainwindow also contains other general methods.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QLineEdit>
#include "userinformations.h"
#include "sortTable/tablebox.h"
#include "sortTable/fieldviewtable.h"
#include "json/jsonlinker.h"
#include "data/settings.h"

class ExamPreview;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void showWelcomeView();
  void showCreateProjectView();
  void showEvaluationView();
  void openProject();
  void openFileExplorer(QLineEdit *lineEdit, const QString &labelText);
  void checkInputs(QLineEdit *lineEdit1, QLineEdit *lineEdit2,
                   QLineEdit *lineEdit3, QPushButton *pushButton);
  QHBoxLayout *createFileEntry(const QString &labelText, QLineEdit *lineEdit);
  void saveData();
  void loadData(const QString &filePath);
  void handleUndo();
  void handleRedo();
  void toggleDarkMode();
  void openUserManual();

private:
  void createMenuBar();
  void createMainStack();
  void createWelcomeView();
  void createCreateProjectView();
  void createEvaluationView();

  QLabel *currentViewTitle;
  QMenu *fileMenu;
  QMenu *editMenu;
  QMenu *viewMenu;
  QMenu *helpMenu;
  QStackedWidget *mainStack;

  QString saveFilePath;
  QStringList scanFilePaths;
  QStringList jsonFilePaths;

  QWidget *welcomeView;
  QWidget *createProjectView;
  QWidget *evaluationView;

  JsonLinker jsonLinker;

  Settings settings;
};

#endif // MAINWINDOW_H
