#ifndef JSONLINKER_H
#define JSONLINKER_H
#include <QString>
#include <QStringList>
#include <QDebug>
#include "jsonreader.h"

using namespace mJSON;

class JsonLinker
{
    public:
        struct fieldInfo{
            QString m_filePath;
            QString m_copyName;
            QString m_pageName;
            QString m_fieldName;
            QString m_checked;
            QString m_syntex;


            fieldInfo(){}

            fieldInfo(QString const fieldName, QString const copyName) : m_fieldName(fieldName), m_copyName(copyName){}

        };

        JsonLinker();
        QList<JsonLinker::fieldInfo> const& collectFields(QStringList const& filePaths, QStringList const& jsonFilePaths);
        QMap<QString, dataCopieJSON*> const& getFileDataMap();
    private:
        QList<fieldInfo> fieldList;
        QMap<QString,QList<fieldInfo>> fileNameCoordMap;
        QMap<QString,dataCopieJSON*> fileDataMap;
        jsonreader jsonReader;


        void initialiseMaps(QStringList const& jsonPath);
        dataCopieJSON *loadAndGetJsonCoords(QString const& jsonPath);


};

#endif // JSONLINKER_H
