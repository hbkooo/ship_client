#include "parseJson.h"


using namespace std;



// format : [ label score x1 y1 x2 y2 x3 y3 x4 y4 ]
std::vector<std::vector<float> > parseFourPoints(QJsonArray results, int result_nums) {
    
    std::vector<std::vector<float> > locations;
    
    QJsonObject result;
    for (int i = 0; i < result_nums; ++i) {
        std::vector<float> location(10);
        result = results.at(i).toObject();
        int label = result.value("label").toInt();
        float score = result.value("score").toDouble();
        float x1 = result.value("x1").toDouble();
        float x2 = result.value("x2").toDouble();
        float x3 = result.value("x3").toDouble();
        float x4 = result.value("x4").toDouble();
        float y1 = result.value("y1").toDouble();
        float y2 = result.value("y2").toDouble();
        float y3 = result.value("y3").toDouble();
        float y4 = result.value("y4").toDouble();
        
        location[0] = label;
        location[1] = score;
        location[2] = x1;
        location[3] = y1;
        location[4] = x2;
        location[5] = y2;
        location[6] = x3;
        location[7] = y3;
        location[8] = x4;
        location[9] = y4;
        locations.push_back(location);
        
        //std::cout << label << " \t " << score << " \t " << x1 << " \t " << x2 << " \t " << x3 << " \t " << x4
        //              << " \t " << y1 << " \t " << y2 << " \t " << y3 << " \t " << y4 << " \n ";
    }
    
    return locations;

}

// format : [ label score xmin ymin xmax ymax ]
std::vector<std::vector<float> > parseTwoPoints(QJsonArray results, int result_nums) {
    
    std::vector<std::vector<float> > locations;
    
    QJsonObject result;
    for (int i = 0; i < result_nums; ++i) {
        std::vector<float> location(6);
        result = results.at(i).toObject();
        int label = result.value("label").toInt();
        float score = result.value("score").toDouble();
        float xmin = result.value("xmin").toDouble();
        float ymin = result.value("ymin").toDouble();
        float xmax = result.value("xmax").toDouble();
        float ymax = result.value("ymax").toDouble();
        
        location[0] = label;
        location[1] = score;
        location[2] = xmin;
        location[3] = ymin;
        location[4] = xmax;
        location[5] = ymax;
        locations.push_back(location);
    }
    
    return locations;

}

std::vector<std::vector<float> > parseResult(char * results, int & response_id, string &error_msg) {
    
    std::vector<std::vector<float> > locations;
    
    QJsonParseError jsonError;
    QJsonDocument json_doc  =QJsonDocument::fromJson(results,&jsonError);
    if(jsonError.error == QJsonParseError::NoError) {
        if(json_doc.isNull() || json_doc.isEmpty()) return locations;

        QJsonObject object =  json_doc.object();
            
        string request_id = object.value("request_id").toString().toStdString();              // 返回请求的唯一标识符
        response_id = atoi(object.value("response_id").toString().toStdString().c_str());     // 返回的状态码
        error_msg = object.value("error_message").toString().toStdString();                   // 返回的错误信息（如果出错的话）
        QJsonArray result = object.value("result").toArray();                            // 返回的检测结果
        int result_nums = object.value("result_nums").toInt(); // 返回的检测结果的数量
        int time_used = object.value("time_used").toInt();     // 返回检测所耗时间
        
        //std::cout << "request_id : " << request_id << "\n";
        //std::cout << "response_id : " << response_id << "\n";
        //std::cout << "time_used : " << time_used << "\n";
        //std::cout << "result_nums : " << result_nums << "\n";
        //std::cout << "error_msg : " << error_msg << "\n";
        
        switch(response_id) {
            
            case FINDER_YOLO_SUCCESS : 
                //break;
            case FINDER_SAR_SUCCESS : 
                locations = parseTwoPoints(result, result_nums);
                break;
            case FINDER_SHIP_SUCCESS : 
                locations = parseFourPoints(result, result_nums);
                break;
        
        }

            
    }
   
    return locations;
}

// 加载 json 配置文件
bool loadConfig(std::string configPath) {

    bool isSuccess = true;
    
    QFile info_file(QString::fromStdString(configPath));
    
    if (!info_file.exists()) {
        //std::cout << "load config path : ' " << configPath << " ' failed ... \n";
        if (generateDefaultConfig(configPath)) {
            std::cout << "generate default config file : ' " << configPath << " ' \n";
            return true;
        } 
        std::cout << "generate default config file : ' " << configPath << " ' failed ... \n";
        return false;
    }
    
    ConfigParams * params = ConfigParams::instance();
    
    if(info_file.open(QFile::ReadOnly)) {
        QJsonParseError jsonError;
        //QJsonDocument json_doc = QJsonDocument::fromJson(codec->toUnicode(info_file.readAll().constData()).toLatin1(), &jsonError);
        QJsonDocument json_doc = QJsonDocument::fromJson(info_file.readAll(), &jsonError);
        if(jsonError.error == QJsonParseError::NoError) {
            if(json_doc.isNull() || json_doc.isEmpty()) isSuccess = false;

            QJsonObject object =  json_doc.object();
            
            params->CODE_NAME = object.value("code_name").toString().toStdString();

            QJsonObject thresh = object.value("detect_thresh").toObject();
            params->YOLO_THRESH = thresh.value("yolo_thresh").toDouble();
            params->SHIP_THRESH = thresh.value("ship_thresh").toDouble();
            params->SAR_THRESH = thresh.value("sar_thresh").toDouble();

            params->INTERFACEID_YOLO = object.value("interface_yolo").toInt();
            params->INTERFACEID_SHIP = object.value("interface_ship").toInt();
            params->INTERFACEID_SAR = object.value("interface_sar").toInt();

            params->SERVER = object.value("server").toString().toStdString();

            
        }
        else {
            isSuccess = false;
            //qDebug() << "error : " << jsonError.errorString();
        }
    }
    info_file.close();
    
    /*
    std::cout << "SERVER : ' " << params->SERVER << " ' \n";
    std::cout << "CODE_NAME : ' " << params->CODE_NAME << " ' \n";
    std::cout << "INTERFACEID_YOLO : ' " << params->INTERFACEID_YOLO << " ' \n";
    std::cout << "INTERFACEID_SHIP : ' " << params->INTERFACEID_SHIP << " ' \n";
    std::cout << "INTERFACEID_SAR : ' " << params->INTERFACEID_SAR << " ' \n";
    std::cout << "YOLO_THRESH : ' " << params->YOLO_THRESH << " ' \n";
    std::cout << "SHIP_THRESH : ' " << params->SHIP_THRESH << " ' \n";
    std::cout << "SAR_THRESH : ' " << params->SAR_THRESH << " ' \n"; */
    
    
    return isSuccess;
    
}


// 生成默认的 json 配置文件
bool generateDefaultConfig(std::string configPath) {
    
    bool isSuccess = true;
    
    // 生成 json 中的每一项
    QJsonObject object;
    object.insert("code_name",QString::fromStdString(DEFAULT_CODE_NAME));
    object.insert("server",QString::fromStdString(DEFAULT_URL));
    object.insert("interface_yolo",DEFAULT_INTERFACEID_YOLO);
    object.insert("interface_ship",DEFAULT_INTERFACEID_SHIP);
    object.insert("interface_sar",DEFAULT_INTERFACEID_SAR);

    QJsonObject thresh;
    thresh.insert("yolo_thresh",0.5);
    thresh.insert("ship_thresh",0.6);
    thresh.insert("sar_thresh",0.7);
    object.insert("detect_thresh",thresh);
    
    QJsonDocument document;
    document.setObject(object);
    QByteArray byte_array = document.toJson();

    QFile saveJson(QString::fromStdString(configPath));
    if(!saveJson.open(QIODevice::WriteOnly | QFile::Truncate)) {
        isSuccess = false;
        std::cout << "open file faile\n";
    } else {
        std::cout << "open file success\n";
    }
    saveJson.write(byte_array);
    saveJson.close();
    
    return isSuccess;

}

// 将json的属性转换为map格式
QMap<QString,QString> convertJsonToMap(QString attr) {
    
    QMap<QString, QString> map;
    
    QJsonParseError jsonError;
    QJsonDocument json_doc  =QJsonDocument::fromJson(attr.toUtf8().data(),&jsonError);
    
    
    if(jsonError.error == QJsonParseError::NoError) {
        if(json_doc.isNull() || json_doc.isEmpty()) return map;

        QJsonObject object =  json_doc.object();
        
        foreach(QString attr_name, object.keys()) {
            //qDebug() << "attr_name : " << class_name;
            map.insert(attr_name, object.value(attr_name).toString());
        }  
    }
   
    return map;
}

QPixmap loadPixImage(QString path) {
    QPixmap map(path);
    QFile img(path);
    if(!map && img.open(QFile::ReadOnly)) {
        std::cout << "open image...";
        QByteArray bytes = img.readAll();
        map.loadFromData(bytes);
    }
    img.close();
    return map;
}


/*
// format : [ label score x1 y1 x2 y2 x3 y3 x4 y4 ]
std::vector<std::vector<float> > parseFourPoints(Json::Value results, int result_nums) {
    
    std::vector<std::vector<float> > locations;
    
    Json::Value result;
    for (int i = 0; i < result_nums; ++i) {
        std::vector<float> location(10);
        result = results[i];
        int label = atoi(result["label"].asString().c_str());
        float score = atof(result["score"].asString().c_str());
        float x1 = atof(result["x1"].asString().c_str());
        float x2 = atof(result["x2"].asString().c_str());
        float x3 = atof(result["x3"].asString().c_str());
        float x4 = atof(result["x4"].asString().c_str());
        float y1 = atof(result["y1"].asString().c_str());
        float y2 = atof(result["y2"].asString().c_str());
        float y3 = atof(result["y3"].asString().c_str());
        float y4 = atof(result["y4"].asString().c_str());
        
        location[0] = label;
        location[1] = score;
        location[2] = x1;
        location[3] = y1;
        location[4] = x2;
        location[5] = y2;
        location[6] = x3;
        location[7] = y3;
        location[8] = x4;
        location[9] = y4;
        locations.push_back(location);
        
        //std::cout << label << " \t " << score << " \t " << x1 << " \t " << x2 << " \t " << x3 << " \t " << x4
        //              << " \t " << y1 << " \t " << y2 << " \t " << y3 << " \t " << y4 << " \n ";
    }
    
    return locations;

}

// format : [ label score xmin ymin xmax ymax ]
std::vector<std::vector<float> > parseTwoPoints(Json::Value results, int result_nums) {
    
    std::vector<std::vector<float> > locations;
    
    Json::Value result;
    for (int i = 0; i < result_nums; ++i) {
        std::vector<float> location(6);
        result = results[i];
        int label = atoi(result["label"].asString().c_str());
        float score = atof(result["score"].asString().c_str());
        float xmin = atof(result["xmin"].asString().c_str());
        float ymin = atof(result["ymin"].asString().c_str());
        float xmax = atof(result["xmax"].asString().c_str());
        float ymax = atof(result["ymax"].asString().c_str());
        
        location[0] = label;
        location[1] = score;
        location[2] = xmin;
        location[3] = ymin;
        location[4] = xmax;
        location[5] = ymax;
        locations.push_back(location);
    }
    
    return locations;

}

std::vector<std::vector<float> > parseResult(char * results, int & response_id, string &error_msg) {
    
    std::vector<std::vector<float> > locations;
    
    Json::Reader reader;
    Json::Value value;
    if (reader.parse(results,value)) {
        string request_id = value["request_id"].asString();              // 返回请求的唯一标识符
        response_id = atoi(value["response_id"].asString().c_str());     // 返回的状态码
        error_msg = value["error_message"].asString();                   // 返回的错误信息（如果出错的话）
        Json::Value result = value["result"];                            // 返回的检测结果
        int result_nums = atoi(value["result_nums"].asString().c_str()); // 返回的检测结果的数量
        int time_used = atoi(value["time_used"].asString().c_str());     // 返回检测所耗时间
        
        //std::cout << "request_id : " << request_id << "\n";
        //std::cout << "response_id : " << response_id << "\n";
        //std::cout << "time_used : " << time_used << "\n";
        //std::cout << "result_nums : " << result_nums << "\n";
        //std::cout << "error_msg : " << error_msg << "\n";
        
        switch(response_id) {
            
            case FINDER_YOLO_SUCCESS : 
                //break;
            case FINDER_SAR_SUCCESS : 
                locations = parseTwoPoints(result, result_nums);
                break;
            case FINDER_SHIP_SUCCESS : 
                locations = parseFourPoints(result, result_nums);
                break;
        
        }
    }
    return locations;
}
*/

/*
// 加载 json 配置文件
bool loadConfig(std::string configPath) {
    
    ifstream in(configPath);
    if (!in.is_open()) {
        std::cout << "load config path : ' " << configPath << " ' failed ... \n";
        if (generateDefaultConfig(configPath)) {
            std::cout << "generate default config file : ' " << configPath << " ' \n";
            return true;
        } 
        std::cout << "generate default config file : ' " << configPath << " ' failed ... \n";
        return false;
    }
    Json::Reader reader;
    Json::Value config;
    ConfigParams * params = ConfigParams::instance();

    if (!reader.parse(in, config, false) || !config.isObject())
    {
        std::cout << "Config file syntax error.";
    }
    in.close();
    
    // 加载 json 文件中的每一项
    if (config.isMember("server")) {
        params->SERVER = config["server"].asString();
    }
    
    if (config.isMember("code_name")) {
        params->CODE_NAME = const_cast <char *>(config["code_name"].asString().c_str());
    }
    
    if (config.isMember("interface_yolo")) {
        params->INTERFACEID_YOLO = config["interface_yolo"].asUInt();
    }
    
    if (config.isMember("interface_ship")) {
        params->INTERFACEID_SHIP = config["interface_ship"].asUInt();
    }
    
    if (config.isMember("interface_sar")) {
        params->INTERFACEID_SAR = config["interface_sar"].asUInt();
    }
    
    if (config.isMember("detect_thresh") && config["detect_thresh"].isObject()) {
        if (config["detect_thresh"].isMember("ship_thresh")) {
            params->SHIP_THRESH = config["detect_thresh"]["ship_thresh"].asFloat();
        }
        if (config["detect_thresh"].isMember("sar_thresh")) {
            params->SAR_THRESH = config["detect_thresh"]["sar_thresh"].asFloat();
        }
        if (config["detect_thresh"].isMember("yolo_thresh")) {
            params->YOLO_THRESH = config["detect_thresh"]["yolo_thresh"].asFloat();
        }
    }
    
    
    std::cout << "SERVER : ' " << params->SERVER << " ' \n";
    std::cout << "CODE_NAME : ' " << params->CODE_NAME << " ' \n";
    std::cout << "INTERFACEID_YOLO : ' " << params->INTERFACEID_YOLO << " ' \n";
    std::cout << "INTERFACEID_SHIP : ' " << params->INTERFACEID_SHIP << " ' \n";
    std::cout << "INTERFACEID_SAR : ' " << params->INTERFACEID_SAR << " ' \n";
    std::cout << "YOLO_THRESH : ' " << params->YOLO_THRESH << " ' \n";
    std::cout << "SHIP_THRESH : ' " << params->SHIP_THRESH << " ' \n";
    std::cout << "SAR_THRESH : ' " << params->SAR_THRESH << " ' \n";
    
    
    return true; 

}
*/

/*
// 生成默认的 json 配置文件
bool generateDefaultConfig(std::string configPath) {
    
    // 生成 json 中的每一项
    Json::Value config;
    config["server"] = DEFAULT_URL;
    config["code_name"] = DEFAULT_CODE_NAME;
    config["interface_yolo"] = DEFAULT_INTERFACEID_YOLO;
    config["interface_ship"] = DEFAULT_INTERFACEID_SHIP;
    config["interface_sar"] = DEFAULT_INTERFACEID_SAR;
    
    Json::Value detect_thresh;
    detect_thresh["yolo_thresh"] = 0.5;
    detect_thresh["ship_thresh"] = 0.5;
    detect_thresh["sar_thresh"] = 0.5;
    
    config["detect_thresh"] = detect_thresh;

    Json::StyledWriter writer;
    string document = writer.write(config);

    ofstream os(configPath);
    if (!os.is_open())
        return false;
    os << document;
    os.close();
    return true;

}*/








