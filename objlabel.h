#ifndef OBJLABEL_H
#define OBJLABEL_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QTextCodec>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <cfg/config.h>
#include "Detect/sqldatabasehelper.h"
#include "Detect/detectthread.h"
#include "clickablelabel.h"

class ObjLabel : public QWidget
{
    Q_OBJECT
public:
    ObjLabel(QSize fixed_size,QWidget *parent = 0);

    ~ObjLabel();


    void updateObjLabelWidget();            // 因窗口大小改变而更新对象标注界面
    void setFixedSize(QSize fixed_size);
    void showObjLable();
    void setDetectorThread(DetectThread *detector);

private:
    QTextCodec *codec;   // 防止乱码
    ConfigParams * params;                     // 配置信息参数
    DetectThread *detector = nullptr;


private:
    SQLDatabaseHelper dbhelper;                  // 数据库操作成员变量

    QSize fixed_size;

    QWidget *label_obj_wg;                       // choose label objects result of detection

    QListWidget *obj_img_list_widget = nullptr;  // 显示所有图像的 list widget
    QPushButton *save_btn;                       // 保存标注信息按钮
    QPushButton *add_attr_btn;                   // 新增属性按钮
    ClickableLabel *all_obj_lb;                  // 所有图片
    ClickableLabel *labeled_obj_lb;              // 已标注的对象图片
    ClickableLabel *nolabeled_obj_lb;            // 未标注的对象图片
    QList<ClickableLabel *> labeled_lbs;

    int obj_img_width;                           // 对象标注中每个对象图片的宽
    QLabel *obj_img_name_label;                  // 图像的名字

    QTableView * obj_info_table_view;            // table view
    QStandardItemModel *obj_info_model;          // 为table view 设置数据模式

    QList<img_info> obj_infos;

    ClickableLabel* obj_img_last_click_label = nullptr;  // 上一次点击的图像对象

    int per_row_count = 4;                      // 图片列表中的每一行显示的图片数量
    int obj_img_current_row = -1;               // 当前点击的行号
    int obj_img_current_index = -1;             // 当前点击的图片的序号

    void updateObjLabel(QList<img_info> obj_infos);                      // 更新显示界面
    void InitObjLabelFrame();                   // 初始化标注对象界面
    void addItemOfObjImgListWidget(QListWidget *list_widget, int row);          // 添加列表中的某一项
    QWidget* getOneImageWidget(int row, int index);  // 根据序号获取显示的图片信息


signals:

public slots:

    void on_labeled_clicked();  // 菜单label点击事件
    void on_obj_img_clicked();
    void on_obj_save_btn_clicked();
    void on_obj_add_attr_btn_clicked();

};

#endif // OBJLABEL_H
