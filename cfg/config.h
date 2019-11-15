#ifndef CONFIG_H
#define CONFIG_H


#include <memory>

#define DEFAULT_INTERFACEID_YOLO 1
#define DEFAULT_INTERFACEID_SHIP 2
#define DEFAULT_INTERFACEID_SAR 3

#define DEFAULT_URL "tcp:\/\/127.0.0.1:5563"
#define DEFAULT_CODE_NAME "GBK"

#define CONFIG_PATH "cfg/config.json"
#define SHIP_INFO_PATH "cfg/ship_info.json"

static std::string YOLO_LABEL[] = {"人", "自行车", "小轿车", "摩托车" , "飞机", "公交车", "火车", "卡车", "船", "红绿灯",
                                    "消防栓", "停车标记", "停车计时器", "长凳", "鸟", "猫", "狗", "马", "羊", "奶牛", 
                                    "大象", "熊", "斑马", "长颈鹿", "双肩背包", "伞", "手提包", "领带", "手提箱", "飞盘",
                                    "滑雪板skis", "滑雪板snowboard", "运动球", "风筝", "棒球棒", "棒球手套", "溜冰板skateboard", "冲浪板", "网球拍", "饼子",
                                    "酒杯", "杯子", "叉子", "刀", "勺子", "碗", "香蕉", "苹果", "三明治", "橙子", 
                                    "西兰花", "胡萝卜", "热狗", "披萨", "炸面圈", "蛋糕", "椅子", "沙发", "盆栽植物", "床",
                                    "餐桌", "厕所", "电视监视器tvmonitor", "笔记本电脑", "鼠标", "遥控remote", "键盘", "手机", "微波炉", "烤箱oven",
                                    "烤面包器toaster", "水槽sink", "冰箱", "书", "钟表", "花瓶", "剪刀", "泰迪熊", "吹风机", "牙刷" };
static std::string SAR_LABEL[] = {"船", "飞机"};
static std::string SHIP_LABEL[] = {"军舰", "民船", "潜艇", "航母" };


class ConfigParams {

    public :
        static ConfigParams* instance() ; 
        ~ConfigParams();

    //protected :
        ConfigParams();

    private :
        
        static ConfigParams * params;

    public :
    
        std::string CODE_NAME = "GBK";                                  // 编码格式
        
        std::string SERVER = "tcp://127.0.0.1:5563";             // 服务器地址
        
        int INTERFACEID_YOLO = 1;
        int INTERFACEID_SHIP = 2;
        int INTERFACEID_SAR = 3;
        
        float YOLO_THRESH = 0.5;        // 民用外图像检测阈值, >= 0.5
        float SHIP_THRESH = 0.5;        // 舰船检测检测阈值
        float SAR_THRESH = 0.5;         // 红外图像检测阈值
        
};



#endif
