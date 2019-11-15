#include "detectthread.h"
#include <QDebug>
#include <QProcess>
#include <complex.h> 

#include <QTime> 

#define PI 3.1415926

using namespace std;

DetectThread::DetectThread(QObject *parent,QProgressDialog *progress, QMessageBox * message_box):progress(progress), message_box(message_box)
{
    //qDebug() << "detect thread id is : " << QThread::currentThreadId();
    params = ConfigParams::instance();
    codec = QTextCodec::codecForName(params->CODE_NAME.data());
    currentValue = 0;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(valueChanged()));
    connect(this,SIGNAL(timerStart()),this,SLOT(startTimer()));       // 在线程中启动计时器，只能用信号槽的方式启动，不能直接启动
    connect(this,SIGNAL(updateProgress()),this,SLOT(valueChanged()));
    connect(this,SIGNAL(detectOver()),this,SLOT(updateDetectOver()));
    
    qRegisterMetaType<std::string>("std::string");
    connect(this,SIGNAL(error(int, std::string)),this,SLOT(errorMsg(int ,std::string)));
  
    label_colors.append(Qt::red);
    label_colors.append(Qt::green);
    label_colors.append(Qt::cyan);
    label_colors.append(Qt::magenta);
    label_colors.append(Qt::yellow);
    label_colors.append(Qt::blue);
    label_colors.append(Qt::darkRed);
    label_colors.append(Qt::darkGreen);
    label_colors.append(Qt::darkBlue);
    label_colors.append(Qt::darkCyan);
    label_colors.append(Qt::darkMagenta);
    label_colors.append(Qt::darkYellow);
    label_colors.append(Qt::darkGray);
    label_colors.append(Qt::gray);
    label_colors.append(Qt::lightGray);
    label_colors.append(Qt::darkGreen);
    label_colors.append(Qt::darkCyan);
    label_colors.append(Qt::magenta);
    
    orb = ORB::create();
    updateAllLabeledObjImgs(); // 更新获取已经标注的对象信息
    
}


DetectThread::~DetectThread()
{
    qDebug() << "in ~DeteceThread and the thread is destroyed";
    if (progress != NULL) {
        delete progress;
        qDebug() << "release QProgress...";
    }
    if (message_box != NULL) {
        delete message_box;
        qDebug() << "release message_box...";
    }
    if (info_dialog != nullptr) {
        delete info_dialog;
        qDebug() << "release info_dialog...";
    }
}

void DetectThread::setInterfaceAndLabel(int interface_id,QLabel *label)
{
    // every time, only load one net in the memory,the other net should be released
    if (this->interface_id != interface_id) {
        this->interface_id = interface_id;
        this->image_label = label;
    }
}

void DetectThread::setImagePath(QString path)
{
    this->path = path;
}

void DetectThread::run()
{
 
     
     if (info_dialog != nullptr) {
         info_dialog->hide();
     }
        
    // 检测
    //qDebug() << "is detecting ...";
    emit timerStart();  // 启动计时器
    isDetect = true;
    currentValue = 0;
   
    // 创建上下文和套接字
    context = zmq_ctx_new();
    ReqSock = zmq_socket(context, ZMQ_DEALER);
    
    if (url == "") {
        url = params->SERVER;
        if (url == "" && !loadConfig(CONFIG_PATH)) {
            emit error(CONFIG_FILE_ERROR,codec->toUnicode("配置文件有误!").toStdString());
            return;
        }
        url = params->SERVER;
        std::cout << "url : " << url << endl;
    }
    
    /*std::cout << "SERVER : ' " << params->SERVER << " ' \n";
    std::cout << "params->CODE_NAME.data() : ' " << params->CODE_NAME.data() << " ' \n";
    std::cout << "INTERFACEID_YOLO : ' " << params->INTERFACEID_YOLO << " ' \n";
    std::cout << "INTERFACEID_SHIP : ' " << params->INTERFACEID_SHIP << " ' \n";
    std::cout << "INTERFACEID_SAR : ' " << params->INTERFACEID_SAR << " ' \n";*/
    
    int rc = zmq_connect(ReqSock,url.c_str());
    
    //std::cout << "connect result : " << rc << std::endl;
    
    if (rc == -1) {
        emit error(CONNECT_FAILED, codec->toUnicode("url连接失败...           ").toStdString());
        return;
    }
    
    //zmq_connect(ReqSock, "tcp://127.0.0.1:5559");  //localhost:5561
    
    QTime time;  
    time.start();  
    
    
    create_send(ReqSock, path.toLatin1().data(), interface_id);  // 发送图片信息
    create_client(ReqSock);                                      // 创建接收信息的客户端

    zmq_close(ReqSock);
    zmq_close(context);
    
    ReqSock = NULL;
    context = NULL;
   
    qDebug()<< "detect time : " << time.elapsed()/1000.0<<"s";
    
    emit detectOver();  // detect over and send a signal to draw bounding box
    
     // 检测结束，并且得到检测结果
    if(isDetect && interface_id == params->INTERFACEID_SHIP) {
        QImage original_img(path);
        
        obj_attr_maps.clear();
        
        int index = 1;
        for(int i = 0; i < detections.size(); i++) {
            const vector<float>& d = detections[i];
            if (d[1] < params->SHIP_THRESH)
                continue;
            cropObjectFromImage(original_img, d, index);
            index++;
        }
        
        
    }
    
}

void DetectThread::startTimer() {
    timer->start(500);    // 启动计时器
    progress->show();
    //qDebug() << "startTimer thread is:" << QThread::currentThreadId() ;
}

void DetectThread::updateDetectOver()
{
    qDebug() << "in updateDetectOver from the signal detectOver";
    if(isDetect) {
        isDetect = false; //妫�娴嬬粨鏉?        
        currentValue = 100;
        progress->setValue(currentValue); 
        currentValue = 0;
        //qDebug() << "test111";
        if(interface_id == params->INTERFACEID_YOLO) {
            normal_drawed_map = paintObject(detections,path);
        } else if (interface_id == params->INTERFACEID_SAR) {
            sar_drawed_map = paintObject(detections,path);
        } else if(interface_id == params->INTERFACEID_SHIP) {
            ship_drawed_map = paintObject(detections,path);
        }

    }
    timer->stop();
    progress->setHidden(true);
}

void DetectThread::cancel()
{    
    isDetect = false;
        
    if (ReqSock != NULL) {
        zmq_close(ReqSock);
        qDebug() << "delete reqSock ... ";
        ReqSock = NULL;
    }
    if (context != NULL) {
        zmq_close(context);
        qDebug() << "delete context ...";
        context = NULL;
    }
    
    timer->stop();
    qDebug() << "clicked canceled!!!";
    progress->setHidden(true);
}

// 进度条改变
void DetectThread::valueChanged()
{
    currentValue++;
    if(currentValue == 100){
        currentValue = 50;
    }
    //qDebug() << "current value : " << currentValue;
    if(currentValue >= timeout / 500) {
        // timeout 超时处理
        currentValue = 0;
        progress->setValue(currentValue);
        errorMsg(OUTTIME, codec->toUnicode("请求超时           ").toStdString());
    }
    progress->setValue(currentValue);
}

// creat client to receive response
void DetectThread::create_client(void *ReqSock)
{
    //cout << "create_client ... " << endl;
    zmq_pollitem_t items[] = {{ReqSock, 0, ZMQ_POLLIN, 0}};
    while (1) {
        {
            zmq_poll(items, 1, 10 * ZMQ_POLL_MSEC);
            //cout << "hbk in create_client : test1 ... " << endl; 
            if (items[0].revents & ZMQ_POLLIN)
            {
                zmsg_t *msg = zmsg_recv(ReqSock);
                zframe_t *cont = zmsg_pop(msg);
                //cout << "receive the response ... " << endl;                
                if (cont) {
                    char *result = zframe_strdup(cont);                
                    cout<<result<<endl;
                    int  response_id;       // 返回状态码
                    string error_msg = "";  // 返回错误信息
                    
                    // 解析返回的结果
                    detections = parseResult(result, response_id, error_msg);
                    
                    if (error_msg != "") {
                        error_msg = codec->toUnicode(error_msg.data()).toStdString();
                        emit error(response_id, error_msg);
                    }
                    //cout << "response_id is : " << response_id << endl;
                    //cout << "detections'size is : " << detections.size() << endl;
                    delete[] result;
                    result = nullptr;
                }
                zmsg_destroy(&msg);
                break;
            }
            // 超时处理
            if (!isDetect) break;
        }
    }
}

// 发送图片
void DetectThread::create_send(void *ReqSock, char * path, int msg_type) {
    /*-------------------------------璇诲浘锟?-------------------------------------*/
    cout << "create_send ... " << endl;
    FILE *pFile;
    long lSize;
    char *buffer;
    size_t result;
    
    pFile = fopen(path, "rb");
    if (pFile == NULL)
        fputs("File error", stderr);
        
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);
    buffer = new char[lSize];
    if (buffer == NULL)
        fputs("Memory error", stderr);
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize)
        fputs("Reading error", stderr);
    /*----------------------------base64编码------------------------------------*/
    string imgBase64 = dev::base64_encode(buffer, lSize);
    /*------------------------------Json打包-------------------------------------*/
    
    QJsonObject object;
    object.insert("api_key","");
    object.insert("interface",msg_type);
    object.insert("image_base64",QString::fromStdString(imgBase64));
    std::string tmpstr = QString(QJsonDocument(object).toJson()).toStdString();
    
    //Json::Value root;
    //root["api_key"] = "";
    //root["interface"] = msg_type;
    //root["image_base64"] = imgBase64;
    //std::string tmpstr = root.toStyledString();

    /*------------------------------ 发数据 ------------------------------------*/

    zstr_sendf(ReqSock, tmpstr.c_str());
    delete[] buffer;
    buffer = nullptr;
    
    //cout << "hbk in create_send : test4 loading over ... " << endl;
    
}

// 错误信息提示
void DetectThread::errorMsg(int response_id, std::string error_msg) {
    cancel();
    message_box->setText(QString::fromStdString("type : " + std::to_string(response_id) + "\n" + error_msg));
    message_box->show();
    
}


// format:[label , confidence, xmin, ymin, xmax, ymax ]      the first label index is 1
QPixmap DetectThread::paintObject(std::vector<std::vector<float> > detections, QString path)
{
    //std::cout << "size1 : " << YOLO_LABEL->size() << ", size2 : " << SAR_LABEL->size() << ", size3 : " << SHIP_LABEL->size() << std::endl;
    
    float thresh = 0.5;
    
    // 获取 label 信息
    if ( interface_id == params->INTERFACEID_YOLO ) {
        thresh = params->YOLO_THRESH;
        for (int i = 0; i < 80; ++i)
            LABEL[i] = YOLO_LABEL[i];
    } else if ( interface_id == params->INTERFACEID_SHIP ) {
        for (int i = 0; i < SHIP_LABEL->size(); ++i)
            LABEL[i] = SHIP_LABEL[i];
    } else if ( interface_id == params->INTERFACEID_SAR ) {
        thresh = params->SAR_THRESH;
        for (int i = 0; i < SAR_LABEL->size(); ++i)
            LABEL[i] = SAR_LABEL[i];
    }
    
    if(interface_id == params->INTERFACEID_SHIP)
    {
        qDebug() << "paint ship ... ";
        // 绘制有角度的边界框
        return paintRObject(detections,path);
    }
    
    QPixmap drawed_map = loadPixImage(path);  // Init the original image
    // get the scale between image_label with original image
    double wid = (double)image_label->width()/(double)drawed_map.width();
    double hid = (double)image_label->height()/(double)drawed_map.height();
    
    // change the original image's scale to the image_label's scale.
    drawed_map = drawed_map.scaled(image_label->size());
    
    Qt::GlobalColor color;
    QPainter painter(&drawed_map);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    painter.setFont(QFont("Arial",15,QFont::Bold,true));
    
    QFontMetrics label_fmt(QFont("Arial",15,QFont::Bold)); // 字体的样式
    
    qDebug() << "detect number is " << detections.size();
    qDebug() << "draw thresh is " << thresh;
    
    int index = 1;
    
    for(int i = 0; i < detections.size(); i++) {
        const vector<float>& d = detections[i];
        if(d[1] > thresh) {         
            
            // 获取颜色并设置画笔的颜色
            color = label_colors.at(((int)d[0]-1) % 18);
            pen.setColor(color);
            painter.setPen(pen);
            
            // change the result boxes' scale to the image_label's scale
            int x = d[2] * wid;
            int y = d[3] * hid;
            int w = (d[4] - d[2]) * wid;
            int h = (d[5] - d[3]) * hid;
            painter.drawRect(x,y,w,h);  // draw the box of the object
               
            // class label and the confidence
            QString label_conf = QString::number(index++) + "   " + codec->toUnicode(LABEL[(int)d[0]-1].data()) + "   " + QString::number(d[1]);
            //QString label_conf = codec->toUnicode(LABEL[(int)d[0]-1].data()) + "   " + QString::number(d[1]);
            
            // draw the background of the label
            QRect rect(x-1,y-25,label_fmt.width(label_conf)+5,25);
            painter.fillRect(rect,QBrush(color));
            
            // draw text of the class label and the confidence
            painter.setPen(QColor(Qt::white));
            painter.drawText(x+2,y-5,label_conf);   

        } 
    }

    image_label->setPixmap(drawed_map);
    return drawed_map;

}
// format : [ label score x1 y1 x2 y2 x3 y3 x4 y4 ]  ( the first label is 1 )
QPixmap DetectThread::paintRObject(std::vector<std::vector<float> > detections, QString path) {
    
    qDebug() << "start test paint..."; 
    
    isUsed = false;   // 获得新结果，该结果暂未被智能分析
    
    QPixmap drawed_map = loadPixImage(path);  // Init the original image
    
    // get the scale between image_label with original image
    double wid = (double)image_label->width()/(double)drawed_map.width();
    double hid = (double)image_label->height()/(double)drawed_map.height();
    
    // change the original image's scale to the image_label's scale.
    drawed_map = drawed_map.scaled(image_label->size());
    
    Qt::GlobalColor color;
    QPainter painter(&drawed_map);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    painter.setFont(QFont("Arial",15,QFont::Bold,true));
    QFontMetrics label_fmt(QFont("Arial",15,QFont::Bold)); // 字体的样式
    
    qDebug() << "detect number is " << detections.size();
    
    qDebug() << "ship thresh : " << params->SHIP_THRESH;
    
    
    //QImage original_img(path);
    int index = 1;
    
    for(int i = 0; i < detections.size(); i++) {
        const vector<float>& d = detections[i];
        
        if (d[1] < params->SHIP_THRESH)
            continue;
        
        // 获取颜色并设置画笔的颜色
        color = label_colors.at(((int)d[0]-1) % 18);
        pen.setColor(color);
        painter.setPen(pen);
        
        painter.drawLine(d[2]*wid, d[3]*hid, d[4]*wid, d[5]*hid);
        painter.drawLine(d[4]*wid, d[5]*hid, d[6]*wid, d[7]*hid);
        painter.drawLine(d[6]*wid, d[7]*hid, d[8]*wid, d[9]*hid);
        painter.drawLine(d[8]*wid, d[9]*hid, d[2]*wid, d[3]*hid);  
              
        // class label and the confidence
        QString label_conf = " " + QString::number(index++) + "   " + codec->toUnicode(LABEL[(int)d[0]-1].data()) + "   " + QString::number(d[1]);
        //QString label_conf = codec->toUnicode(LABEL[(int)d[0]-1].data()) + "   " + QString::number(d[1]);
            
        int cx = (d[2] + d[6]) / 2;
        int cy = (d[3] + d[7]) / 2;
        // draw the background of the label
        QRect rect(cx*wid,cy*hid,label_fmt.width(label_conf)+5,25);
        painter.fillRect(rect,QBrush(color));
            
        // draw text of the class label and the confidence
        painter.setPen(QColor(Qt::white));
        painter.drawText(cx*wid,cy*hid+18,label_conf);  
        
        //index ++;
           
    }
    image_label->setPixmap(drawed_map);
    return drawed_map;
}

// 获取检测得到的信息统计
QMap<int, QMap<QString, QList<int>>> DetectThread::getObjAttrMaps() {
    return obj_attr_maps;
}

void DetectThread::cropObjectFromImage(QImage original_img, std::vector<float> detection, int index) {
    
    bool isTrans = false;
    float xmax, ymax, xmin, ymin;
    
    xmin = original_img.size().width() + 100;
    ymin = original_img.size().height() + 100;
    xmax = -1;
    ymax = -1;
    
    
    std::vector<float> d(8);
    for (int i = 0; i < 8; i++) {
        d[i] = detection[i+2];
        if (i % 2 == 0) {
            if (d[i] > xmax) xmax = d[i];
            if (d[i] < xmin) xmin = d[i];
        } else {
            if (d[i] > ymax) ymax = d[i];
            if (d[i] < ymin) ymin = d[i];
        }
    }
    
    if (xmin < 0) xmin = 0;
    if (ymin < 0) ymin = 0;
    if (xmax > original_img.size().width()) xmax = original_img.size().width();
    if (ymax > original_img.size().height()) ymax = original_img.size().height();
    
    float w = xmax - xmin;
    float h = ymax - ymin;
    
    float tmpw = w, tmph = h;
    
    if (h > w) {
        isTrans = true;
        tmpw = h;
        tmph = w;
    }
    
    QImage save_img(tmpw,tmph,QImage::Format_RGB32);
    
    /*
    qDebug() << "x1 : " << d[0] << "y1 : " << d[1] << "x2 : " << d[2] << "y2 : " << d[3] << "x3 : " << d[4] << "y3 : " << d[5] << "x4 : " << d[6] << "y4 : " << d[7];
    qDebug () << "xmin is : " << xmin  << "ymin is : " << ymin << "xmax is : " << xmax << "ymax is : " << ymax;
    qDebug () << "w is : " << w  << "h is : " << h;
    qDebug () << "save image size is : " << save_img.size();*/
    
    // 一行一行计算,每次需要计算第col行的第一个坐标 xw,yw
    int x,y;
    for (int row = 0; row < h; row++) {
            y = int(ymin + row);
        for (int col = 0; col < w; col++) {
            x = int(xmin + col);
            
            // 防止超过边界
            /*
            if (x < 0) x = 0;
            if (y < 0) y = 0;
            if (x > original_img.size().width()) x = original_img.size().width();
            if (y > original_img.size().height()) y = original_img.size().height();
            */
            
            //if (x < 0 || y < 0 || x > original_img.size().width() || y > original_img.size().height()) continue;
            
            if (isTrans) 
                save_img.setPixel(row,col,original_img.pixel(x,y));
            else
                save_img.setPixel(col,row,original_img.pixel(x,y));
        }
    }
    
    QDir dir;
    if(!dir.exists("result")) {
        dir.mkdir("result");
        dir.mkdir("result/object_img");
        dir.mkdir("result/object_img/warship");
        dir.mkdir("result/object_img/civil");
        dir.mkdir("result/object_img/submarine");
        dir.mkdir("result/object_img/aircraft_carrier");
    } else if(!dir.exists("result/object_img")){   
        dir.mkdir("result/object_img");     
        dir.mkdir("result/object_img/warship");
        dir.mkdir("result/object_img/civil");
        dir.mkdir("result/object_img/submarine");
        dir.mkdir("result/object_img/aircraft_carrier");
    } else {
        if(!dir.exists("result/object_img/warship"))
            dir.mkdir("result/object_img/warship");
        if(!dir.exists("result/object_img/civil"))
            dir.mkdir("result/object_img/civil");
        if(!dir.exists("result/object_img/submarine"))
            dir.mkdir("result/object_img/submarine");
        if(!dir.exists("result/object_img/aircraft_carrier"))
            dir.mkdir("result/object_img/aircraft_carrier");
    }
    
    QString class_name = "";
    switch(int(detection[0])) {
    case 1:
        class_name = "warship/";
        break;
    case 2:
        class_name = "civil/";
        break;
    case 3:
        class_name = "submarine/";
        break;
    case 4:
        class_name = "aircraft_carrier/";
        break;
    }

    QString image_name =  path.split('/').last().split('.').first();
    image_name = image_name + "_" + QString::number(index) + ".jpg";
    QString save_image_path = "./result/object_img/" + class_name + image_name;
    //qDebug() << "save image name is : " << save_image_path;
    save_img.save(save_image_path);
    
    img_info info;
    info.id = dbhelper.getMaxID() + 1;
    info.img_name = save_image_path;
    info.islabeled = false;
    info.label = detection[0];
    info.original_img = path;
    info.attr = "";
        
    // 说明数据库中不存在该对象的信息
    if (dbhelper.findObjByImgName(info.img_name) == 0) {
        if(!dbhelper.insertObjInfo(info)) {
            message_box->setText(codec->toUnicode("插入数据库失败 ..."));
            message_box->show();
        } else {
            //qDebug() << "save image success....";
        }
    }
    
    addImgAttrToMap(save_image_path, detection[0], index);

}

void DetectThread::addImgAttrToMap(QString img_path, int label, int index) {
    
    QList<img_info> img_infos = all_labeled_obj_imgs.at(label-1); // 获取对应类别的已经标注的数据集
    
    QMap<QString, QList<int>> obj_attr_map = obj_attr_maps.value(label-1);
    
    float min_similar_distance = 99999999;
    QString attr;   // 该对象的属性值
    
    foreach(img_info info, img_infos) {
        float similar_distance = cacuDistance(img_path, info.img_name);  // 计算两张图片的距离
        //qDebug() << "current distance : " << similar_distance;
        // 根据距离来求图片的最大相似图像属性
        if (similar_distance < min_similar_distance) {
            min_similar_distance = similar_distance;
            attr = info.attr;
        }        
    }
    QList<int> indexs = obj_attr_map.value(attr);
    indexs.append(index);
    obj_attr_map.insert(attr,indexs);
    
    obj_attr_maps.insert(label-1,obj_attr_map); // 向类别为label添加所有对象属性
}

// 计算两张图片的相似关键点的个数
float DetectThread::cacuDistance(QString img_path1, QString img_path2) {
    
    //qDebug() << "\n\n\npath1 : " << img_path1 << " , path2 : " << img_path2;
    
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    
    //qDebug() << "test1 : ";
    
    Mat rgbd1 = imread(img_path1.toStdString());
    Mat rgbd2 = imread(img_path2.toStdString());
    //qDebug() << "test2 : ";
    
    orb->detectAndCompute(rgbd1, Mat(), keypoints1, descriptors1);
	  orb->detectAndCompute(rgbd2, Mat(), keypoints2, descriptors2);
    //qDebug() << "test3: ";
     
    //std::cout << "keypoints1 " << keypoints1.size() << " \n";
    //std::cout << "keypoints2 " << keypoints2.size() << " \n";
    
    
    //Matching
  	//std::vector<DMatch> matches;  // match()
    std::vector<std::vector<DMatch> > matches;  // knnMatch()
  	Ptr<DescriptorMatcher> matcher =DescriptorMatcher::create("BruteForce");
    //qDebug() << "test4 : ";
    try{
 	    matcher->knnMatch(descriptors1, descriptors2, matches,1);
    } catch(cv::Exception e) {
      //qDebug() << "test5: ";
    }
    
    float distances = 0;
    //qDebug() << "test6 : ";
    for(int i = 0; i < matches.size(); i++) {
        distances += matches[i][0].distance;
        //cout << match.size() << " ' " << m.distance << "\n";
    }
    
    //qDebug() << "test7 : \n\n";
    //std::cout << "find out total " << matches.size() << " matches\n";
    //std::cout << "all distances : " << distances << "\n";
   
    return matches.size() != 0 ? distances/matches.size() : 0;
}

// 更新获取已经标注的对象信息
void DetectThread::updateAllLabeledObjImgs() {
    all_labeled_obj_imgs.clear();
    // 一共四种类别
    for(int i = 1; i <= 4; i++) {
        QList<img_info> obj_img_info = dbhelper.getInfosByTags(i, true);
        qDebug() << "class " << i << " labeled number is : " << obj_img_info.size();
        all_labeled_obj_imgs.append(obj_img_info);
    }
}

// 得到的结果 w > h，并转换 d 中的顺序使 (d[0],d[1]) 与 (d[2],d[3]) 之间距离为 w
void DetectThread::computerWH(std::vector<float> &d, float &h, float &w) {

    float x1 = d[0];
    float y1 = d[1];
    float x2 = d[2];
    float y2 = d[3];
    float x3 = d[4];
    float y3 = d[5];

    w = (float) sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
    h = (float) sqrt((x2-x3)*(x2-x3) + (y2-y3)*(y2-y3));

    float tmp;
    // 规定 w > h
    if (h > w) {
        tmp = w;
        w = h;
        h = tmp;
        d[0] = d[2];
        d[1] = d[3];
        d[2] = d[4];
        d[3] = d[5];
        d[4] = d[6];
        d[5] = d[7];
        d[6] = x1;
        d[7] = y1;
    }


}

