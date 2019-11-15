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
    
    void SetMainWindowLayout();               // ���ô��ڵĲ���
    void InitMenuLabel();                     // ��ʼ���˵�label
    void SetConnect();                        // �󶨿ؼ����¼�����
    
    void startDetect();                       // ��ʼ���
    void setMenuLabel(ClickableLabel* label); // ����ѡ��label�˵����Ч��
    void setMainImage(int inedx);             // ����ѡ�в˵���Ҫ��ʾ��������ͼƬ
    
    void InitAnalysis();                      // ��ʼ�����ܷ����Ľ���

    // ���Ķ��������ֵ [ �ĸ����(0,1,2,3), ÿ������������Ժͱ�ŵ�map ]
    void updateAnalysis(QMap<int, QMap<QString, QList<int>> > obj_attr_maps);                    // �������ܷ����Ľ��
    void addItemOfListWidget(QListWidget *list_widget, QList<int> indexs, QMap<QString,QString> attrs, int label);          // ����б��е�ĳһ��

    void updateAnalysisWidget();    // �򴰿ڴ�С�ı���������ܷ�������

// ���ܷ����ĳ�Ա����
private:
    
    Analysis *analysis_wg;               // choose analysis detect widget

// �����ע
private:

    ObjLabel * obj_label_wg;
    


private:

    QWidget * login_widget;

    QString bg_color_style = "background-color:#2a2f55;";      //������ı�����ɫ����#00005a
    QString item_selected_style = "background-color:#5b5bd7;"; //�˵�ѡ��ѡ�к����ɫ����
    QString black_bg_style = "background-color:#111126;";             //�����к�ɫ�ı���������ɫ

    QString font_light_name = "Source Han Sans SC";  // Source Han Sans SC     Noto Sans S Chinese DemiLight
    QString font_medium_name = "Source Han Sans CN";  // Noto Sans S Chinese Black Source Han Sans CN
    QFont font_light;            //ǳ˼Դ����
    QFont font_medium;           //�е�˼Դ����

    QWidget *head_widget;        //�������ؼ�
    QHBoxLayout *header_layout;  //����������
    QLabel *header_label;        //�������ı���label
    QLabel *header_role_lb;      //�������Ľ�ɫlabel
    QPushButton *head_image_btn; //��������ͷ��button
    QPushButton *head_exit_btn;  //���������˳�button

    QHBoxLayout *main_layout;    // ������Ĳ���
    QWidget *main_widget;        // ������Ŀؼ�

    //ClickableLabel *helmet_person_lb; // helmet and person menu,Clickable label
    ClickableLabel *normal_lb;        // normal menu ,Clickable label
    ClickableLabel *ships_lb;         // ships menu,Clickable label
    ClickableLabel *red_SAR_lb;       // red and SAR menu��Clickable label
    ClickableLabel *analysis_lb;       // analysis menu��Clickable label
    ClickableLabel *label_obj_lb;       // label object menu��Clickable label

    QSize img_fixed_size;             // set the main image'size
    QLabel *normal_image_lb;          // choose normal detect imgae
    QLabel *ships_image_lb;           // choose ships detect image
    QLabel *red_SAR_image_lb;         // choose red and SAR detect image

    QString image_path;               // ԭʼͼƬ�ĵ�ַ
    QPixmap original_img;             // ����Ҫ��ʾ��ԭʼͼƬ

    QPushButton *file_btn;            // ��������ļ�button
    QList<ClickableLabel*> menu;      // �˵�
    QList<QWidget*> main_img_list;    // ��������ʾ��ͼƬ����

    QVBoxLayout *layout;              // �������ڽ���Ĳ���

    int current_index = 0;            // ��¼��ʱ������ʾ��ͼƬ���ͣ��˵�ѡ����ţ�
    int current_interface = -1;       // ��¼��ǰ���㷨�Ľӿ�

    QIcon logo_icon;

    bool mMoveing; //�ж��Ƿ������϶�����
    QPoint last;   //��¼��һ�ε�λ��
    
    QTextCodec *codec;   // ��ֹ����
    
private:
    DetectThread *detector = NULL;             // object detector
    QProgressDialog * detect_progress = NULL;  // detect progress
    QMessageBox * message_box = NULL;          // information dialog
    
    ConfigParams * params;                     // ������Ϣ����




protected:
//    bool eventFilter(QObject* watched,QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);


private slots:
    void on_filebtn_clicked();  // �ļ�ͼ�����¼�
    void on_exitbtn_clicked();  // �˳���ť����¼�
    void on_label_selected();   // �˵�label����¼�
    

};

#endif // MAINWINDOW_H
