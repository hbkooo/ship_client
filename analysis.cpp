#include "analysis.h"

Analysis::Analysis(QSize fixed_size, QWidget *parent) : QWidget(parent)
{

    params = ConfigParams::instance();
    codec = QTextCodec::codecForName(params->CODE_NAME.data());

    this->fixed_size = fixed_size;
    this->setFixedSize(fixed_size);


    int wg_width = fixed_size.width();
    int wg_height = fixed_size.height();

    anal_right_width = (wg_width - 30)/4;
    int img_width = wg_width - 30 - anal_right_width;  // 1200
    int img_height = img_width * 2 / 3;          // 800

    // 图片的最大高度为 widget 控件的高度
    if(img_height + 80 > wg_height)
        img_height = wg_height - 80;


    result_img_label = new QLabel("");
    result_img_label->setFixedSize(QSize(img_width,img_height)); // 1660 = 1200 + 30 + 430


    QLabel * result_label = new QLabel(codec->toUnicode("目标画像"));
    result_label->setStyleSheet("font-family:KaiTi_GB2312;"
                                "font-size:30px;"
                                "color:white");

    anal_list_widget = new QListWidget();
    anal_list_widget->setFixedWidth(anal_right_width);
    anal_list_widget->setStyleSheet("background:white");

    QVBoxLayout * analysis_layout = new QVBoxLayout();
    analysis_layout->setMargin(0);  // 设置布局的外围的距离为零
    analysis_layout->setSpacing(0); //设置布局内的各控件之间默认距离为0
    analysis_layout->setContentsMargins(0,0,0,0); //设置布局内的控件距离边界的距离left、top、right、bottom
    analysis_layout->addWidget(result_label,0,Qt::AlignHCenter);
    analysis_layout->addSpacing(20);
    analysis_layout->addWidget(anal_list_widget);

    anal_right_widget = new QWidget();
    anal_right_widget->setFixedSize(anal_right_width,img_height);
    //right_widget->setStyleSheet("background:green");
    anal_right_widget->setLayout(analysis_layout);



    QHBoxLayout * layout = new QHBoxLayout();
    layout->setMargin(0);  // 设置布局的外围的距离为零
    layout->setSpacing(0); //设置布局内的各控件之间默认距离为0
    layout->setContentsMargins(0,0,0,0); //设置布局内的控件距离边界的距离left、top、right、bottom

    layout->addWidget(result_img_label);
    layout->addSpacing(30);
    layout->addWidget(anal_right_widget);
    //right_widget->setVisible(false);

    this->setLayout(layout);
}

Analysis::~Analysis()
{

}

void Analysis::setFixedSize(QSize fixed_size)
{
    this->fixed_size =fixed_size;
}

void Analysis::setDetectorThread(DetectThread *detector)
{
    this->detector  =detector;
}

void Analysis::updateAnalysisWidget()
{
    int wg_width = fixed_size.width();
    int wg_height = fixed_size.height();

    anal_right_width = (wg_width - 30)/4;
    int img_width = wg_width - 30 - anal_right_width;  // 1200
    int img_height = img_width * 2 / 3;          // 800

    // 图片的最大高度为 widget 控件的高度
    if(img_height + 80 > wg_height)
        img_height = wg_height - 80;

    result_img_label->setFixedSize(QSize(img_width,img_height));

    anal_right_widget->setFixedSize(anal_right_width,img_height);
    anal_list_widget->setFixedWidth(anal_right_width);
    if(detector != NULL)  {
        qDebug() << "change analysis widget ...";
        detector->isUsed = false;
        updateAnalysis(detector->getObjAttrMaps());
    }

}


// 更新智能分析的结果
// 检测的对象的属性值 [ 四个类别(0,1,2,3), 每个类别下是属性和编号的map ]
/**
 * 更新智能分析的结果
 * @brief MainWindow::updateAnalysis
 * @param obj_attr_maps
 *                     键值对信息，外层map的键为int，表示四种类别（0~4）；
 *                     内层map的键为QString，表示属性值，内层map的值为int的集合，表示属性值相同的对象的编号
 */
void Analysis::updateAnalysis(QMap<int, QMap<QString, QList<int>> > obj_attr_maps) {

    // 如果检测的数据在智能分析中已经使用了，则不用再更新界面
    if (detector->isUsed) return;

    detector->isUsed = true;  // 设置检测的数据已经在智能分析中使用

    //QList<QList<float> > class_info_list = detector->getClass_info_list();

    QPixmap map = detector->ship_drawed_map;
    map = map.scaled(result_img_label->size());
    result_img_label->setPixmap(map);

    anal_list_widget->clear();

    qDebug() << "items count0 : " << anal_list_widget->count();

    int i = 0;
    for(int i = 0; i < 4; i++) {
        QMap<QString, QList<int>> obj_attr_map = obj_attr_maps.value(i);
        foreach(QString attr, obj_attr_map.keys()) {
            QList<int> indexs = obj_attr_map.value(attr);
            QMap<QString,QString> attrs = convertJsonToMap(attr);
            addItemOfListWidget(anal_list_widget, indexs, attrs, i);
        }
    }
    qDebug() << "items count1 : " << anal_list_widget->count();
}

// 向anal_list_widget中添加条目
void Analysis::addItemOfListWidget(QListWidget *list_widget, QList<int> indexs, QMap<QString,QString> attrs, int label_id) {

    // table view 的数据适配器
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(2);
    model->setHeaderData(0,Qt::Horizontal,codec->toUnicode("属性名"));
    model->setHeaderData(1,Qt::Horizontal,codec->toUnicode("属性值"));

    int index = 0;
    foreach(QString key, attrs.keys()) {
        QList<QStandardItem *> item;
        item.append(new QStandardItem(key));
        item.append(new QStandardItem(attrs.value(key)));
        model->appendRow(item);
        model->item(index,0)->setTextAlignment(Qt::AlignCenter);
        model->item(index++,1)->setTextAlignment(Qt::AlignCenter);
    }

    //int ls_width = list_widget->size().width();
    //qDebug() << "anal_right_width : " << anal_right_width;
    //qDebug() << " list_widget->size().width() : " <<  list_widget->size().width();

    // 创建 table view
    QTableView * table_view = new QTableView();
    table_view->setFixedWidth(anal_right_width-30);
    table_view->setModel(model);
    table_view->setColumnWidth(0,(anal_right_width-40)/2);
    table_view->setColumnWidth(1,(anal_right_width-40)/2);
    table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);   // 不可编辑

    //qDebug() << " table_view->size().width() : " <<  table_view->size().width();
    QLabel *label = new QLabel();
    label->setMaximumWidth(anal_right_width-30);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignTop);
    label->setStyleSheet("font-family:KaiTi_GB2312;"
                                "font-size:18px;");
    QString index_num = codec->toUnicode(SHIP_LABEL[label_id].data()) + " : " + QString::number(indexs.at(0));
    for(int i = 1; i < indexs.size(); i++) {
        //qDebug() << "info : " << info;
        index_num = index_num + " , " + QString::number(indexs.at(i));
    }
    label->setText(index_num);

    QVBoxLayout *vlayout = new QVBoxLayout(list_widget);
    vlayout->addWidget(label);
    //vlayout->addSpacing(10);
    vlayout->addWidget(table_view);

    QWidget * content = new QWidget(list_widget);
    content->setLayout(vlayout);
    content->setFixedWidth(anal_right_width-20); //list_widget->size().width()

    QListWidgetItem *item = new QListWidgetItem(list_widget);
    item->setSizeHint(content->sizeHint());

    list_widget->setItemWidget(item,content);
    //qDebug() << "add one item...";

}

