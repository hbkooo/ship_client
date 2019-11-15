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

static std::string YOLO_LABEL[] = {"��", "���г�", "С�γ�", "Ħ�г�" , "�ɻ�", "������", "��", "����", "��", "���̵�",
                                    "����˨", "ͣ�����", "ͣ����ʱ��", "����", "��", "è", "��", "��", "��", "��ţ", 
                                    "����", "��", "����", "����¹", "˫�米��", "ɡ", "�����", "���", "������", "����",
                                    "��ѩ��skis", "��ѩ��snowboard", "�˶���", "����", "�����", "��������", "�����skateboard", "���˰�", "������", "����",
                                    "�Ʊ�", "����", "����", "��", "����", "��", "�㽶", "ƻ��", "������", "����", 
                                    "������", "���ܲ�", "�ȹ�", "����", "ը��Ȧ", "����", "����", "ɳ��", "����ֲ��", "��",
                                    "����", "����", "���Ӽ�����tvmonitor", "�ʼǱ�����", "���", "ң��remote", "����", "�ֻ�", "΢��¯", "����oven",
                                    "�������toaster", "ˮ��sink", "����", "��", "�ӱ�", "��ƿ", "����", "̩����", "�����", "��ˢ" };
static std::string SAR_LABEL[] = {"��", "�ɻ�"};
static std::string SHIP_LABEL[] = {"����", "��", "Ǳͧ", "��ĸ" };


class ConfigParams {

    public :
        static ConfigParams* instance() ; 
        ~ConfigParams();

    //protected :
        ConfigParams();

    private :
        
        static ConfigParams * params;

    public :
    
        std::string CODE_NAME = "GBK";                                  // �����ʽ
        
        std::string SERVER = "tcp://127.0.0.1:5563";             // ��������ַ
        
        int INTERFACEID_YOLO = 1;
        int INTERFACEID_SHIP = 2;
        int INTERFACEID_SAR = 3;
        
        float YOLO_THRESH = 0.5;        // ������ͼ������ֵ, >= 0.5
        float SHIP_THRESH = 0.5;        // �����������ֵ
        float SAR_THRESH = 0.5;         // ����ͼ������ֵ
        
};



#endif
