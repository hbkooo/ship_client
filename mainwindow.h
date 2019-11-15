#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QIcon>
#include "clickablelabel.h"
#include <QFileDialog>
#include <QEvent>
#include <QMessageBox>

#include <QPainter>
#include <QPen>

#include <QProgressBar>
#include <QProgressDialog>
#include <QMessageBox>

#include <QButtonGroup>
#include <QRadioButton>

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTableView>
#include <QStandardItemModel>

#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include <QCoreApplication>
#include <QFontDatabase>
#include <QIODevice>

#include <QMouseEvent>
#include <qtextcodec.h>

#include <QPushButton>
#include <Detect/sqldatabasehelper.h>

#include <Detect/detectthread.h>
#include <cfg/config.h>

#include "objlabel.h"
#include "analysis.h"

#include "Detect/parseJson.h"

#define IMAGEWIDTH 960
#define IMAGEHEIGHT 720
#define MAXROWCOUNT 9

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *login_widget,QWidget *parent = 0);
    ~MainWindow();

private:
    
    void SetMainWindowLayout();               // 设置窗口的布局
    void InitMenuLabel();                     // 初始化菜单label
    void SetConnect();                        // 绑定控件与事件函数
    
    void startDetect();                       // 开始检测
    void setMenuLabel(ClickableLabel* label); // 设置选中label菜单后的效果
    void setMainImage(int inedx);             // 设置选中菜单后要显示的主界面图片
    
    void InitAnalysis();                      // 初始化智能分析的界面

    // 检测的对象的属性值 [ 四个类别(0,1,2,3), 每个类别下是属性和编号的map ]
    void updateAnalysis(QMap<int, QMap<QString, QList<int>> > obj_attr_maps);                    // 更新智能分析的结果
    void addItemOfListWidget(QListWidget *list_widget, QList<int> indexs, QMap<QString,QString> attrs, int label);          // 添加列表中的某一项

    void updateAnalysisWidget();    // 因窗口大小改变而更新智能分析界面

// 智能分析的成员变量
private:
    
    Analysis *analysis_wg;               // choose analysis detect widget

// 对象标注
private:

    ObjLabel * obj_label_wg;
    


private:

    QWidget * login_widget;

    QString bg_color_style = "background-color:#2a2f55;";      //主界面的背景颜色控制#00005a
    QString item_selected_style = "background-color:#5b5bd7;"; //菜单选项选中后的颜色控制
    QString black_bg_style = "background-color:#111126;";             //界面中黑色的背景具体颜色

    QString font_light_name = "Source Han Sans SC";  // Source Han Sans SC     Noto Sans S Chinese DemiLight
    QString font_medium_name = "Source Han Sans CN";  // Noto Sans S Chinese Black Source Han Sans CN
    QFont font_light;            //浅思源字体
    QFont font_medium;           //中等思源字体

    QWidget *head_widget;        //标题栏控件
    QHBoxLayout *header_layout;  //标题栏布局
    QLabel *header_label;        //标题栏的标题label
    QLabel *header_role_lb;      //标题栏的角色label
    QPushButton *head_image_btn; //标题栏的头像button
    QPushButton *head_exit_btn;  //标题栏的退出button

    QHBoxLayout *main_layout;    // 主界面的布局
    QWidget *main_widget;        // 主界面的控件

    //ClickableLabel *helmet_person_lb; // helmet and person menu,Clickable label
    ClickableLabel *normal_lb;        // normal menu ,Clickable label
    ClickableLabel *ships_lb;         // ships menu,Clickable label
    ClickableLabel *red_SAR_lb;       // red and SAR menu，Clickable label
    ClickableLabel *analysis_lb;       // analysis menu，Clickable label
    ClickableLabel *label_obj_lb;       // label object menu，Clickable label

    QSize img_fixed_size;             // set the main image'size
    QLabel *normal_image_lb;          // choose normal detect imgae
    QLabel *ships_image_lb;           // choose ships detect image
    QLabel *red_SAR_image_lb;         // choose red and SAR detect image

    QString image_path;               // 原始图片的地址
    QPixmap original_img;             // 桌面要显示的原始图片

    QPushButton *file_btn;            // 主界面的文件button
    QList<ClickableLabel*> menu;      // 菜单
    QList<QWidget*> main_img_list;    // 主界面显示的图片集合

    QVBoxLayout *layout;              // 整个窗口界面的布局

    int current_index = 0;            // 记录当时窗口显示的图片类型（菜单选项序号）
    int current_interface = -1;       // 记录当前的算法的接口

    QIcon logo_icon;

    bool mMoveing; //判断是否正在拖动窗口
    QPoint last;   //记录上一次的位置
    
    QTextCodec *codec;   // 防止乱码
    
private:
    DetectThread *detector = NULL;             // object detector
    QProgressDialog * detect_progress = NULL;  // detect progress
    QMessageBox * message_box = NULL;          // information dialog
    
    ConfigParams * params;                     // 配置信息参数




protected:
//    bool eventFilter(QObject* watched,QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);


private slots:
    void on_filebtn_clicked();  // 文件图标点击事件
    void on_exitbtn_clicked();  // 退出按钮点击事件
    void on_label_selected();   // 菜单label点击事件
    

};

#endif // MAINWINDOW_H
