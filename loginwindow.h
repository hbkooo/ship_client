#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "cfg/config.h"
#include "mainwindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QEvent>

class LoginWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    void setMainLayout();

    void getSiyuanFont();                     // 获取思源字体
    QString loadFontFamilyFromFiles(const QString &fontfile); //根据字体文件地址加载字体


private:
    QString font_light_name = "";   // Noto Sans S Chinese DemiLight
    QString font_medium_name = "";  // Noto Sans S Chinese Black
    QString font_kaiti_name = "";   // KaiTi_GB2312
    QFont font_light;               // light
    QFont font_medium;              // medium

    QString color = "#5b5dd7";

    QLabel * image_lb;       //登录显示图像

    QLabel * logo_lb;        //logo图标
    QLabel * main_title_lb;  //主界面的标题信息
    QWidget * line_widget;   //短线
    QLabel * user_manage_lb; //用户管理英文字体
    QLabel * user_manage_chinese_lb;//用户管理中文字体


    QLabel * welcome_lb;     //“欢迎登录”显�?

    QLineEdit * username_le; //用户名输入框
    QIcon user_nor_icon;     //用户名输入栏默认情况
    QIcon user_sel_icon;     //用户名输入栏默认情况
    QPushButton *user_btn;   //用户名输入栏图标button

    QLineEdit * password_le; //密码输入�?
    QIcon pwd_nor_icon;      //密码输入栏默认情�?
    QIcon pwd_sel_icon;      //密码输入栏默认情�?
    QPushButton * pwd_btn;   //密码输入栏图标button

    QPushButton * login_btn; //登录按钮
    QLabel * info_lb;        //底部的版权显�?
    QVBoxLayout * layout;    //界面的主布局

    MainWindow* main_window = NULL;
    
    
    QTextCodec *codec;   // ��ֹ����


signals:

public slots:
    bool eventFilter(QObject * obj,QEvent* ev);  //注册监听器，监听用户名与密码输入框的焦点获取情况
    void on_login_clicked();
};

#endif // LOGINWINDOW_H
