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
    void openFileExplorer();
    void openFileExplorerAlt(QLineEdit *lineEdit);
    QHBoxLayout *createFileEntry(const QString &labelText, QLineEdit *lineEdit);
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
    ExamPreview *previewBox;

    QWidget *welcomeView;
    QWidget *createProjectView;
    QWidget *evaluationView;
};

#endif // MAINWINDOW_H
