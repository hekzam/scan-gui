#ifndef SORTTABLE_H
#define SORTTABLE_H

#include <QTableWidget>
#include <QList>
#include <QFile>
#include <QHeaderView>
#include "progresscell.h"
#include "../json/jsonlinker.h"

/* Layout of the sortTable module
 *
 *  * Classe abtraite héritant de QTableWidget et contenant plusieurs colonnes représentant différentes métriques
 *    chaque ligne est remplie par les informations données par la map associant des noms
 *    de sujet avec les sujets correspondant.
 *
 *    Deux sous classes associées :
 *      - FieldViewTable : Chaque ligne possède sujet, copie, page, champ et syntaxe.
 *      - GroupViewTable : Les champs sont groupés par pages, les pages sont groupées par copies
 *                         et les copies sont groupées par sujets. Les syntaxes sont associées aux copies.
 *
 *    Chaque cellule du tableau possède une data qui représente des données en fonction de la colonne de
 *    la cellule (ex : cellule de COL_COPY aura data copy)
 */

class SortTable : public QTableWidget
{
  Q_OBJECT
public:
  enum Columns
  {
    COL_EXAM = 0,
    COL_COPY,
    COL_PAGE,
    COL_FIELD,
    COL_SYNTAX,
    COL_MET1,
    COL_MET2,
    NB_COL
  };

  SortTable(std::map<QString, ExamInfo> &exams, QWidget *parent = nullptr);
  virtual void initSortTable() = 0;

  // méthode d'accès et d'ajout à la liste d'erreurs de linkage
  virtual QStringList getErrors();

private:
  QStringList headerList;

protected:
  std::map<QString, ExamInfo> &examMap;
  virtual void addErrors(QString &newErrors);

  // liste des erreurs de linkage
  QStringList errors;

public slots:
  void editColumn(int checkState, int column);
  QStringList getHeaderList();
};

#endif // SORTTABLE_H
