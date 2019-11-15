#include "mainwindow.h"
#include <QDebug>
#include <QtGui/QMouseEvent>
#include <QApplication>
#include <QProcess>
#include <qtextcodec.h>
#include <Detect/detectthread.h>

//#include <Detect/initcaffethread.h>

#include "cfg/config.h"
#include <string>

MainWindow::MainWindow(QWidget *login_widget,QWidget *parent)
    : QWidget(parent)
{
    
    params = ConfigParams::instance();
    current_index = 1;
    current_interface = params->INTERFACEID_YOLO;
    codec = QTextCodec::codecForName(params->CODE_NAME.data());
    
    //qDebug() << "in mainwindow : params->CODE_NAME.data() : " << params->CODE_NAME.data();
    
    setMinimumSize(1366,774);  //设置最小窗口大小



    this->size().width();
    this->size().height();
    
    //setWindowFlags(Qt::FramelessWindowHint);  //隐藏顶部
    logo_icon.addFile(":/img/logotitle");
    setWindowIcon(logo_icon);

    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // 界面标题label
    header_label = new QLabel(codec->toUnicode("图像数据深度学习通用平台系统"));
    header_label->setStyleSheet("font-family:KaiTi_GB2312;"
                                "font-size:34px;"
                                "color:white");

#if 0
    header_role_lb = new QLabel("admin");            //角色label
    header_role_lb->setStyleSheet("color:white");

    head_image_btn = new QPushButton();              //角色头像button
    head_image_btn->setObjectName("head_img");
    QIcon icon(":/image/head");
    head_image_btn->setIcon(icon);
    head_image_btn->setIconSize(QSize(42,42));
    head_image_btn->setFlat(true);
    head_image_btn->setStyleSheet("background:blue");
#endif

    head_exit_btn = new QPushButton();                //用户退出button
    head_exit_btn->setObjectName("head_exit");
    QIcon exit_icon(":/img/exit");
    head_exit_btn->setIcon(exit_icon);
    head_exit_btn->setIconSize(QSize(30,30));
    head_exit_btn->setFlat(true);  //去掉按钮边框
    //head_exit_btn->setStyleSheet("background:red;");
    //head_exit_btn->setStyleSheet("QPushButton:hover{border-image:url(:/img/exit);color:red}");

    //helmet_person_lb = new ClickableLabel(codec->toUnicode("安全帽行人检测"));  // helmet and person label
    normal_lb = new ClickableLabel(codec->toUnicode("民用检测"));                // normal menu label
    ships_lb = new ClickableLabel(codec->toUnicode("舰船检测"));                 // ships menu label
    red_SAR_lb = new ClickableLabel(codec->toUnicode("红外&SAR"));               // red and SAR menu label
    analysis_lb = new ClickableLabel(codec->toUnicode("智能分析"));              // analysis menu label
    label_obj_lb = new ClickableLabel(codec->toUnicode("目标对象特征标注"));              // analysis menu label
    //menu.append(helmet_person_lb);
    menu.append(normal_lb);
    menu.append(ships_lb);
    menu.append(red_SAR_lb);
    menu.append(analysis_lb);
    menu.append(label_obj_lb);
    InitMenuLabel();

    img_fixed_size.setWidth(this->size().width() - 260);                // set the size of the image (1106)    1660 = WINDOWS_WIDTH - 180 - 20 - 60
    img_fixed_size.setHeight(this->size().height() - 146);                     // (622)      934 = WINDOWS_HEIGHT - 66 - 40 - 40

    normal_image_lb = new QLabel("");
    ships_image_lb = new QLabel("");
    red_SAR_image_lb = new QLabel("");
    analysis_wg = new Analysis(img_fixed_size);
    obj_label_wg = new ObjLabel(img_fixed_size);
    
    main_img_list.append(normal_image_lb);
    main_img_list.append(ships_image_lb);
    main_img_list.append(red_SAR_image_lb);
    main_img_list.append(analysis_wg);
    main_img_list.append(obj_label_wg);
    //normal_image_lb->hide();
    ships_image_lb->hide();
    red_SAR_image_lb->hide();
    analysis_wg->hide();
    obj_label_wg->hide();

    file_btn = new QPushButton();           //文件button
    file_btn->setObjectName("file_btn");
    QIcon file_icon(":/img/file");
    file_btn->setIcon(file_icon);
    file_btn->setIconSize(QSize(30,30));
    file_btn->setFlat(true);
    //file_btn->setStyleSheet("background:blue");

    SetMainWindowLayout();
    SetConnect();
    this->login_widget = login_widget;
    login_widget->hide();
    
}

void MainWindow::SetMainWindowLayout()
{
    //标题栏的布局
    header_layout = new QHBoxLayout();
    header_layout->setMargin(0);  // 设置布局的外围的距离为零
    header_layout->setSpacing(0); //设置布局内的各控件之间默认距离为0
    header_layout->setContentsMargins(0,0,0,0); //设置布局内的控件距离边界的距离left、top、right、bottom
    header_layout->addSpacing(34);
    header_layout->addWidget(header_label,0,Qt::AlignVCenter);
    header_layout->addStretch();
//    header_layout->addWidget(header_role_lb,0,Qt::AlignVCenter);
//    header_layout->addSpacing(12);
//    header_layout->addWidget(head_image_btn,0,Qt::AlignVCenter);
//    header_layout->addSpacing(20);
    header_layout->addWidget(head_exit_btn,0,Qt::AlignVCenter);
    header_layout->addSpacing(10);   // 退出按钮与右边框的距离

    head_widget = new QWidget();
    head_widget->setStyleSheet(bg_color_style);
    //head_widget->setFixedSize(this->width(),66);   //设置控件大小为固定大小
    head_widget->setFixedHeight(66);
    head_widget->setLayout(header_layout);         //为控件设置布局

    QVBoxLayout *left = new QVBoxLayout();  // 主窗口的左边一列的菜单按钮布局
    left->setMargin(0);
    left->setSpacing(0);
    QWidget *space = new QWidget();    //在菜单按钮上方显示黑条间隔以便与标题分隔开
    space->setStyleSheet(black_bg_style);
    space->setFixedHeight(6);          //设置背景间隔条的高度为6px
    left->addWidget(space);
    left->addSpacing(34);
    foreach(ClickableLabel * label,menu) {
        left->addWidget(label);
    }
    left->addStretch();
    QWidget *left_widget = new QWidget();
    left_widget->setStyleSheet(bg_color_style);
    left_widget->setFixedWidth(180);
    left_widget->setLayout(left);

    QVBoxLayout *center = new QVBoxLayout();
    center->setMargin(0);
    center->setSpacing(0);
    center->addStretch();
    foreach(QWidget * widget,main_img_list) {
        center->addWidget(widget);
    }
    center->addStretch();

    QVBoxLayout *right = new QVBoxLayout();
    right->setMargin(0);
    right->setSpacing(0);
    right->addSpacing(40);
    right->addWidget(file_btn,0,Qt::AlignHCenter);
    right->addStretch();
    QWidget *right_widget = new QWidget();
    right_widget->setFixedWidth(60);
    right_widget->setLayout(right);

    main_layout = new QHBoxLayout();
    main_layout->setMargin(0);
    main_layout->setSpacing(0);
    main_layout->addWidget(left_widget);
    main_layout->addSpacing(20);
    main_layout->addLayout(center);
    main_layout->addStretch();
    main_layout->addWidget(right_widget);

    main_widget = new QWidget();
    main_widget->setStyleSheet(black_bg_style);
  //  main_widget->setFixedHeight(this->height()-66);
    main_widget->setLayout(main_layout);

    layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(head_widget);
    layout->addWidget(main_widget);

    setLayout(layout);

    qDebug() << normal_image_lb->size();

}

void MainWindow::InitMenuLabel()
{
    font_light.setFamily(font_light_name);
    font_light.setPixelSize(18);
    font_medium.setFamily(font_medium_name);
    font_medium.setPixelSize(18);
    foreach(ClickableLabel* lb , menu){
        lb->setFixedHeight(48);
        lb->setAlignment(Qt::AlignVCenter);
        lb->setContentsMargins(34,0,0,0);
        lb->setStyleSheet(bg_color_style + "color:rgba(255, 255, 255, 0.7);");
        lb->setFont(font_light);
    }
    menu.at(0)->setStyleSheet(item_selected_style +
                                "color:rgba(255, 255, 255, 1);");
    menu.at(0)->setFont(font_medium);
}

void MainWindow::setMenuLabel(ClickableLabel* label)
{
    foreach(ClickableLabel* lb , menu){
        lb->setStyleSheet(bg_color_style + "color:rgba(255, 255, 255, 0.7);");
        lb->setFont(font_light);
    }
    label->setStyleSheet(item_selected_style +
                         "color:rgba(255, 255, 255,1);");
    label->setFont(font_medium);
}

void MainWindow::SetConnect()
{
    connect(head_exit_btn,SIGNAL(clicked()),this,SLOT(on_exitbtn_clicked()));
    connect(file_btn,SIGNAL(clicked()),this,SLOT(on_filebtn_clicked()));
    foreach(QLabel* label,menu)
    {
        connect(label,SIGNAL(clicked()),this,SLOT(on_label_selected()));
    }
}

void MainWindow::on_filebtn_clicked()
{
    
    if (current_index == 4) {
        QMessageBox::warning(this,codec->toUnicode("警告"),codec->toUnicode("正在分析图像，不能对图像进行检测..."));
        return ;
    }
    if (current_index == 5) {
        QMessageBox::warning(this,codec->toUnicode("警告"),codec->toUnicode("正在标注图像，不能对图像进行检测..."));
        return ;
    }
    
    // "/home/hbk/data/image/"/home/xw/data/ships/train/JPEGImages
    QFileDialog *file_dialog = new QFileDialog(this,codec->toUnicode("请选择图片"),"/home/hbk/data/709testData/","Images(*.png *.xpm *.jpg *.bmp)");

    if(file_dialog->exec() == QFileDialog::Accepted) {
        image_path = file_dialog->selectedFiles()[0];
        qDebug() << image_path;
        //QPixmap map(image_path);
        
        QPixmap map = loadPixImage(image_path);
        
        if(!map) {
            qDebug() << "image is null ...";
            QMessageBox::warning(this,codec->toUnicode("图片加载失败"),codec->toUnicode("图片打开失败"));
        } else {
            original_img = map;
            original_img = original_img.scaled(img_fixed_size);
            ((QLabel*)main_img_list.at(current_index-1))->setPixmap(original_img);
            startDetect();
        }
    } else {
        file_dialog->close();
    }
}

void MainWindow::startDetect()
{

    // TODO 算法处理，开启线程，显示进度条，获取边界框、类别和得分
    // 然后在original_img上进行画图标注，加载到formated_img，然后显示到current_lb上

    //qDebug() << "start detect and main thread is:" << QThread::currentThreadId();

    if (detector == NULL) {
        if (detect_progress == NULL) {      
            detect_progress = new QProgressDialog(codec->toUnicode("正在检测..."),codec->toUnicode("取消"),0,100,this);      
            detect_progress->setFixedWidth(300);
            detect_progress->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
            detect_progress->setWindowTitle(codec->toUnicode("请稍等"));
            detect_progress->setModal(true);
        }
        if (message_box == NULL) {
            message_box = new QMessageBox(this); 
            message_box->setIcon(QMessageBox::Warning);
            message_box->setWindowTitle(codec->toUnicode("警告"));
            message_box->setText(codec->toUnicode("请求超时"));
            message_box->setModal(true);
            message_box->setFixedSize(300,200);
            QFont font;
            font.setPixelSize(20);
            message_box->setFont(font);
            //message_box->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
            message_box->setDefaultButton(QMessageBox::Yes);
            message_box->setHidden(true);
        }
        detector = new DetectThread(0,detect_progress, message_box);
        connect(detect_progress,SIGNAL(canceled()),detector,SLOT(cancel()));
        analysis_wg->setDetectorThread(detector);
        obj_label_wg->setDetectorThread(detector);
    }

    QLabel* current_lb = (QLabel*) main_img_list.at(current_index-1);
    detector->setImagePath(image_path);
    detector->setInterfaceAndLabel(current_interface, current_lb);
    detector->start();

}

void MainWindow::on_label_selected()
{
    ClickableLabel *label = (ClickableLabel*)sender();
    setMenuLabel(label);      //设置label选中效果
    QString name = label->text();

    if (name == codec->toUnicode("民用检测")) {
        current_index = 1;
        current_interface = params->INTERFACEID_YOLO;
    } else if (name == codec->toUnicode("舰船检测")) {
        current_index = 2;
        current_interface = params->INTERFACEID_SHIP;
    } else if (name == codec->toUnicode("红外&SAR")) {
        current_index = 3;
        current_interface = params->INTERFACEID_SAR;
    } else if (name == codec->toUnicode("智能分析")) {
        current_index = 4;
    } else if (name == codec->toUnicode("目标对象特征标注")) {
        current_index = 5;
    }
    setMainImage(current_index);
}

void MainWindow::setMainImage(int index)
{
    foreach(QWidget * widget,main_img_list)
    {
        widget->hide();
    }
    QWidget * current_wg = main_img_list.at(index-1);
    current_wg->setVisible(true);
    
    if (index == 4 && detector != NULL) {
        qDebug() << "update analysis ...";
        // 只对舰船的检测结果进行分析
        if (detector->ship_drawed_map.isNull())
            return;
        // 更新智能分析的结果
        analysis_wg->updateAnalysis(detector->getObjAttrMaps());
    } else if(index == 5) {
        qDebug() << "update object label ...";
        obj_label_wg->showObjLable();
    }
    
}

void MainWindow::on_exitbtn_clicked()
{    
    qDebug() << "in on_exitbtn_clicked and release space...";
    if(detector != NULL) {
        qDebug() << "detector is not null and delete detector thread";
        delete detector;
        detector = NULL;
    }
    close();
    login_widget->show();
}

MainWindow::~MainWindow()
{
    qDebug() << "in ~MainWindow and release space...";
    if(detector != NULL) {
        qDebug() << "detector is not null and delete detector thread";
        delete detector;
        detector = NULL;
    }
    if (params != NULL)
        delete params;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(head_widget && head_widget->contentsRect().contains(event->pos())) {
        last = event->globalPos();
        mMoveing = true;
    }
    return QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mMoveing && (event->buttons() == Qt::LeftButton))
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        move(this->x()+dx,this->y()+dy);
        last = event->globalPos();
    }
    return QWidget::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mMoveing = false;
    return QWidget::mouseReleaseEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    qDebug() << event->size();
    img_fixed_size.setHeight(event->size().height()-146);
    img_fixed_size.setWidth(event->size().width()-260);
    normal_image_lb->setFixedSize(img_fixed_size);
    ships_image_lb->setFixedSize(img_fixed_size);
    red_SAR_image_lb->setFixedSize(img_fixed_size);
    analysis_wg->setFixedSize(img_fixed_size);
    obj_label_wg->setFixedSize(img_fixed_size);

    if(detector != NULL) {
        if(!detector->normal_drawed_map.isNull()) {
            detector->normal_drawed_map = detector->normal_drawed_map.scaled(img_fixed_size);
            normal_image_lb->setPixmap(detector->normal_drawed_map);
            qDebug() << "change normal detected image...";
        }
        if(!detector->sar_drawed_map.isNull()) {
                detector->sar_drawed_map = detector->sar_drawed_map.scaled(img_fixed_size);
                red_SAR_image_lb->setPixmap(detector->sar_drawed_map);
                qDebug() << "change sar detected image...";
        }
        if(!detector->ship_drawed_map.isNull()) {
            detector->ship_drawed_map = detector->ship_drawed_map.scaled(img_fixed_size);
            ships_image_lb->setPixmap(detector->ship_drawed_map);
            qDebug() << "change ship detected image...";
        }
    }

    analysis_wg->updateAnalysisWidget();
    obj_label_wg->updateObjLabelWidget();

}
