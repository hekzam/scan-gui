#include "jsonlinker.h"

JsonLinker::JsonLinker() {

}

std::vector<JsonLinker::infoPage> JsonLinker::linkFileToJson(QStringList const& filePaths, QStringList const& jsonFilePaths){
    std::vector<infoPage> pathFileJson;

    std::unordered_map<QString,QString> jsonMap;

    for(QString const& jsonPath : jsonFilePaths){
        QString jsonName = jsonPath.section("/",-1);
        QString jsonIdentifier = jsonName.section("-",0,2);
        jsonMap[jsonIdentifier] = jsonPath;
    }

    for (QString const& filePath : filePaths){
        QString fileName = filePath.section("/",-1);
        QString fileIdentifier = fileName.section("-",0,2);
        std::unordered_map<QString,QString>::iterator it = jsonMap.find(fileIdentifier);
        if (it != jsonMap.end()){
            infoPage page;
            page.filePath = filePath;
            page.jsonPath = it->second;
            page.fileName = fileName;
            pathFileJson.push_back(page);
        }
    }
    return pathFileJson;
}
