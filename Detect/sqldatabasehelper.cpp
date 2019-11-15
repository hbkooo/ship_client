#include "sqldatabasehelper.h"

SQLDatabaseHelper::SQLDatabaseHelper()
{
    create_sql = "create table object_info (id int primary key, img_name varchar(30), "
                         "islabeled bool, label int, original_img varchar(30), attr varchar(500))";
    InitDB();
}

void SQLDatabaseHelper::InitDB()
{
    qDebug() << "in initDB() ...";
    QMutexLocker lock(&m_lockdb);
    if(QSqlDatabase::contains("mysqlit_con")) {
        m_database = QSqlDatabase::database("mysqlit_con");
    } else {
        m_database = QSqlDatabase::addDatabase("QSQLITE","mysqlit_con");
        m_database.setDatabaseName("obj_database.db");
        m_database.setUserName("root");
        m_database.setPassword("123456");
        if(!OpenDB())
            return;
        QSqlQuery sql_query(m_database);
        QString isExist_table = "select count(*) from sqlite_master where type='table' and name='object_info'";
        if(!sql_query.exec(isExist_table)) {
            qDebug()<<sql_query.lastError();
        } else {
            if(sql_query.next()) {
                int num = sql_query.value(0).toInt();
                if (num == 0) {
                    if(!sql_query.exec(create_sql)) {
                        qDebug()<<sql_query.lastError();
                    } else {
                        qDebug()<<"table created!";
                    }
                } else {
                    qDebug() << "object_info table has exist ...";
                }
            }
        }
    }
    CloseDB();
    lock.unlock();

}

// 获取最大的id
int SQLDatabaseHelper::getMaxID()
{
    QMutexLocker lock(&m_lockdb);
    if (!OpenDB())
        return 0;

    int max_id = 0;

    QSqlQuery sql_query(m_database);
    QString select_max_id = "select max(id) from object_info";
    sql_query.prepare(select_max_id);

    if (!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    } else {
        while(sql_query.next()) {
            max_id = sql_query.value(0).toInt();
            //qDebug()<<QString("max id:%1").arg(max_id);
        }
    }
    CloseDB();
    lock.unlock();
    return max_id;
}

bool SQLDatabaseHelper::insertObjInfo(img_info info)
{
    QMutexLocker lock(&m_lockdb);
    if (!OpenDB())
        return false;
    
    bool flag = false;
    QSqlQuery sql_query(m_database);

    QString insert_sql = "insert into object_info (id, img_name,islabeled, label, original_img, attr)"
                         " values (:id, :img_name, :islabeled, :label, :original_img, :attr) ";
    sql_query.prepare(insert_sql);

    sql_query.bindValue(":id", info.id);
    sql_query.bindValue(":img_name", info.img_name);
    sql_query.bindValue(":islabeled", info.islabeled);
    sql_query.bindValue(":label", info.label);
    sql_query.bindValue(":original_img", info.original_img);
    sql_query.bindValue(":attr", info.attr);

    if (!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    } else {
        flag = true;
        //qDebug() << "insert success ...";
    }
    CloseDB();
    return flag;

}

// 更新属性值
bool SQLDatabaseHelper::updateObjInfo(QString attr, int id)
{

    QMutexLocker lock(&m_lockdb);
    if (!OpenDB())
        return false;
    bool flag = false;
    QSqlQuery sql_query(m_database);
    bool islabeled = true;
    QString update_sql = "update object_info set islabeled = :islabeled ,attr = :attr where id = :id";
    
    //qDebug() << "attr : " << attr;
    
    if(attr == "" || attr == "{}") {
        islabeled = false;
    } 
    
    sql_query.prepare(update_sql);

    sql_query.bindValue(":islabeled",islabeled);
    sql_query.bindValue(":attr",attr);
    sql_query.bindValue(":id",id);

    if (!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    } else {
        flag = true;
        qDebug() << "update success ...";
    }
    CloseDB();
    return flag;

}

int SQLDatabaseHelper::findObjByImgName(QString img_name)
{
    int number = 0;
    QList<img_info> infos;
    QMutexLocker lock(&m_lockdb);
    if (!OpenDB())
        return number;

    QSqlQuery sql_query(m_database);
    QString select_max_id = "select count(*) from object_info where img_name = :img_name";
    sql_query.prepare(select_max_id);
    sql_query.bindValue(":img_name",img_name);

    if (!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    } else {
        if(sql_query.next()) {
            number = sql_query.value(0).toInt();
            //qDebug() << "find " << img_name << " number : " << number;
        }
    }
    CloseDB();
    return number;

}

QList<img_info> SQLDatabaseHelper::getAllInfos()
{
    QList<img_info> infos;
    QMutexLocker lock(&m_lockdb);
    if (!OpenDB())
        return infos;

    QSqlQuery sql_query(m_database);
    QString select_max_id = "select * from object_info ORDER BY id DESC";  // ASC为升序，DESC为降序
    sql_query.prepare(select_max_id);

    if (!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    } else {
        while(sql_query.next()) {
            img_info info;
            info.id = sql_query.value(0).toInt();
            info.img_name = sql_query.value(1).toString();
            info.islabeled = sql_query.value(2).toBool();
            info.label = sql_query.value(3).toInt();
            info.original_img = sql_query.value(4).toString();
            info.attr = sql_query.value(5).toString();
            infos.append(info);
        }
    }
    CloseDB();
    return infos;


}

QList<img_info> SQLDatabaseHelper::getInfosByTag(bool isLabel)
{
    QList<img_info> infos;
    QMutexLocker lock(&m_lockdb);
    if (!OpenDB())
        return infos;

    QSqlQuery sql_query(m_database);
    QString select_max_id = "select * from object_info where islabeled = :islabeled ORDER BY id DESC";  // ASC为升序，DESC为降序
    sql_query.prepare(select_max_id);
    sql_query.bindValue(":islabeled",isLabel);

    if (!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    } else {
        while(sql_query.next()) {
            img_info info;
            info.id = sql_query.value(0).toInt();
            info.img_name = sql_query.value(1).toString();
            info.islabeled = sql_query.value(2).toBool();
            info.label = sql_query.value(3).toInt();
            info.original_img = sql_query.value(4).toString();
            info.attr = sql_query.value(5).toString();
            infos.append(info);
        }
    }
    CloseDB();
    return infos;
}

QList<img_info> SQLDatabaseHelper::getInfosByTags(int label, bool isLabel) {
    QList<img_info> infos;
    QMutexLocker lock(&m_lockdb);
    if (!OpenDB())
        return infos;
    QSqlQuery sql_query(m_database);
    QString select_max_id = "select * from object_info where label = :label and islabeled = :islabeled ORDER BY id DESC";  // ASC为升序，DESC为降序
    sql_query.prepare(select_max_id);
    sql_query.bindValue(":label",label);
    sql_query.bindValue(":islabeled",isLabel);

    if (!sql_query.exec()) {
        qDebug() << "hbk ===> " << sql_query.lastError();
    } else {
        while(sql_query.next()) {
            img_info info;
            info.id = sql_query.value(0).toInt();
            info.img_name = sql_query.value(1).toString();
            info.islabeled = sql_query.value(2).toBool();
            info.label = sql_query.value(3).toInt();
            info.original_img = sql_query.value(4).toString();
            info.attr = sql_query.value(5).toString();
            infos.append(info);
        }
    }
    CloseDB();
    return infos;
}

bool SQLDatabaseHelper::OpenDB()
{
    return m_database.open();
}

void SQLDatabaseHelper::CloseDB()
{
    m_database.close();
}


