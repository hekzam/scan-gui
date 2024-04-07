#include <QtWidgets>
#include "mainwindow.h"
static const QSize minPanelSize(250,500);

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

    fileMenu = menuBar -> addMenu(tr("&Fichier"));

    QAction *newFileAction = new QAction(tr("&Nouveau"), this);
    connect(newFileAction, &QAction::triggered, this, &MainWindow::openFileExplorer);
    fileMenu -> addAction(newFileAction);

    QAction *openFileAction = new QAction(tr("&Ouvrir"), this);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFileExplorer);
    fileMenu -> addAction(openFileAction);

    QAction *saveFileAction = new QAction(tr("&Enregistrer"), this);
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::openFileExplorer);
    fileMenu -> addAction(saveFileAction);

    // Menu Déroulant Edition.

    editMenu = menuBar -> addMenu(tr("&Édition"));

    QAction *undoAction = new QAction(tr("&Annuler"), this);
    connect(undoAction, &QAction::triggered, this, &MainWindow::handleUndo);
    editMenu -> addAction(undoAction);

    QAction *redoAction = new QAction(tr("&Rétablir"), this);
    connect(redoAction, &QAction::triggered, this, &MainWindow::handleRedo);
    editMenu -> addAction(redoAction);

    // Menu Déroulant Affichage.

    viewMenu = menuBar -> addMenu(tr("&Affichage"));

    QAction *darkModeAction = new QAction(tr("&Mode Sombre"), this);
    connect(darkModeAction, &QAction::triggered, this, &MainWindow::toggleDarkMode);
    viewMenu -> addAction(darkModeAction);

    // Menu Déroulant Aide.

    helpMenu = menuBar -> addMenu(tr("&Aide"));

    QAction *userManualAction = new QAction(tr("&Manuel Utilisateur"), this);
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

    QLabel *descriptionLabel = new QLabel("Logiciel d'évaluation d'examens.", welcomeView);
    descriptionLabel -> setAlignment(Qt::AlignCenter);

    QPushButton *createButton = new QPushButton("Créer un nouveau projet", welcomeView);
    createButton -> setStyleSheet("font-size: 16px; background-color: green; color: white;");
    createButton -> setFixedSize(250, 50);
    connect(createButton, &QPushButton::clicked, this, &MainWindow::showCreateProjectView);

    QPushButton *openButton = new QPushButton("Ouvrir un projet existant", welcomeView);
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

    /**QGridLayout *fileGrid = new QGridLayout(createProjectView);
    QHBoxLayout *repositoryLayout = new QHBoxLayout(createProjectView);
    QHBoxLayout *examLayout = new QHBoxLayout(createProjectView);
    QHBoxLayout *scanLayout = new QHBoxLayout(createProjectView);
    QHBoxLayout *listLayout = new QHBoxLayout(createProjectView);

    fileGrid -> addWidget(new QLabel("Pour continuer, veuillez renseigner les champs suivants.", createProjectView), 0, 0);
    fileGrid -> addLayout(repositoryLayout, 1, 0);
    fileGrid -> addLayout(examLayout, 2, 0);
    fileGrid -> addLayout(scanLayout, 3, 0);
    fileGrid -> addWidget(new QLabel("Si vous le souhaitez, vous pouvez également renseigner une liste d'émargement.", createProjectView), 0, 0);
    fileGrid -> addLayout(listLayout, 5, 0);*/

    QPushButton *openFileButton = new QPushButton("Ouvrir", createProjectView);
    openFileButton -> setStyleSheet("font-size: 16px; background-color: orange; color: white;");
    openFileButton -> setFixedSize(100, 50);
    connect(openFileButton, &QPushButton::clicked, this, &MainWindow::showEvaluationView);

    createProjectViewLayout -> addWidget(openFileButton);

    mainStack -> addWidget(createProjectView);
}

void MainWindow::createEvaluationView()
{
    evaluationView = new QWidget(this);

    // Premier split.

    QHBoxLayout *evaluationViewLayout = new QHBoxLayout(evaluationView);
    QSplitter *verticalSplitter = new QSplitter(evaluationView);
    QSplitter *horizontalSplitter = new QSplitter(evaluationView);
    QGroupBox *infoBox = new QGroupBox(evaluationView);

    evaluationViewLayout -> addWidget(verticalSplitter);
    verticalSplitter -> addWidget(horizontalSplitter);
    verticalSplitter -> addWidget(infoBox);
    verticalSplitter -> setOrientation(Qt::Vertical);
    verticalSplitter -> setSizes(QList<int>() << 800 << 200);

    // Deuxième split / partie haute.

    QGroupBox *tableBox = new QGroupBox("Tableau de Tri", evaluationView);
    QGroupBox *previewBox = new QGroupBox("Aperçu", evaluationView);

    horizontalSplitter -> addWidget(tableBox);
    horizontalSplitter -> addWidget(previewBox);

    // Partie basse.

    QHBoxLayout *infoLayout = new QHBoxLayout(infoBox);
    QGroupBox *informationsBox = new QGroupBox("Informations Générales", evaluationView);
    QVBoxLayout *informationsLayout = new QVBoxLayout(informationsBox);
    QGroupBox *generalButtonsBox = new QGroupBox(evaluationView);
    QHBoxLayout *generalButtonsLayout = new QHBoxLayout(generalButtonsBox);

    infoLayout -> addWidget(informationsBox);
    infoLayout -> addWidget(generalButtonsBox);
    informationsLayout -> addWidget(new QLabel("WIP Informations",this));
    generalButtonsLayout -> addWidget(new QLabel("WIP General Buttons",this));
    generalButtonsBox -> setStyleSheet("QGroupBox { border: none; }");

    mainStack -> addWidget(evaluationView);
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
