#ifndef JSONLINKER_H
#define JSONLINKER_H
#include <QString>
#include <QStringList>
#include <QDebug>

class JsonLinker
{
public:
    typedef struct struct_info_page{
        QString filePath;
        QString jsonPath;
        QString fileName;
    } infoPage;

    JsonLinker();
    std::vector<JsonLinker::infoPage> linkFileToJson(QStringList const& filePaths, QStringList const& jsonFilePaths);

};

#endif // JSONLINKER_H
