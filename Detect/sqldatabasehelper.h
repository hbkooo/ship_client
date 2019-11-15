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
    QString img_name;  // ���·��
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
    bool updateObjInfo(QString attr,int id); // ��������ֵ
    int findObjByImgName(QString img_name);  // ���Ҹ�����������
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
