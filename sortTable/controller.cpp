#include "controller.h"

//Controller
Controller::Controller() {
    boundaryWindow = new Boundary();
    liaisonControlBoundary();
}

Controller::~Controller(){
    delete boundaryWindow;
}

void Controller::liaisonControlBoundary(){
    boundaryWindow->setController(this);
}

void Controller::afficherGestionnaire(){
    QStringList const nomsFichiers = QFileDialog::getOpenFileNames(boundaryWindow, ("Ouvrir des fichiers"), "/Users/marcomartins/My Documents /Licence Informatique L3/Bureau d'Étude/QT/Proto", "*.*");
    boundaryWindow->initTableView(nomsFichiers);
}


