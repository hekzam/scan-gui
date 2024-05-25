#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <vector>

class Boundary;

#pragma once
#include "boundary.h"

class Controller
{
public:
  Controller();
  ~Controller();
  void liaisonControlBoundary();
  void afficherGestionnaire();

private:
  Boundary *boundaryWindow;
  void initListFichier(QStringList files);
};

#endif // CONTROLLER_H
