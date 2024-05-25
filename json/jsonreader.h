/*
 * this class reads the json and stores the informations in dataCopieJSON and
 * the data for individual cells is dataFieldJSON To be modified if (when) the
 * data model changes.
 */
#ifndef JSONREADER_H
#define JSONREADER_H

#include <QJsonObject>
#include <QSize>

namespace mJSON
{
enum JSONERROR
{
  ALLCLEAR = 0,
  OPENJSONFAIL = 1,
  BYTEARRAYEMPTY = 2,
  CONVERTDOCFAIL = 3,
  EMPTYJSON = 4,
  NOTANOBJECT = 5

};

// coordonnées des éléments présents dans une copie
// une copie peut comprendre plusieures pages.
struct dataFieldJSON
{
  qreal x, y, h, w;
  QString clef;
  QVariant valeur; // to be used to store the "value" of the field, might be
                   // string or bool
  int pagenum;

  dataFieldJSON()
  {
    x = y = h = w = 0;
    pagenum = 1;
  };

  ~dataFieldJSON(){};

  dataFieldJSON(qreal xx, qreal yy, qreal hh, qreal ww)
  {
    x = (xx);
    y = (yy);
    h = (hh);
    w = (ww);
  }
};

struct dataCopieJSON
{
  // QMap<int,QList<coordinates> ?
  // enlever les pointeurs ?
  QSize pageSizeInMM;
  QList<dataFieldJSON> *documentFields;
  QList<dataFieldJSON> *documentMarkers;
  int pagecount = 0;

  ~dataCopieJSON()
  {
    delete documentFields;
    delete documentMarkers;
  }

  void addCoordinates(const dataFieldJSON &c)
  {
    documentFields->append(c);
  }

  void addMarker(const dataFieldJSON &m)
  {
    documentMarkers->append(m);
    if (m.pagenum > pagecount)
    {
      pagecount = m.pagenum;
    }
  }

  dataCopieJSON()
  {
    documentFields = new QList<dataFieldJSON>;
    documentMarkers = new QList<dataFieldJSON>;
  }
};

class jsonreader
{
  // Q_OBJECT
public:
  jsonreader();
  ~jsonreader();

  // those are two seperate functions because then we can check for the
  // potential error code when loading the json
  int loadFromJSON(const QString filename);
  int getCoordinates();
  QList<dataCopieJSON *> *listeCopies;
  dataCopieJSON *nouvelleCopie;

private:
  void parseValues(QJsonObject &, dataFieldJSON &);
  void identifyFields(QJsonObject &, dataFieldJSON &);
  void identifyMarkers(QJsonObject &, dataFieldJSON &);

  QJsonObject *jsonObj;
  QJsonDocument *jsonDoc;
};
} // namespace mJSON
#endif // JSONREADER_H
