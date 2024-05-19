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
        QString copyName = jsonNameAndExtension.section("-",0,2); //(ex : 1-0-0)
        //We store the data structure in a map in order to give the informations to the preview later
        dataCopieJSON *data(loadAndGetJsonCoords(jsonPath));
        if(!fileCopyMap.contains(copyName))
            fileCopyMap[copyName] = CopyInfo(copyName,data);
        //For each field we search for the corresponding file name in the map
        //We store the fields even though we already store the data in order to display them on the table according to the file they belong to
        for (coordinates const& coordinate : *data->documentFields){
            QString const pageName = "page" + QString::number(coordinate.pagenum); // (ex : page1)
            fileCopyMap[copyName].addFieldToPage(pageName,coordinate.clef);
            qDebug() << "Page in JSON : " << copyName+"-"+pageName;
        }
    }
}

QMap<QString,CopyInfo> const& JsonLinker::collectFields(QStringList const& filePaths, QStringList const& jsonPaths){
    initialiseMaps(jsonPaths);
    //For each file, we collect the fields and add them to the field list
    for (QString const& filePath : filePaths){
        QString fileNameAndExtension = filePath.section("/",-1); //(ex : 1-0-0-page1.png)
        QString fileName = fileNameAndExtension.split(".")[0]; //(ex : 1-0-0-page1)
        QString copyName = fileNameAndExtension.section("-",0,2); //(ex : 1-0-0)
        QString pageName = fileName.section("-",3); //(ex : page1)

        if (!fileCopyMap.contains(copyName))
            //If a copy was not added during the json loading we add it in order to collect the pages that will not be associated to any JSON
            fileCopyMap[copyName] = CopyInfo(copyName);

        if (!fileCopyMap[copyName].containsPage(pageName))
            //If a copy does not contain a selected page, then we add it as "not part of JSON"
            fileCopyMap[copyName].addPage(pageName,0);

        //We then proceed to set the page path
        fileCopyMap[copyName].setPagePath(pageName,filePath);
    }
    return fileCopyMap;
}
