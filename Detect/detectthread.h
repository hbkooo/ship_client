#ifndef DETECTTHREAD_H
#define DETECTTHREAD_H

#include <QThread>
#include <QProgressDialog>
#include <QMessageBox>

#include <QIcon>
#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>

#include <QTimer>

#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <qtextcodec.h>
#include <QDir>

#include <QMap>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Detect/base64.h"

#include "parseJson.h"
#include <QJsonObject>

#include "cfg/config.h"
#include <zmq.h>
#include <iostream>
#include <string>
#include <czmq.h>

//#include <common/json/json.h>
//#include "common/base64.h"


#include "Detect/sqldatabasehelper.h"

using namespace std;
using namespace cv;

class DetectThread : public QThread
{
    Q_OBJECT
public:
    explicit DetectThread(QObject *parent=0,QProgressDialog *progress = 0, QMessageBox * message_box = 0);
    ~DetectThread();

    void setInterfaceAndLabel(int interface_id,QLabel *label);
    void setImagePath(QString path);
    
    void updateAllLabeledObjImgs();                        // ���»�ȡ�Ѿ���ע�Ķ�����Ϣ
    
    QMap<int, QMap<QString, QList<int>>> getObjAttrMaps(); // ��ȡ˽�г�Ա���� 
    
private:
    Ptr<ORB> orb;                                          // opencv ƥ�����
    QMap<int, QMap<QString, QList<int>> > obj_attr_maps;   // ���Ķ��������ֵ [ �ĸ����(0,1,2,3), ÿ������������Ժͱ�ŵ�map ]
    QList<QList<img_info>> all_labeled_obj_imgs;           // �����Ѿ���ע�Ķ�����Ϣ
    
    
    void addImgAttrToMap(QString img_path, int label, int index); // �����Ķ��������ֵ��ӵ�map��
    float cacuDistance(QString img_path1, QString img_path2);     // ��������ͼƬ�ľ���
    

protected:
    void run();
    

public:
    QPixmap normal_drawed_map;
    QPixmap ship_drawed_map;
    QPixmap sar_drawed_map;
    //QPixmap drawed_map;             // ��ͼ���ͼƬ
    bool isUsed = true;             // ���Ľ���Ƿ������ܷ�����ʹ��
    

// crop image functions
private:

    void cropObjectFromImage(QImage original_img, std::vector<float> detection, int index);
    void computerWH(std::vector<float> &d, float &h, float &w);
    

// ���ݿ������
private:
    SQLDatabaseHelper dbhelper;


private:
    bool isDetect = true;             // �Ƿ����ڼ��
    QProgressDialog * progress;
    QMessageBox * message_box;
    QDialog *info_dialog = nullptr;   // ship classes information dialog
    QListWidget *list_widget = nullptr;// list widget
    
    QTimer * timer;                   // ��ʱ��

    int currentValue = 0;             // �������ĵ�ǰֵ
    int timeout = 20000;               // ������Ϊ 20s ʱ��Ϊ����ʱ

    QLabel * image_label;             // ��ʾͼƬ��label
    QString path;                     // ͼƬ·��

    QList<Qt::GlobalColor> label_colors;               // ���ò�ͬ�������ɫ
    std::string LABEL[80];            // ��ǰ�㷨�ı�ǩ���
    QTextCodec *codec;                // ��ֹ����
    
    std::vector<std::vector<float> > detections;       // the result of the detect
    
private:  
    
    ConfigParams * params;                               // ������Ϣ����
    std::string url = "";                                // ������ url ��ַ
    int interface_id = -1;                               // ���ýӿ� id ����ִ�в�ͬ�ļ���㷨
    void *context = NULL;                                // ������
    void *ReqSock = NULL;                                // �׽���


private:
    
    QPixmap paintObject(std::vector<std::vector<float> > detections, QString path);                                                           // ����ˮƽ�ı߽��
    QPixmap paintRObject(std::vector<std::vector<float> > detections, QString path);                                                          // �����нǶȵı߽��
    
    void create_client(void *ReqSock);                           // �����ͻ���
    void create_send(void *ReqSock, char * path, int msg_tpye);  // ��������

signals:
    void timerStart();     // ������ʱ���ź�
    void updateProgress(); // ���½������ź�
    void detectOver();     // �������ź�
    void error(int,std::string);

private slots:
    void startTimer();      // ������ʱ���Ĳ�
    void valueChanged();    // �������ı�Ĳ�
    void updateDetectOver();// �������Ĳۣ���ʼ���������ʱ��
    void cancel();          // ȡ����ť�Ĳ�
    void errorMsg(int,std::string);
};

#endif // DETECTTHREAD_H
