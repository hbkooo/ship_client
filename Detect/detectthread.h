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
    
    void updateAllLabeledObjImgs();                        // 更新获取已经标注的对象信息
    
    QMap<int, QMap<QString, QList<int>>> getObjAttrMaps(); // 获取私有成员对象 
    
private:
    Ptr<ORB> orb;                                          // opencv 匹配操作
    QMap<int, QMap<QString, QList<int>> > obj_attr_maps;   // 检测的对象的属性值 [ 四个类别(0,1,2,3), 每个类别下是属性和编号的map ]
    QList<QList<img_info>> all_labeled_obj_imgs;           // 所有已经标注的对象信息
    
    
    void addImgAttrToMap(QString img_path, int label, int index); // 将检测的对象的属性值添加到map中
    float cacuDistance(QString img_path1, QString img_path2);     // 计算两张图片的距离
    

protected:
    void run();
    

public:
    QPixmap normal_drawed_map;
    QPixmap ship_drawed_map;
    QPixmap sar_drawed_map;
    //QPixmap drawed_map;             // 画图后的图片
    bool isUsed = true;             // 检测的结果是否在智能分析中使用
    

// crop image functions
private:

    void cropObjectFromImage(QImage original_img, std::vector<float> detection, int index);
    void computerWH(std::vector<float> &d, float &h, float &w);
    

// 数据库操作类
private:
    SQLDatabaseHelper dbhelper;


private:
    bool isDetect = true;             // 是否正在检测
    QProgressDialog * progress;
    QMessageBox * message_box;
    QDialog *info_dialog = nullptr;   // ship classes information dialog
    QListWidget *list_widget = nullptr;// list widget
    
    QTimer * timer;                   // 计时器

    int currentValue = 0;             // 进度条的当前值
    int timeout = 20000;               // 进度条为 20s 时认为请求超时

    QLabel * image_label;             // 显示图片的label
    QString path;                     // 图片路径

    QList<Qt::GlobalColor> label_colors;               // 设置不同类别框的颜色
    std::string LABEL[80];            // 当前算法的标签类别
    QTextCodec *codec;                // 防止乱码
    
    std::vector<std::vector<float> > detections;       // the result of the detect
    
private:  
    
    ConfigParams * params;                               // 配置信息参数
    std::string url = "";                                // 服务器 url 地址
    int interface_id = -1;                               // 设置接口 id 请求执行不同的检测算法
    void *context = NULL;                                // 上下文
    void *ReqSock = NULL;                                // 套接字


private:
    
    QPixmap paintObject(std::vector<std::vector<float> > detections, QString path);                                                           // 绘制水平的边界框
    QPixmap paintRObject(std::vector<std::vector<float> > detections, QString path);                                                          // 绘制有角度的边界框
    
    void create_client(void *ReqSock);                           // 创建客户端
    void create_send(void *ReqSock, char * path, int msg_tpye);  // 发送数据

signals:
    void timerStart();     // 开启计时器信号
    void updateProgress(); // 更新进度条信号
    void detectOver();     // 检测结束信号
    void error(int,std::string);

private slots:
    void startTimer();      // 启动计时器的槽
    void valueChanged();    // 进度条改变的槽
    void updateDetectOver();// 检测结束的槽，初始检测结束后的时间
    void cancel();          // 取消按钮的槽
    void errorMsg(int,std::string);
};

#endif // DETECTTHREAD_H
