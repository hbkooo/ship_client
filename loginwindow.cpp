#include "loginwindow.h"
#include <QDebug>
#include <QAction>


LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)
{
    // 加载配置信息
    loadConfig(CONFIG_PATH);
    
    /*
    std::cout << "login SERVER : ' " << ConfigParams::instance()->SERVER << " ' \n";
    std::cout << "ConfigParams::instance()->CODE_NAME : ' " << ConfigParams::instance()->ConfigParams::instance()->CODE_NAME << " ' \n";
    std::cout << "INTERFACEID_YOLO : ' " << ConfigParams::instance()->INTERFACEID_YOLO << " ' \n";
    std::cout << "INTERFACEID_SHIP : ' " << ConfigParams::instance()->INTERFACEID_SHIP << " ' \n";
    std::cout << "INTERFACEID_SAR : ' " << ConfigParams::instance()->INTERFACEID_SAR << " ' \n";*/
    
    setMinimumSize(1366,768);
    setStyleSheet("background-color:#eeeeee");
    
    codec = QTextCodec::codecForName(ConfigParams::instance()->CODE_NAME.data());

    setWindowTitle(codec->toUnicode("用户登录"));
    setWindowIcon(QIcon(":/img/logotitle"));
    
    //QFontDatabase db;
    //QStringList families = db.families();
    //qDebug() << "font1:" << families;

    getSiyuanFont();
    
    //QFontDatabase db1;
    //QStringList families1 = db1.families();
    //qDebug() << "font2:" << families1;

    logo_lb = new QLabel();
    QPixmap map(":/img/logo");
    logo_lb->setPixmap(map);

    main_title_lb = new QLabel(codec->toUnicode("图像数据深度学习\n通用平台系统"));
    //main_title_lb->setFont(font_medium);
    main_title_lb->setStyleSheet("font-family:" + font_light_name + ";"
                                 "font-size:36px;"
                                 "color:#eeeeee");


    line_widget = new QWidget();
    line_widget->setFixedSize(32,6);
    line_widget->setStyleSheet("background-color:#eeeeee");

    user_manage_lb = new QLabel("USER MANAGEMENT BACKGROUND");
    //user_manage_lb->setFont(font_medium);
    user_manage_lb->setStyleSheet("font-size:26px;"
                                  "color:#eeeeee");
    user_manage_chinese_lb = new QLabel(codec->toUnicode("用户管理后台"));
    //user_manage_chinese_lb->setFont(font_light);
    user_manage_chinese_lb->setStyleSheet("font-family:" + font_light_name + ";"
                                          "font-size:12px;"
                                          "color:#eeeeee");

    welcome_lb = new QLabel(codec->toUnicode("欢迎登录"));
    welcome_lb->setStyleSheet("font-family:" + font_light_name + ";"
                              "font-size:16px;"
                              "color:#333333");

    username_le = new QLineEdit();
    username_le->setMaxLength(20);
    username_le->setFixedSize(218,42);
    username_le->setTextMargins(38,0,0,0);
    username_le->installEventFilter(this);          //注册监听器
    username_le->setAlignment(Qt::AlignVCenter);
    //username_le->setFont(font_medium);
    username_le->setStyleSheet("QLineEdit { border: 1px none #e5e5e5;"
                               "font-family:" + font_light_name + ";"
                               "border-bottom-style:solid;"
                               "font-size:14px;"
                               "color:#666666; }"
                               "QLineEdit:focus {"
                               "border-color:#5b5dd7;}");

    QPixmap user_map_nor(":/img/user_nor");
    QPixmap user_map_sel(":/img/user_sel");
    user_nor_icon.addPixmap(user_map_nor);
    user_sel_icon.addPixmap(user_map_sel);

    user_btn = new QPushButton();
    user_btn->setFixedSize(18,18);
    user_btn->setIcon(user_nor_icon);
    user_btn->setIconSize(QSize(18,18));
    user_btn->setFlat(true);
    user_btn->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout *user_layout = new QHBoxLayout();
    user_layout->setMargin(0);
    user_layout->setSpacing(0);
    user_layout->setContentsMargins(0,0,0,0);
    user_layout->addWidget(user_btn);
    user_layout->addStretch();
    username_le->setLayout(user_layout);

    password_le = new QLineEdit();
    password_le->setMaxLength(16);
    password_le->setEchoMode(QLineEdit::Password);
    password_le->setAlignment(Qt::AlignVCenter);
    password_le->installEventFilter(this);               //注册密码监听器
    password_le->setFixedSize(218,42);
//    password_le->setContentsMargins(0,12,0,12);
    password_le->setTextMargins(38,0,0,0);
    password_le->setContentsMargins(0,0,0,0);

    password_le->setStyleSheet("QLineEdit[echoMode=\"2\"] {"
                               "lineedit-password-character: 42;}"
                               "QLineEdit { border: 1px none #e5e5e5;"
                               "border-bottom-style:solid;}"
                               " QLineEdit:focus{"
                               "border-color:#5b5dd7;"
                               "}");

    QPixmap pwd_map_nor(":/img/pwd_nor");
    QPixmap pwd_map_sel(":/img/pwd_sel");
    pwd_nor_icon.addPixmap(pwd_map_nor);
    pwd_sel_icon.addPixmap(pwd_map_sel);

    pwd_btn = new QPushButton();
    pwd_btn->setFixedSize(18,18);
    pwd_btn->setIcon(pwd_map_nor);
    pwd_btn->setIconSize(QSize(18,18));
    pwd_btn->setFlat(true);
    pwd_btn->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout *pwd_layout = new QHBoxLayout();
    pwd_layout->setMargin(0);
    pwd_layout->setSpacing(0);
    pwd_layout->setContentsMargins(0,0,0,0);
    pwd_layout->addWidget(pwd_btn);
    pwd_layout->addStretch();
    password_le->setLayout(pwd_layout);



    login_btn = new QPushButton(codec->toUnicode("登录"));
    login_btn->setFixedSize(232,52);
    login_btn->setStyleSheet("font-family:" + font_light_name + ";"
                             "font-size:14px;"
                             "color:white;"
                             "background-image:url(:/img/login_btn_bg);");
    //login_btn->setFont(font_medium);

    info_lb = new QLabel(codec->toUnicode("@中国船舶重工集团公司第七〇九研究所版权所有"));
    info_lb->setStyleSheet("font-family:" + font_light_name + ";"
                           "font-size:14px;"
                           "color:#666666");

    setMainLayout();

}

void LoginWindow::setMainLayout()
{

    QVBoxLayout *left_layout = new QVBoxLayout();
    left_layout->setMargin(0);
    left_layout->setSpacing(0);
    left_layout->addSpacing(36);
    left_layout->addWidget(logo_lb);
    left_layout->addSpacing(36);
    left_layout->addWidget(main_title_lb);
    left_layout->addSpacing(18);
    left_layout->addWidget(line_widget);
    left_layout->addStretch();
    left_layout->addWidget(user_manage_lb);
    left_layout->addSpacing(10);
    left_layout->addWidget(user_manage_chinese_lb);
    left_layout->addSpacing(72);
    left_layout->setAlignment(user_manage_lb,Qt::AlignRight);
    left_layout->setAlignment(user_manage_chinese_lb,Qt::AlignRight);

    QWidget * left_widget = new QWidget();
    left_widget->setFixedSize(586,460);
    left_widget->setContentsMargins(40,0,40,0);
    left_widget->setStyleSheet("background-color: rgba(0, 0, 0, 0);");    //背景透明
    left_widget->setLayout(left_layout);


    QVBoxLayout *right_layout = new QVBoxLayout();
    right_layout->setMargin(0);
    right_layout->setSpacing(0);
    right_layout->addSpacing(120);
    right_layout->addWidget(welcome_lb);
    right_layout->addSpacing(6);
    QWidget *under_line_widget = new QWidget();
    under_line_widget->setStyleSheet("background-color:#5b5dd7;");      //背景透明
    under_line_widget->setFixedSize(60,2);
    right_layout->addWidget(under_line_widget);
    right_layout->addSpacing(20);//32-瀛椾綋鐨勯珮搴???
    right_layout->addWidget(username_le);
    right_layout->addWidget(password_le);
    right_layout->addSpacing(40);
    right_layout->addWidget(login_btn);
    right_layout->addStretch();
    right_layout->setAlignment(welcome_lb,Qt::AlignHCenter);
    right_layout->setAlignment(under_line_widget,Qt::AlignHCenter);
    right_layout->setAlignment(username_le,Qt::AlignHCenter);
    right_layout->setAlignment(password_le,Qt::AlignHCenter);
    right_layout->setAlignment(login_btn,Qt::AlignHCenter);

    QWidget *right_widget = new QWidget();
    right_widget->setFixedSize(266,460);
    right_widget->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    right_widget->setLayout(right_layout);

    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->setMargin(0);
    main_layout->setSpacing(0);
    main_layout->addStretch();
    main_layout->addWidget(left_widget);
    main_layout->addWidget(right_widget);
    main_layout->addStretch();

    QWidget * main_widget = new QWidget();
//    main->setFixedSize(852,460);
    main_widget->setFixedSize(866,474);  //背景图片大小
    main_widget->setObjectName("main");
    main_widget->setStyleSheet("QWidget#main{background-image: url(:/img/main_login_bg);}");
    main_widget->setLayout(main_layout);


    layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addStretch();
    layout->addWidget(main_widget);
//    layout->addSpacing(10);
    layout->addWidget(info_lb);
    layout->addStretch();
    layout->setAlignment(Qt::AlignHCenter);
    layout->setAlignment(info_lb,Qt::AlignHCenter);

    setLayout(layout);
    connect(login_btn,SIGNAL(clicked()),this,SLOT(on_login_clicked()));

}

void LoginWindow::on_login_clicked()
{
    QString username = username_le->text();
    QString password = password_le->text();
    if (username == "admin" && password == "123") {
        // 用户登录
        main_window = new MainWindow(this);
//        main_window->show();
        main_window->showMaximized();
        this->hide();
    } else {
        // 密码错误，弹窗提示
        QMessageBox * messageBox = new QMessageBox(this);
        messageBox->setWindowTitle(codec->toUnicode("提示"));
        messageBox->setText(codec->toUnicode("用户名或密码错误"));
        messageBox->show();
    }
}

LoginWindow::~LoginWindow()
{
    qDebug() << "delete in LoginWindow";
    if (main_window != NULL) {
        qDebug() << "main_window is not null and release space of main_window...";
        delete main_window;
    }
}

void LoginWindow::getSiyuanFont()
{
    QString path = QCoreApplication::applicationDirPath();//获取工程工作路径
    font_light_name = loadFontFamilyFromFiles(path+"/font/SourceHanSansSC-Normal.otf");  // SourceHanSansCN-Normal.otf  SourceHanSansSC-Normal.otf   ==> name : Source Han Sans SC
    font_medium_name = loadFontFamilyFromFiles(path+"/font/SourceHanSansCN-Medium.otf");  // SourceHanSansCN-Medium.otf   SourceHanSansSC-Medium.otf   ==> name : Source Han Sans CN
    font_kaiti_name = loadFontFamilyFromFiles(path + "/font/kaiti_GB2312.ttf");
    font_light.setFamily(font_light_name);
    font_medium.setFamily(font_medium_name);
    qDebug() << font_light_name;
    qDebug() << font_medium_name;
    qDebug() << font_kaiti_name;
}

QString LoginWindow::loadFontFamilyFromFiles(const QString &fontfile)
{
    QString font = "";
    QFile fontFile(fontfile);

    qDebug() << fontfile;

    if(!fontFile.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Open font file error";
        return font;
    }
    int loadedFontID = QFontDatabase::addApplicationFontFromData(fontFile.readAll());
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
    if(!loadedFontFamilies.empty())
    {
        font = loadedFontFamilies.at(0);
    }
    fontFile.close();
    return font;
}

bool LoginWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == username_le)
    {
        if(ev->type() == QEvent::FocusIn)
        {
            qDebug() << "user focus in";
            user_btn->setIcon(user_sel_icon);
        } else if (ev->type() == QEvent::FocusOut)
        {
            qDebug() << "user focus out";
            user_btn->setIcon(user_nor_icon);
        }
    }
    if(obj == password_le)
    {
        if(ev->type() == QEvent::FocusIn)
        {
            pwd_btn->setIcon(pwd_sel_icon);
        } else if (ev->type() == QEvent::FocusOut)
        {
            pwd_btn->setIcon(pwd_nor_icon);
        }
    }
    return QWidget::eventFilter(obj,ev);
}


