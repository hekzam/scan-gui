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
#include "sortTable/tablebox.h"
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
    void checkInputs(QLineEdit *lineEdit1, QLineEdit *lineEdit2, QLineEdit *lineEdit3, QPushButton *pushButton);
    QHBoxLayout *createFileEntry(const QString &labelText, QLineEdit *lineEdit);
    void saveData();
    void loadData(const QString& filePath);
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
    // ExamPreview *previewBox;

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
