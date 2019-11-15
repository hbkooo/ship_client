#define FINDER_NO_SERVICE 001
#define CONNECT_FAILED 002
#define OUTTIME 003
#define CONFIG_FILE_ERROR 004
#define PARAMS_ERROR 005

#define FINDER_YOLO_SUCCESS 101
#define FINDER_YOLO_FAIL 102

#define FINDER_SHIP_SUCCESS 201
#define FINDER_SHIP_FAIL 202

#define FINDER_SAR_SUCCESS 301
#define FINDER_SAR_FAIL 302

#define FINDER_API_KEY_INVALID 10001
#define FINDER_NONE_FACE 10002
#define FINDER_MULTI_FACE 10003
#define FINDER_UNKNOW_ERROR 10004
#define FINDER_PARAMETERS_ERROR 10005

#define ERROR_JSON_SYNTAX 10006
#define ERROR_IMAGE_BASE64 10007
#define REQUEST_LIMITED 10008

#define FINDER_PARAMETERS_EMPTY 10009
#define FINDER_NOT_AUTHORIZED 10010

response_id : 
    
    002  :  url ���ӷ�����ʧ��
    003  :  ����ʱ
    004  :  ���������ļ� config.json ����
    005  :  ��������
    
    101  :  YOLO���ɹ�
    102  :  YOLO���ʧ��
    
    201  :  ң�н������ɹ�
    202  :  ң�н������ʧ��
    
    301  :  SARͼ����ɹ�
    302  :  SARͼ����ʧ��
    
    10001  :  api_key ����
    10005  :  Json��������ȱ�ٲ��������Ͳ���
    10006  :  Json body�﷨����
    10007  :  ͼ�����ݸ�ʽ����(ͼƬbase64�������(ɾ��ͷ`data:image/jpg;base64`))
    10009  :  ����Ϊ��
    10010  :  ��Ȩ����
    
    
need : 
    cfg/*.json
    deps/lib/*.so
    font/*
    ObjectDetection
    