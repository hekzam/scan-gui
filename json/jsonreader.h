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

struct coordinates
{
  int x, y, h, w = 0;
  QString clef;

  coordinates(){};
  ~coordinates(){};

  coordinates(double xx, double yy, double hh, double ww)
  {
    x = round(xx);
    y = round(yy);
    h = round(hh);
    w = round(ww);
  }
};

struct coordArray
{
  QSize documentSize;
  QList<coordinates> *documentFields;
  QList<coordinates> *documentMarkers;

  ~coordArray()
  {
    delete documentFields;
    delete documentMarkers;
  }

  void addCoordinates(coordinates &c)
  {
    documentFields->append(c);
  }

  void addMarker(coordinates &m)
  {
    documentMarkers->append(m);
  }

  coordArray()
  {
    documentFields = new QList<coordinates>;
    documentMarkers = new QList<coordinates>;
  }
};

class jsonreader
{
  // Q_OBJECT
public:
  jsonreader();
  ~jsonreader();

  int loadFromJSON(const QString filename);
  void getCoordinates();
  // QString jsonFilename;
  coordArray *a;

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
