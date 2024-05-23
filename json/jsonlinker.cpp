#include "jsonlinker.h"

JsonLinker::JsonLinker() {}

dataCopieJSON *JsonLinker::loadAndGetJsonCoords(QString const &jsonPath)
{
  // Load json and collect fields' coordinates
  jsonReader.loadFromJSON(jsonPath);
  int coordIndex = jsonReader.getCoordinates();
  return jsonReader.listeCopies->at(coordIndex);
}

void JsonLinker::initialiseMaps(QStringList const &jsonPaths)
{
  // For each json file, we load and collect the coordinates and create the
  // fields that we will map to the corresponding file name
  for (QString const &jsonPath : jsonPaths)
  {
    QString jsonNameAndExtension =
        jsonPath.section("/", -1); //(ex : 1-0-0-json.json)
    QString subjectName =
        jsonNameAndExtension.section("-", 0, 2); //(ex : 1-0-0)
    // We store the data structure in a map in order to give the informations to
    // the preview later
    dataCopieJSON *data(loadAndGetJsonCoords(jsonPath));
    if (fileSubjectMap.find(subjectName) == fileSubjectMap.end())
      fileSubjectMap[subjectName] = SubjectInfo(subjectName, data);

    SubjectInfo &subject = fileSubjectMap[subjectName];
    int numCopies = subjectName.front().digitValue();
    for (int i = 1; i < numCopies + 1; i++)
    {
      QString const copyName = "copie" + QString::number(i);
      subject.addCopy(copyName, 1);
      for (coordinates const &coordinate : *data->documentFields)
      {
        QString const pageName =
            "page" + QString::number(coordinate.pagenum); // (ex : page1)
        subject.addPageToCopy(copyName, pageName, 1);
        subject.addFieldToCopyPage(copyName, pageName, coordinate.clef);
      }
    }
  }
}

std::map<QString, SubjectInfo> &
JsonLinker::collectFields(QStringList const &filePaths,
                          QStringList const &jsonPaths)
{
  initialiseMaps(jsonPaths);
  // For each file, we collect the fields and add them to the field list
  for (QString const &filePath : filePaths)
  {
    QString fileNameAndExtension =
        filePath.section("/", -1); //(ex : 1-0-0-page1.png)
    QString fileName = fileNameAndExtension.split(".")[0]; //(ex : 1-0-0-page1)
    QString subjectName =
        fileNameAndExtension.section("-", 0, 2);               //(ex : 1-0-0)
    QString copyName = fileName.section("-", 3).split("-")[0]; //(ex : copie1)
    QString pageName = fileName.section("-", 3).split("-")[1]; //(ex : page1)

    if (fileSubjectMap.find(subjectName) == fileSubjectMap.end())
      // If a copy was not added during the json loading we add it in order to
      // collect the pages that will not be associated to any JSON
      fileSubjectMap[subjectName] = SubjectInfo(subjectName);
    SubjectInfo &subject = fileSubjectMap[subjectName];
    if (!subject.containsCopy(copyName))
      subject.addCopy(copyName, 0);
    if (!subject.copyContainsPage(copyName, pageName))
      subject.addPageToCopy(copyName, pageName, 0);

    // We then proceed to set the page path
    subject.setPagePath(copyName, pageName, filePath);
    subject.setCopyInFiles(copyName, 1);
    subject.setCopyPageInFiles(copyName, pageName, 1);
  }
  return fileSubjectMap;
}
