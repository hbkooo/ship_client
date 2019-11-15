#ifndef SQLDATABASEHELPER_H
#define SQLDATABASEHELPER_H

#include <QObject>
#include <QDebug>

#include <QJsonDocument>

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

#include <QMutex>

struct img_info {
    int id;
    QString img_name;  // 相对路径
    bool islabeled;
    int label;
    QString original_img;
    QString attr;
};

class SQLDatabaseHelper
{
public:
    SQLDatabaseHelper();
    void InitDB();

    int getMaxID();
    bool insertObjInfo(img_info info);
    bool updateObjInfo(QString attr,int id); // 更新属性值
    int findObjByImgName(QString img_name);  // 查找该命名的数量
    QList<img_info> getAllInfos();
    QList<img_info> getInfosByTag(bool isLabel);
    QList<img_info> getInfosByTags(int label, bool isLabel);


private:
    bool OpenDB();
    void CloseDB();

private:
    QString create_sql;

    QSqlDatabase m_database;
    QMutex m_lockdb;

};

#endif // SQLDATABASEHELPER_H
