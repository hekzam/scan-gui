#include "jsonlinker.h"

JsonLinker::JsonLinker(){}

dataCopieJSON *JsonLinker::loadAndGetJsonCoords(QString const& jsonPath){
    //Load json and collect fields' coordinates
    jsonReader.loadFromJSON(jsonPath);
    int coordIndex = jsonReader.getCoordinates();
    return jsonReader.listeCopies->at(coordIndex);
}


void JsonLinker::initialiseMaps(QStringList const& jsonPaths){
    //For each json file, we load and collect the coordinates and create the fields that we will map to the corresponding file name
    for(QString const& jsonPath : jsonPaths){
        QString jsonNameAndExtension = jsonPath.section("/",-1); //(ex : 1-0-0-json.json)
        QString copyIdentifier = jsonNameAndExtension.section("-",0,2); //(ex : 1-0-0)
        //We store the data structure in a map in order to give the informations to the preview later
        dataCopieJSON *data(loadAndGetJsonCoords(jsonPath));
        fileDataMap[copyIdentifier] = data;
        //For each field we search for the corresponding file name in the map
        //We store the fields even though we already store the data in order to display them on the table according to the file they belong to
        for (coordinates const& coordinate : *data->documentFields){
            QString fileName(copyIdentifier);
            fileName.append("-page"); //(ex : 1-0-0-page)
            int numPage = coordinate.pagenum;
            fileName.append(QString::number(numPage)); //(ex : 1-0-0-page1)
            fieldInfo field(coordinate.clef,copyIdentifier);
            fileNameCoordMap[fileName].push_back(field);
        }
    }
}

QList<JsonLinker::fieldInfo> const& JsonLinker::collectFields(QStringList const& filePaths, QStringList const& jsonPaths){
    initialiseMaps(jsonPaths);
    //For each file, we collect the fields and add them to the field list
    for (QString const& filePath : filePaths){
        QString fileNameAndExtension = filePath.section("/",-1); //(ex : 1-0-0-page1.png)
        QString fileName = fileNameAndExtension.split(".")[0]; //(ex : 1-0-0-page1)
        QString pageName = fileName.section("-",3);
        if (fileNameCoordMap.contains(fileName)){
            for (fieldInfo& field : fileNameCoordMap[fileName]){
                field.m_pageName = pageName;
                field.m_filePath = filePath;
                fieldList.push_back(field);
            }
            //Remove from the map once treated
            fileNameCoordMap.remove(fileName);
        }
        else
            //If a file is not amongst the keys of fileNameCoordMap it means that is has no corresponding json
            qDebug() << "File : " << fileName << "has no correspondig json.";
    }
    //If a jsonIdentifier key remains in the map, it means that the files related to a json were not selected
    for (auto i = fileNameCoordMap.cbegin(), end = fileNameCoordMap.cend(); i != end; ++i)
        qDebug() << "File : " << qPrintable(i.key()) << "was not selected.";

    return fieldList;
}

QMap<QString, dataCopieJSON*> const& JsonLinker::getFileDataMap(){
    return fileDataMap;
}
