#ifndef JSONLINKER_H
#define JSONLINKER_H
#include <QString>
#include <QStringList>
#include <QDebug>
#include "jsonreader.h"
#include "../scaninfo.h"

/* Classe temporaire permettant de regrouper manuellement les fichiers JSON avec les pages
 * en fonction de leurs noms.
*/


using namespace mJSON;

class JsonLinker
{
public:
  JsonLinker();
  std::map<QString, SubjectInfo> &
  collectFields(QStringList const &filePaths, QStringList const &jsonFilePaths);

private:
  std::map<QString, SubjectInfo> fileSubjectMap;
  jsonreader jsonReader;

  void initialiseMaps(QStringList const &jsonPath);
  dataCopieJSON *loadAndGetJsonCoords(QString const &jsonPath);
};

#endif // JSONLINKER_H
