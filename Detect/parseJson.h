#ifndef PARSE_JSON_H
#define PARSE_JSON_H

#include <fstream>
#include <iostream>
//#include <common/json/json.h>
#include <common/error.h>
#include <string>
#include "../cfg/config.h"

#include <QFile>
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

#include <QPixmap>
#include <QFile>
#include <QMap>

using namespace std;

// format : [ label score x1 y1 x2 y2 x3 y3 x4 y4 ]
std::vector<std::vector<float> > parseFourPoints(QJsonArray results, int result_nums);

// format : [ label score xmin ymin xmax ymax ]
std::vector<std::vector<float> > parseTwoPoints(QJsonArray results, int result_nums);

// �������
std::vector<std::vector<float> > parseResult(char * results, int & response_id, string &error_msg);

// ���� json �����ļ�
bool loadConfig(std::string configPath);

// ����Ĭ�ϵ� json �����ļ�
bool generateDefaultConfig(std::string configPath);

// ��json������ת��Ϊmap��ʽ
QMap<QString,QString> convertJsonToMap(QString attr);

QPixmap loadPixImage(QString path);




#endif