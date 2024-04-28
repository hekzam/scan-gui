#include <QtWidgets>
#include "mainwindow.h"
#include "preview/preview.h"

static const QSize minPanelSize(250, 500);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(1280,720);
    createMenuBar();
    createMainStack();
    createWelcomeView();
    createCreateProjectView();
    createEvaluationView();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenuBar()
{
    QMenuBar *menuBar = this -> menuBar();

    // Menu Déroulant Fichier.

    fileMenu = menuBar -> addMenu(tr("&File"));

    QAction *newFileAction = new QAction(tr("&New File"), this);
    connect(newFileAction, &QAction::triggered, this, &MainWindow::openFileExplorer);
    fileMenu -> addAction(newFileAction);

    QAction *openFileAction = new QAction(tr("&Open File"), this);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFileExplorer);
    fileMenu -> addAction(openFileAction);

    QAction *saveFileAction = new QAction(tr("&Save File"), this);
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::openFileExplorer);
    fileMenu -> addAction(saveFileAction);

    QAction *exportAction = new QAction(tr("&Export"), this);
    connect(exportAction, &QAction::triggered, this, &MainWindow::openFileExplorer);
    fileMenu -> addAction(exportAction);

    // Menu Déroulant Edition.

    editMenu = menuBar -> addMenu(tr("&Edit"));

    QAction *undoAction = new QAction(tr("&Undo"), this);
    connect(undoAction, &QAction::triggered, this, &MainWindow::handleUndo);
    editMenu -> addAction(undoAction);

    QAction *redoAction = new QAction(tr("&Redo"), this);
    connect(redoAction, &QAction::triggered, this, &MainWindow::handleRedo);
    editMenu -> addAction(redoAction);

    // Menu Déroulant Affichage.

    viewMenu = menuBar -> addMenu(tr("&View"));

    QAction *darkModeAction = new QAction(tr("&Dark Mode"), this);
    connect(darkModeAction, &QAction::triggered, this, &MainWindow::toggleDarkMode);
    viewMenu -> addAction(darkModeAction);

    // Menu Déroulant Aide.

    helpMenu = menuBar -> addMenu(tr("&Help"));

    QAction *userManualAction = new QAction(tr("&User Manual"), this);
    connect(userManualAction, &QAction::triggered, this, &MainWindow::openUserManual);
    helpMenu -> addAction(userManualAction);
}

void MainWindow::createMainStack()
{
    mainStack = new QStackedWidget(this);
    setCentralWidget(mainStack);
}

void MainWindow::createWelcomeView()
{
    welcomeView = new QWidget(this);

    QVBoxLayout *welcomeViewLayout = new QVBoxLayout(welcomeView);
    welcomeViewLayout -> setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel("<b>HEKZAM</b>", welcomeView);
    titleLabel -> setAlignment(Qt::AlignCenter);
    titleLabel -> setStyleSheet("font-size: 24px;");

    QLabel *descriptionLabel = new QLabel("Exam evaluation software.", welcomeView);
    descriptionLabel -> setAlignment(Qt::AlignCenter);

    QPushButton *createButton = new QPushButton("Create a new project", welcomeView);
    createButton -> setStyleSheet("font-size: 16px; background-color: green; color: white;");
    createButton -> setFixedSize(250, 50);
    connect(createButton, &QPushButton::clicked, this, &MainWindow::showCreateProjectView);

    QPushButton *openButton = new QPushButton("Open an existing project", welcomeView);
    openButton -> setStyleSheet("font-size: 16px; background-color: blue; color: white;");
    openButton -> setFixedSize(250, 50);
    connect(openButton, &QPushButton::clicked, this, &MainWindow::openFileExplorer);

    welcomeViewLayout -> addWidget(titleLabel);
    welcomeViewLayout -> addWidget(descriptionLabel);
    welcomeViewLayout -> addWidget(createButton);
    welcomeViewLayout -> addWidget(openButton);

    mainStack -> addWidget(welcomeView);
}

void MainWindow::createCreateProjectView()
{
    createProjectView = new QWidget(this);

    QVBoxLayout *createProjectViewLayout = new QVBoxLayout(createProjectView);
    createProjectViewLayout -> setAlignment(Qt::AlignCenter);
    createProjectViewLayout -> setContentsMargins(50, 50, 50, 50);

    QFormLayout *fileForm = new QFormLayout();
    fileForm->setSpacing(20);
    fileForm -> addRow(new QLabel("To continue, please fill in the following fields.", this));

    QLineEdit *repositoryImport = new QLineEdit;
    fileForm -> addRow(createFileEntry("Project Repository", repositoryImport));

    QLineEdit *examImport = new QLineEdit;
    fileForm -> addRow(createFileEntry("Exam data", examImport));

    QLineEdit *scanImport = new QLineEdit;
    fileForm -> addRow(createFileEntry("Scan file(s)", scanImport));

    fileForm -> addRow(new QLabel("If you desire, you can also fill in an attendance list.", this));

    QLineEdit *listImport = new QLineEdit;
    fileForm -> addRow(createFileEntry("Attendance list", listImport));

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout -> setAlignment(Qt::AlignRight);

    QPushButton *nextButton = new QPushButton("Next");
    nextButton -> setStyleSheet("font-size: 16px; background-color: green; color: white;");
    nextButton -> setFixedSize(250, 50);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::showEvaluationView);

    QPushButton *backButton = new QPushButton("Back");
    backButton -> setStyleSheet("font-size: 16px; background-color: orange; color: white;");
    backButton -> setFixedSize(250, 50);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showWelcomeView);

    buttonLayout -> addWidget(backButton);
    buttonLayout -> addWidget(nextButton);

    fileForm -> setHorizontalSpacing(100);

    createProjectViewLayout -> addLayout(fileForm);
    createProjectViewLayout -> addSpacing(50);
    createProjectViewLayout -> addLayout(buttonLayout);

    mainStack -> addWidget(createProjectView);
}

void MainWindow::createEvaluationView()
{
    evaluationView = new QWidget(this);

    // Premier split.

    QHBoxLayout *evaluationViewLayout = new QHBoxLayout(evaluationView);
    QSplitter *verticalSplitter = new QSplitter(evaluationView);
    QSplitter *horizontalSplitter = new QSplitter(evaluationView);

    ExamPreview *previewBox = new ExamPreview(evaluationView);

    evaluationViewLayout -> addWidget(horizontalSplitter);
    horizontalSplitter -> insertWidget(0, verticalSplitter);
    horizontalSplitter -> insertWidget(1, previewBox);
    verticalSplitter -> setOrientation(Qt::Vertical);
    verticalSplitter -> setSizes(QList<int>() << 800 << 200);

    // Deuxième split / partie haute.

    // TODO : add saveState() on this splitter
    QStringList fileNames = QFileDialog::getOpenFileNames(this, ("Open files"), "/Users/marcomartins/My Documents /Licence Informatique L3/Bureau d'Étude/QT/Proto", "*.*");
    TableBox *tableBox = new TableBox(fileNames, this, evaluationView);

    verticalSplitter -> addWidget(tableBox);

    // Partie basse.
    QGroupBox *informationBox = new QGroupBox("General information", evaluationView);
    QVBoxLayout *informationLayout = new QVBoxLayout(informationBox);

    verticalSplitter -> addWidget(informationBox);
    informationLayout -> addWidget(new QLabel("WIP Information",this));

    mainStack->addWidget(evaluationView);
}

void MainWindow::showWelcomeView()
{
    mainStack -> setCurrentWidget(welcomeView);
}

void MainWindow::showCreateProjectView()
{
    mainStack -> setCurrentWidget(createProjectView);
}

void MainWindow::showEvaluationView()
{
    mainStack -> setCurrentWidget(evaluationView);
}

void MainWindow::openFileExplorer()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath());
    if (!fileName.isEmpty()) {
        showEvaluationView();
    }
}

void MainWindow::openFileExplorerAlt(QLineEdit *file)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath());
    if (!fileName.isEmpty()) {
        file -> setText(fileName);
    }
}

QHBoxLayout *MainWindow::createFileEntry(const QString &labelText, QLineEdit *lineEdit)
{
    QHBoxLayout *layout = new QHBoxLayout;
    QLabel *label = new QLabel(labelText + " :", this);
    QPushButton *browseButton = new QPushButton("Search...", this);

    layout -> addWidget(label);
    layout -> addWidget(lineEdit);
    layout -> addWidget(browseButton);

    // Définir le stretch pour chaque widget
    layout -> setStretch(0, 2);
    layout -> setStretch(1, 6);
    layout -> setStretch(2, 1);

    connect(browseButton, &QPushButton::clicked, this, [this, lineEdit]() {
        openFileExplorerAlt(lineEdit);
    });

    return layout;
}

void MainWindow::handleUndo()
{
}

void MainWindow::handleRedo()
{
}

void MainWindow::toggleDarkMode()
{
}

void MainWindow::openUserManual()
{
}
