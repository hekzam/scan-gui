#ifndef JSONREADER_H
#define JSONREADER_H

#include <QJsonObject>

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
  QList<coordinates> *Array;

  ~coordArray()
  {
    delete Array;
  }

  void addCoordinates(coordinates &c)
  {
    Array->append(c);
  }

  coordArray()
  {
    Array = new QList<coordinates>;
  }
};

class jsonreader
{
  // Q_OBJECT
public:
  jsonreader();
  ~jsonreader();

  int loadFromJSON(QString filename);
  void getCoordinates();
  // QString jsonFilename;
  coordArray *a;

private:
  void parseValues(QJsonObject &, coordinates &);
  QJsonObject *jsonObj;
  QJsonDocument *jsonDoc;
  QJsonObject o;
  QJsonValue v;
};
} // namespace mJSON
#endif // JSONREADER_H
