#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QWidget>
#include <QMap>
#include <QPixmap>
#include<QListWidget>
#include <QStandardItemModel>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>

#include <QTextCodec>

#include "cfg/config.h"
#include "Detect/detectthread.h"
#include "Detect/sqldatabasehelper.h"

class Analysis : public QWidget
{
    Q_OBJECT
public:
    Analysis(QSize fixed_size, QWidget *parent = 0);

    ~Analysis();

    void setFixedSize(QSize fixed_size);
    void setDetectorThread(DetectThread *detector);

    void updateAnalysis(QMap<int, QMap<QString, QList<int>> > obj_attr_maps);
    void updateAnalysisWidget();    // 因窗口大小改变而更新智能分析界面

// 智能分析的成员变量
private:

    QSize fixed_size;
    QTextCodec *codec;   // 防止乱码
    ConfigParams * params;                     // 配置信息参数



    DetectThread *detector = nullptr;

    QLabel *result_img_label;           // 检测结果图像
    QListWidget *anal_list_widget = nullptr; // list widget
    QWidget *anal_right_widget ;    //

    int anal_right_width;                    // 右边控件的宽度


private:

    void addItemOfListWidget(QListWidget *list_widget, QList<int> indexs, QMap<QString,QString> attrs, int label_id);

signals:

public slots:
};

#endif // ANALYSIS_H
