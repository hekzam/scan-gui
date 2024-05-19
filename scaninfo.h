#include <QString>
#include <QMap>
#include "json/jsonreader.h"

struct FieldInfo{
    QString m_fieldName;
    QString m_checked;
    QString m_value;
    QString m_syntax;

    FieldInfo(){};
    FieldInfo(QString const& fieldName) : m_fieldName(fieldName){};
};

struct PageInfo{
    QString m_pageName;
    QString m_filePath;
    int m_inJSON;
    QMap<QString,FieldInfo> m_pageFieldMap;

    PageInfo(){};
    PageInfo(QString const& pageName, int inJSON) : m_pageName(pageName), m_inJSON(inJSON){};

    void addField(QString const& fieldName){
        m_pageFieldMap[fieldName] = FieldInfo(fieldName);
    }

    void setPath(QString const& path){
        m_filePath = path;
    }
};

struct CopyInfo{
    QString m_copyName;
    mJSON::dataCopieJSON *m_data = nullptr;
    QMap<QString,PageInfo> m_copyPageMap;

    CopyInfo(){};
    CopyInfo(QString const& copyName, mJSON::dataCopieJSON *data = nullptr) : m_copyName(copyName), m_data(data){};

    void addPage(QString const& pageName, int inJSON){
        if(!m_copyPageMap.contains(pageName))
            m_copyPageMap[pageName] = PageInfo(pageName,inJSON);
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
        return m_copyPageMap.contains(pageName);
    }
};
