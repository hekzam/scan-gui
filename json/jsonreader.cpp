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

int jsonreader::loadFromJSON(QString filename)
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
  coordinates coo = coordinates();
  // pour récupérer le nom de l'objet duquel on extrait les données
  QStringList jsonKeys = jsonObj->keys();
  for (auto &clef : jsonKeys)
  {
    coo.clef = clef;
    v = jsonObj->value(clef);
    o = v.toObject();
    parseValues(o, coo);
  }
  // NE PAS EFFACER, autre manière de récupérer les valeurs sans la clef
  //  QJsonObject::Iterator iter;
  //  for (auto iter = jsonObj->constBegin(); iter != jsonObj->constEnd();
  //  ++iter)
  //  {
  //    if (iter->isObject())
  //    {
  //      o = iter->toObject();
  //      parseValues(o, coo);
  //    }
  //  }
  for (auto &stuff : *a->Array)
  {
    qDebug() << stuff.clef << stuff.x << stuff.y << stuff.h << stuff.w;
  }
}

void jsonreader::parseValues(QJsonObject &o, coordinates &coo)
{
  QStringList cKeys = (QStringList() << "x"
                                     << "y"
                                     << "height"
                                     << "width");
  if (const QJsonValue v = o[cKeys.at(0)]; v.isDouble())
  {
    coo.x = round(v.toDouble());
  }
  if (const QJsonValue v = o[cKeys.at(1)]; v.isDouble())
  {
    coo.y = round(v.toDouble());
  }
  if (const QJsonValue v = o[cKeys.at(2)]; v.isDouble())
  {
    coo.h = round(v.toDouble());
  }
  if (const QJsonValue v = o[cKeys.at(3)]; v.isDouble())
  {
    coo.w = round(v.toDouble());
  }
  a->addCoordinates(coo);
}
