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
  int x, y, h, w;
  QString clef;
  int pagenum;

  coordinates()
  {
    x = y = h = w = 0;
    pagenum = 1;
  };
  ~coordinates(){};

  coordinates(double xx, double yy, double hh, double ww)
  {
    x = round(xx);
    y = round(yy);
    h = round(hh);
    w = round(ww);
  }
};

struct dataCopieJSON
{
  struct pageSize
  {
    int numpage;
    QSize pS;
  };

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

  void addCoordinates(coordinates &c)
  {
    documentFields->append(c);
  }

  void addMarker(coordinates &m)
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
  void getCoordinates();
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
