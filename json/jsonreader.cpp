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
jsonreader::jsonreader()
{
  jsonDoc = new QJsonDocument;
  jsonObj = new QJsonObject;
  listeCopies = new QList<dataCopieJSON *>;
}

jsonreader::~jsonreader()
{
  delete jsonDoc;
  delete jsonObj;
  // ATTENTION LA STRUCTURE EST DETRUITE ICI, on perd les données
  for (auto &c : *listeCopies)
  {
    delete c;
  }
  delete listeCopies;
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

int jsonreader::getCoordinates()
{
  nouvelleCopie = new dataCopieJSON;
  QJsonObject o;
  dataFieldJSON coo = dataFieldJSON();
  // pour récupérer le nom de l'objet duquel on extrait les données
  QStringList jsonKeys = jsonObj->keys();
  // TODO : give user the possibility to change the name of the marker id
  // TODO : what to do if some keys are missing (aside from crying)
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

  // here the page is assumed to always be standard A4
  // every page is considered to be the same size
  QSize A4 = QSize(210, 297);
  nouvelleCopie->pageSizeInMM = A4;

  listeCopies->append(nouvelleCopie);

  // UNCOMMENT FOR DEBUG INFO
  // for (auto &c : *a->documentFields)
  // {
  //   qDebug() << c.clef << c.x << c.y << c.h << c.w << c.pagenum;
  // }
  // qDebug() << "markers";
  // for (auto &c : *a->documentMarkers)
  // {
  //   qDebug() << c.clef << c.x << c.y << c.h << c.w << c.pagenum;
  // }
  // for (auto &s : *a->documentSizes)
  // {
  //   qDebug() << s.numpage << s.pS;
  // }
  return listeCopies->indexOf(nouvelleCopie); // -1 on error (defined by Qt)
}

void jsonreader::parseValues(QJsonObject &o, dataFieldJSON &coo)
{
  QStringList cKeys = (QStringList() << "x"
                                     << "y"
                                     << "height"
                                     << "width"
                                     << "page");
  if (const QJsonValue v = o[cKeys.at(0)]; v.isDouble())
  {
    coo.x = v.toDouble();
  }
  if (const QJsonValue v = o[cKeys.at(1)]; v.isDouble())
  {
    coo.y = v.toDouble();
  }
  if (const QJsonValue v = o[cKeys.at(2)]; v.isDouble())
  {
    coo.h = v.toDouble();
  }
  if (const QJsonValue v = o[cKeys.at(3)]; v.isDouble())
  {
    coo.w = v.toDouble();
  }
  if (const QJsonValue v = o[cKeys.at(4)]; v.isDouble())
  {
    coo.pagenum = qFloor(v.toDouble());
  }
}

void jsonreader::identifyFields(QJsonObject &o, dataFieldJSON &coo)
{
  parseValues(o, coo);
  nouvelleCopie->addCoordinates(coo);
}

void jsonreader::identifyMarkers(QJsonObject &o, dataFieldJSON &coo)
{
  parseValues(o, coo);
  nouvelleCopie->addMarker(coo);
}
