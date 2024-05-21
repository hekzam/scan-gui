#include <QString>
#include <QMap>
#include "json/jsonreader.h"

struct FieldInfo{
    QString m_fieldName;
    QString m_checked;
    QString m_value;
    int m_syntax;

    FieldInfo(){};
    FieldInfo(QString const& fieldName) : m_fieldName(fieldName){};
};

struct PageInfo{
    QString m_pageName;
    QString m_filePath;
    int m_inJSON;
    std::map<QString,FieldInfo> m_pageFieldMap;
    int m_pageSpan = 0;

    PageInfo(){};
    PageInfo(QString const& pageName, int inJSON) : m_pageName(pageName), m_inJSON(inJSON){};

    void addField(QString const& fieldName){
        m_pageFieldMap[fieldName] = FieldInfo(fieldName);
        m_pageSpan++;
    }

    void setPath(QString const& path){
        m_filePath = path;
    }
};

struct CopyInfo{
    QString m_copyName;
    mJSON::dataCopieJSON *m_data = nullptr;
    std::map<QString,PageInfo> m_copyPageMap;
    int m_copySpan = 0;

    CopyInfo(){};
    CopyInfo(QString& copyName, mJSON::dataCopieJSON *data = nullptr) : m_copyName(copyName), m_data(data){};

    void addPage(QString const& pageName, int inJSON){
        if(m_copyPageMap.find(pageName) == m_copyPageMap.end())
            m_copyPageMap[pageName] = PageInfo(pageName,inJSON);
        m_copySpan++;
    }

    void addFieldToPage(QString const& pageName, QString const& fieldName){
        FieldInfo field(fieldName);
        addPage(pageName,1);
        m_copyPageMap[pageName].addField(fieldName);
    }

    void setPagePath(QString const& pageName, QString const& path){
        m_copyPageMap[pageName].setPath(path);
    }

    bool containsPage(QString const& pageName){
        return m_copyPageMap.find(pageName) != m_copyPageMap.end();
    }

};

Q_DECLARE_METATYPE(CopyInfo*)
