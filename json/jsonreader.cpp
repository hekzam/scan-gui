#include "jsonreader.h"

#include <QFile>
#include <QJsonDocument>
using namespace mJSON;

/*
 * We could have the Tableview create the data structure (list of coordinates)
 * and pass an index to the preview to get info about the location
 *
 */
// TODO : ERROR HANDLING !!!!!
jsonreader::jsonreader() {}

jsonreader::~jsonreader()
{
  delete jsonDoc;
  delete jsonObj;
  delete a; // ATTENTION LA STRUCTURE EST DETRUITE ICI, on perd les données
}

int jsonreader::loadFromJSON(const QString filename)
{
  // TODO
  // to be done from project root tho, not absolute root, or file dialog
  // maybe with a function getProjectPath avec QDir::setCurrent(dir)?
  QFile jsonFile;
  jsonFile.setFileName(filename);
  if (!jsonFile.exists() || !jsonFile.open(QIODevice::ReadOnly))
  {
    qDebug() << "Impossible d'ouvrir le JSON";
    return (OPENJSONFAIL); // ??
  }
  QByteArray jsonByteArray = jsonFile.readAll();
  if (jsonByteArray.isNull())
  {
    qDebug() << "jsonByteArray is empty";
    return (BYTEARRAYEMPTY);
  }
  jsonFile.close();

  QJsonParseError jsonError;
  jsonDoc = new QJsonDocument;
  *jsonDoc = QJsonDocument::fromJson(jsonByteArray, &jsonError);
  if (jsonError.error != QJsonParseError::NoError)
  {
    qDebug() << "Erreur d'analyse JSON:" << jsonError.errorString();
    return (CONVERTDOCFAIL);
  }
  if (jsonDoc->isEmpty())
  {
    qDebug() << "JSONDoc is empty";
    return (EMPTYJSON);
  }
  if (jsonDoc->isObject())
  {
    jsonObj = new QJsonObject;
    *jsonObj = jsonDoc->object();
  }
  else
  {
    qDebug() << "JSONFile n'est pas un objet";
    qDebug() << "JsonObj is :" << jsonObj;
    return (NOTANOBJECT);
  }
  return ALLCLEAR;
}

void jsonreader::getCoordinates()
{
  a = new coordArray;
  QJsonObject o;
  coordinates coo = coordinates();
  // pour récupérer le nom de l'objet duquel on extrait les données
  QStringList jsonKeys = jsonObj->keys();
  // TODO : give user the possibility to change the name of the marker id
  for (auto &clef : jsonKeys)
  {
    coo.clef = clef;
    o = jsonObj->value(clef).toObject();
    if (clef.contains("marker barcode"))
    {
      identifyMarkers(o, coo);
    }
    else
    {
      identifyFields(o, coo);
    }
  }
  for (auto &stuff : *a->documentFields)
  {
    qDebug() << stuff.clef << stuff.x << stuff.y << stuff.h << stuff.w;
  }
  qDebug() << "markers";
  for (auto &c : *a->documentMarkers)
  {
    qDebug() << c.clef << c.x << c.y << c.h << c.w;
  }
  calculateDocumentSize();
  qDebug() << a->documentSize;
}

void jsonreader::parseValues(QJsonObject &o, coordinates &coo)
{
  QStringList cKeys = (QStringList() << "x"
                                     << "y"
                                     << "height"
                                     << "width");
  if (const QJsonValue v = o[cKeys.at(0)]; v.isDouble())
  {
    coo.x = qFloor(v.toDouble());
  }
  if (const QJsonValue v = o[cKeys.at(1)]; v.isDouble())
  {
    coo.y = qFloor(v.toDouble());
  }
  if (const QJsonValue v = o[cKeys.at(2)]; v.isDouble())
  {
    coo.h = round(v.toDouble());
  }
  if (const QJsonValue v = o[cKeys.at(3)]; v.isDouble())
  {
    coo.w = round(v.toDouble());
  }
}

void jsonreader::identifyFields(QJsonObject &o, coordinates &coo)
{
  parseValues(o, coo);
  a->addCoordinates(coo);
}

void jsonreader::identifyMarkers(QJsonObject &o, coordinates &coo)
{
  parseValues(o, coo);
  a->addMarker(coo);
}

void jsonreader::calculateDocumentSize()
{
  coordinates topleft;
  coordinates bottomright;
  if (!a->documentMarkers->empty())
  {
    for (auto i = a->documentMarkers->cbegin(),
              rend = a->documentMarkers->cend();
         i != rend; ++i)
    {
      if (i->clef.contains("tl"))
      {
        topleft = *i;
      }
      if (i->clef.contains("br"))
      {
        bottomright = *i;
      }
    }
    QSize ds = QSize(topleft.x + bottomright.x, topleft.y + bottomright.y);
    a->documentSize = ds;
  }
  else
  {
    qWarning() << "no markers found for this sheet, marker id should contain "
                  "\"marker barcode\"";
  }
}
