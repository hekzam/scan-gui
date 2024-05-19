#ifndef JSONLINKER_H
#define JSONLINKER_H
#include <QString>
#include <QStringList>
#include <QDebug>
#include "jsonreader.h"
#include "../scaninfo.h"

using namespace mJSON;

class JsonLinker
{
    public:

        JsonLinker();
        QMap<QString, CopyInfo> const& collectFields(QStringList const& filePaths, QStringList const& jsonFilePaths);
    private:
        QMap<QString,CopyInfo> fileCopyMap;
        jsonreader jsonReader;


        void initialiseMaps(QStringList const& jsonPath);
        dataCopieJSON *loadAndGetJsonCoords(QString const& jsonPath);


};

#endif // JSONLINKER_H
