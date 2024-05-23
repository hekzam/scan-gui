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
struct coordinates
{
  qreal x, y, h, w;
  QString clef;
  QVariant valeur; // to be used to store the "value" of the field, might be
                   // string or bool
  int pagenum;

  coordinates()
  {
    x = y = h = w = 0;
    pagenum = 1;
  };
  ~coordinates(){};

  coordinates(qreal xx, qreal yy, qreal hh, qreal ww)
  {
    x = /*round*/ (xx);
    y = /*round*/ (yy);
    h = /*round*/ (hh);
    w = /*round*/ (ww);
  }
};

struct dataCopieJSON
{
  // refactor this to have one struct per page ?
  struct pageSize
  {
    int numpage;
    QSize pS;
  };

  // QMap<int,QList<coordinates>
  // enlever les pointeurs ?
  QList<pageSize> *documentSizes;
  QList<coordinates> *documentFields;
  QList<coordinates> *documentMarkers;
  int pagecount = 0;

  ~dataCopieJSON()
  {
    delete documentFields;
    delete documentMarkers;
    delete documentSizes;
  }

  void addCoordinates(const coordinates &c)
  {
    documentFields->append(c);
  }

  void addMarker(const coordinates &m)
  {
    documentMarkers->append(m);
    if (m.pagenum > pagecount)
    {
      pagecount = m.pagenum;
    }
  }

  dataCopieJSON()
  {
    documentFields = new QList<coordinates>;
    documentMarkers = new QList<coordinates>;
    documentSizes = new QList<pageSize>;
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
  dataCopieJSON *a;

private:
  void parseValues(QJsonObject &, coordinates &);
  void identifyFields(QJsonObject &, coordinates &);
  void identifyMarkers(QJsonObject &, coordinates &);
  void calculateDocumentSize();
  QJsonObject *jsonObj;
  QJsonDocument *jsonDoc;
};
} // namespace mJSON
#endif // JSONREADER_H
