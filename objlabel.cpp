#include "objlabel.h"

ObjLabel::ObjLabel(QSize fixed_size, QWidget *parent) : QWidget(parent)
{

    params = ConfigParams::instance();
    codec = QTextCodec::codecForName(params->CODE_NAME.data());

    this->fixed_size = fixed_size;
    int wg_width = fixed_size.width();
    int wg_height = fixed_size.height();

    this->setFixedSize(fixed_size);
    this->setStyleSheet("background:white");

    int right_width = (wg_width - 30)/4;           // 400
    int left_width = wg_width - 30 - right_width;  // 1200
    //int img_height = img_width * 2 / 3;          // 800

    obj_img_width = (left_width-80)/4;             // ÿ��С����ͼƬ�Ŀ�,80ָ����ͼƬ֮��ļ����һ��4��ͼƬ��3�������ÿ�����20


    all_obj_lb = new ClickableLabel(codec->toUnicode("����ͼ��"));
    labeled_obj_lb  = new ClickableLabel(codec->toUnicode("�ѱ�עͼ��"));
    nolabeled_obj_lb  = new ClickableLabel(codec->toUnicode("δ��עͼ��"));

    labeled_lbs.append(all_obj_lb);
    labeled_lbs.append(labeled_obj_lb);
    labeled_lbs.append(nolabeled_obj_lb);

    foreach(ClickableLabel* lb , labeled_lbs){
        lb->setFixedSize(150,30);
        lb->setAlignment(Qt::AlignCenter);
        lb->setStyleSheet("font-size:20px;"
                                "color:#5b5bd7");
        connect(lb,SIGNAL(clicked()),this,SLOT(on_labeled_clicked()));
    }
    labeled_lbs.at(0)->setStyleSheet("font-size:20px;"
                                "background-color:#5b5bd7;"
                                "color:white");


    obj_img_list_widget = new QListWidget();
    //obj_img_list_widget->setFixedSize(1200, 930);          // 1660 = 1200 + 430 + 30
    obj_img_list_widget->setFixedWidth(left_width);
    obj_img_list_widget->setStyleSheet("background:white");


    save_btn = new QPushButton(codec->toUnicode("����"));
    add_attr_btn = new QPushButton(codec->toUnicode("��������ֵ"));
    save_btn->setFixedHeight(40);
    add_attr_btn->setFixedHeight(40);
    connect(save_btn,SIGNAL(clicked()),this,SLOT(on_obj_save_btn_clicked()));
    connect(add_attr_btn,SIGNAL(clicked()),this,SLOT(on_obj_add_attr_btn_clicked()));

    obj_img_name_label = new QLabel("");

    // table view ������������
    obj_info_model = new QStandardItemModel();
    obj_info_model->setColumnCount(2);
    obj_info_model->setHeaderData(0,Qt::Horizontal,codec->toUnicode("������"));
    obj_info_model->setHeaderData(1,Qt::Horizontal,codec->toUnicode("����ֵ"));


    // ���� table view
    obj_info_table_view = new QTableView();
    obj_info_table_view->setFixedWidth(right_width-10);
    obj_info_table_view->setModel(obj_info_model);
    obj_info_table_view->setColumnWidth(0, (right_width-20)/2);
    obj_info_table_view->setColumnWidth(1, (right_width-20)/2);

    QHBoxLayout *btn_layout = new QHBoxLayout();
    btn_layout->setMargin(0);  // ���ò��ֵ���Χ�ľ���Ϊ��
    btn_layout->setSpacing(0); //���ò����ڵĸ��ؼ�֮��Ĭ�Ͼ���Ϊ0
    btn_layout->setContentsMargins(0,0,0,0); //���ò����ڵĿؼ�����߽�ľ���left��top��right��bottom
    btn_layout->addWidget(save_btn);
    btn_layout->addSpacing(20);
    btn_layout->addWidget(add_attr_btn);


    QHBoxLayout *head_layout = new QHBoxLayout();
    head_layout->setMargin(0);  // ���ò��ֵ���Χ�ľ���Ϊ��
    head_layout->setSpacing(0); //���ò����ڵĸ��ؼ�֮��Ĭ�Ͼ���Ϊ0
    head_layout->setContentsMargins(0,0,0,0); //���ò����ڵĿؼ�����߽�ľ���left��top��right��bottom
    head_layout->addWidget(all_obj_lb);
    head_layout->addWidget(labeled_obj_lb);
    head_layout->addWidget(nolabeled_obj_lb);
    head_layout->addStretch();


    QVBoxLayout *left_layout = new QVBoxLayout();
    left_layout->setMargin(0);  // ���ò��ֵ���Χ�ľ���Ϊ��
    left_layout->setSpacing(0); //���ò����ڵĸ��ؼ�֮��Ĭ�Ͼ���Ϊ0
    left_layout->setContentsMargins(0,0,0,0); //���ò����ڵĿؼ�����߽�ľ���left��top��right��bottom
    left_layout->addLayout(head_layout);
    left_layout->addWidget(obj_img_list_widget);

    QWidget *left_widget = new QWidget();
    //left_widget->setFixedSize(left_width,wg_height);
    left_widget->setStyleSheet("background:white");
    left_widget->setLayout(left_layout);


    QVBoxLayout *right_layout = new QVBoxLayout();
    right_layout->setMargin(0);  // ���ò��ֵ���Χ�ľ���Ϊ��
    right_layout->setSpacing(0); //���ò����ڵĸ��ؼ�֮��Ĭ�Ͼ���Ϊ0
    right_layout->setContentsMargins(0,0,0,0); //���ò����ڵĿؼ�����߽�ľ���left��top��right��bottom
    right_layout->addLayout(btn_layout);
    right_layout->addSpacing(10);
    right_layout->addWidget(obj_img_name_label,0,Qt::AlignHCenter);
    right_layout->addSpacing(10);
    right_layout->addWidget(obj_info_table_view,0,Qt::AlignHCenter);


    QWidget *right_widget = new QWidget();
   // right_widget->setFixedSize(right_width,wg_height);
    right_widget->setStyleSheet("background:white");
    right_widget->setLayout(right_layout);

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setMargin(0);  // ���ò��ֵ���Χ�ľ���Ϊ��
    layout->setSpacing(0); //���ò����ڵĸ��ؼ�֮��Ĭ�Ͼ���Ϊ0
    layout->setContentsMargins(0,0,0,0); //���ò����ڵĿؼ�����߽�ľ���left��top��right��bottom

    layout->addWidget(left_widget);
    layout->addSpacing(30);
    layout->addWidget(right_widget);

    this->setLayout(layout);
}

ObjLabel::~ObjLabel()
{

}


// ���¶���������ע����
void ObjLabel::setFixedSize(QSize fixed_size)
{
    this->fixed_size  =fixed_size;
}

void ObjLabel::showObjLable()
{
    obj_infos.clear();
    foreach(ClickableLabel* lb , labeled_lbs){
        lb->setStyleSheet("font-size:20px;"
                            "color:#5b5bd7");
    }
    labeled_lbs.at(0)->setStyleSheet("font-size:20px;"
                            "background-color:#5b5bd7;"
                            "color:white");
    obj_infos = dbhelper.getAllInfos();
    updateObjLabel(obj_infos);
}

void ObjLabel::setDetectorThread(DetectThread *detector)
{
    this->detector = detector;
}

void ObjLabel::updateObjLabel(QList<img_info> obj_infos) {

    obj_img_list_widget->clear();
    obj_info_model->removeRows(0,obj_info_model->rowCount());

    obj_img_name_label->setText("");
    obj_img_current_row = -1;
    obj_img_current_index = -1;

    obj_img_last_click_label = nullptr;

    qDebug() << "all objs size is : " << obj_infos.count();

    int rows = ceil(float(obj_infos.count())/float(per_row_count));

    for (int row = 0; row < rows; row++) {

        //qDebug() << "row : " << row;
        addItemOfObjImgListWidget(obj_img_list_widget, row);

    }
}


void ObjLabel::addItemOfObjImgListWidget(QListWidget *list_widget, int row) {


    int info_index = per_row_count * row;

    //qDebug() << "row : " << row << ", info_index : " << info_index;

    QHBoxLayout *hlayout = new QHBoxLayout(list_widget);
    hlayout->addWidget(getOneImageWidget(row,0));

    if (info_index + 1 < obj_infos.count()) {
        hlayout->addSpacing(20);
        hlayout->addWidget(getOneImageWidget(row,1));
        if (info_index + 2 < obj_infos.count()) {
            hlayout->addSpacing(20);
            hlayout->addWidget(getOneImageWidget(row,2));
            if (info_index + 3 < obj_infos.count()) {
                hlayout->addSpacing(20);
                hlayout->addWidget(getOneImageWidget(row,3));
            }
        }
    }


    QWidget * content = new QWidget(list_widget);
    content->setLayout(hlayout);
    content->setFixedWidth(list_widget->size().width());

    QListWidgetItem *item = new QListWidgetItem(list_widget);
    item->setSizeHint(content->sizeHint());

    list_widget->setItemWidget(item,content);
    //qDebug() << "add one item...";
}

// row and index from 0 to ...
// ������Ż�ȡ��ʾ��ͼƬ��Ϣ
QWidget* ObjLabel::getOneImageWidget(int row, int index) {

    ClickableLabel *img_label = new ClickableLabel("obj_img");
    img_label->setFixedSize(obj_img_width, obj_img_width/2);
    // objectName ����Ϊ�кź���� row , index
    img_label->setObjectName(QString::number(row) + "," + QString::number(index));
    connect(img_label,SIGNAL(clicked()),this,SLOT(on_obj_img_clicked()));



    img_info info = obj_infos.at(per_row_count * row + index);

    QPixmap map(info.img_name);
    map = map.scaled(img_label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    img_label->setPixmap(map);

    QLabel *name_label = new QLabel("img_name");
    QString name = info.img_name.split('/').last();
    name_label->setText(name);

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->addWidget(img_label);
    vlayout->addSpacing(5);
    vlayout->addWidget(name_label,0,Qt::AlignHCenter);

    QWidget *content = new QWidget();
    content->setLayout(vlayout);
    content->setFixedWidth(obj_img_width);
    content->setFixedHeight(obj_img_width/2 + 50);


    //qDebug() << "row : " << row << ", info_index : " << per_row_count * row + index;
    //qDebug() << "make one widget and widget size : " << content->size();

    return content;

}

// �����ע��Ϣ
void ObjLabel::on_obj_save_btn_clicked() {

    if (obj_img_current_row == -1 || obj_img_current_index == -1) {
        QMessageBox::warning(this,codec->toUnicode("����"),codec->toUnicode("��ѡ��һ��ͼƬ�ٱ����ע��Ϣ..."));
        return ;
    }

    int rows = obj_info_model->rowCount();

    QString attr_name = "";
    QString attr_value = "";
    QJsonObject json;

    for (int i = 0; i < rows; i++) {

        attr_name = obj_info_model->data(obj_info_model->index(i,0)).toString();
        attr_value = obj_info_model->data(obj_info_model->index(i,1)).toString();
        if (attr_name.trimmed() == "") continue;
        json.insert(attr_name,attr_value);

        qDebug() << "attr_name : " << attr_name << " , attr_value : " << attr_value;
    }

    QJsonDocument doc;
    doc.setObject(json);
    QByteArray bt = doc.toJson(QJsonDocument::Compact);
    QString strJson(bt);

    if (!dbhelper.updateObjInfo(strJson, obj_infos.at(obj_img_current_row * per_row_count + obj_img_current_index).id)) {
        QMessageBox::warning(this,codec->toUnicode("����"),codec->toUnicode("�޸�ʧ�ܣ����Ժ�����..."));
    } else {
        QMessageBox::warning(this,codec->toUnicode("��ʾ"),codec->toUnicode("�޸ĳɹ���"));
        img_info info = obj_infos.takeAt(obj_img_current_row * per_row_count + obj_img_current_index);
        info.attr = strJson;
        obj_infos.insert(obj_img_current_row * per_row_count + obj_img_current_index, info);
        //obj_infos.clear();
        //obj_infos = dbhelper.getAllInfos();
        if (detector != nullptr)
            detector->updateAllLabeledObjImgs();
    }

}

// ����������Ϣ
void ObjLabel::on_obj_add_attr_btn_clicked() {

    if (obj_img_current_row == -1 || obj_img_current_index == -1) {
        QMessageBox::warning(this,codec->toUnicode("����"),codec->toUnicode("��ѡ��һ��ͼƬ�ٱ����ע��Ϣ..."));
        return ;
    }

    int rows = obj_info_model->rowCount();
    qDebug() << "rows is : " << rows;

    obj_info_model->setItem(rows,0,new QStandardItem(""));
    obj_info_model->setItem(rows,1,new QStandardItem(""));
    obj_info_model->item(rows,0)->setTextAlignment(Qt::AlignCenter);
    obj_info_model->item(rows,1)->setTextAlignment(Qt::AlignCenter);
    obj_info_table_view->update();

}


void ObjLabel::on_labeled_clicked()
{
    ClickableLabel *label = (ClickableLabel*)sender();
    QString name = label->text();

    obj_infos.clear();
    foreach(ClickableLabel* lb , labeled_lbs){
        lb->setStyleSheet("font-size:20px;"
                                "color:#5b5bd7");
    }
    label->setStyleSheet("font-size:20px;"
                                "background-color:#5b5bd7;"
                                "color:white");

    if (name == codec->toUnicode("����ͼ��")) {
        obj_infos = dbhelper.getAllInfos();
    } else if (name == codec->toUnicode("�ѱ�עͼ��")) {
        obj_infos = dbhelper.getInfosByTag(true);
    } else if (name == codec->toUnicode("δ��עͼ��")) {
        obj_infos = dbhelper.getInfosByTag(false);
    }
    updateObjLabel(obj_infos);
}

void ObjLabel::on_obj_img_clicked()
{

    ClickableLabel *label = (ClickableLabel*)sender();
    QString objectName = label->objectName();
    int index = atoi(objectName.split(',').at(1).toStdString().c_str());        // �� label �� objectName �л�ȡ��� index
    int row = atoi(objectName.split(',').at(0).toStdString().c_str());          // �� label �� objectName �л�ȡ�к� row

    obj_img_current_row = row;
    obj_img_current_index = index;

    //qDebug() << "label->text() : " << label->text() << ", label->objectName() : " << label->objectName();
    //qDebug() << "row : " << row << ", info_index : " << index;

    // ��ȡ��Ӧ��������Ϣ
    img_info info = obj_infos.at(per_row_count * row + index);

    obj_img_name_label->setText(info.img_name.split('/').last() + "   " + codec->toUnicode(SHIP_LABEL[info.label-1].data()));

    QString attr = info.attr;
    QMap<QString,QString> maps;  // ���������ļ�ֵ��

    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(attr.toUtf8(), &jsonError);
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // ����δ��������
        if (doucment.isObject()) { // JSON �ĵ�Ϊ����
            QJsonObject object = doucment.object();  // ת��Ϊ����
            foreach(QString key, object.keys()) {
                maps.insert(key,object.value(key).toString());
            }
           // qDebug() << "result : " << maps;
        }
    }

    obj_info_model->removeRows(0,obj_info_model->rowCount());

    int num = 0;
    foreach(QString key, maps.keys()) {
        QList<QStandardItem *> item;
        item.append(new QStandardItem(key));
        item.append(new QStandardItem(maps.value(key)));
        obj_info_model->appendRow(item);
        obj_info_model->item(num,0)->setTextAlignment(Qt::AlignCenter);
        obj_info_model->item(num++,1)->setTextAlignment(Qt::AlignCenter);
    }

    if (obj_img_last_click_label != nullptr) {
        obj_img_last_click_label->setStyleSheet("border-width: 0px;");
    }

    QList<ClickableLabel*> labelList = obj_img_list_widget->findChildren<ClickableLabel*>();
    for(int i = 0; i < labelList.size(); i++)
    {
        ClickableLabel* label = labelList.at(i);
        if(label->objectName() == objectName) {
            label->setStyleSheet("border-width: 3px; border-style:solid; border-color: rgb(255, 0, 0);"); // rgb(255, 177, 0)
            obj_img_last_click_label = label;
            break;
        }
    }
}


// ���¶����ע����Ĵ�С
void ObjLabel::updateObjLabelWidget()
{

    //qDebug() << "size : " << fixed_size;
    int wg_width = fixed_size.width();
    int wg_height = fixed_size.height();
    this->setFixedHeight(wg_height);

    int right_width = (wg_width - 30)/4;           // 400
    int left_width = wg_width - 30 - right_width;  // 1200
    //int img_height = img_width * 2 / 3;          // 800

    obj_img_width = (left_width-80)/4;             // ÿ��С����ͼƬ�Ŀ�,80ָ����ͼƬ֮��ļ����һ��4��ͼƬ��3�������ÿ�����20

    obj_img_list_widget->setFixedWidth(left_width);

    obj_info_table_view->setFixedWidth(right_width-10);
    obj_info_table_view->setColumnWidth(0, (right_width-20)/2);
    obj_info_table_view->setColumnWidth(1, (right_width-20)/2);

    qDebug() << "change object label widget ...";
    updateObjLabel(obj_infos);

}
