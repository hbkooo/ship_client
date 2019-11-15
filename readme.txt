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
    
    002  :  url 连接服务器失败
    003  :  请求超时
    004  :  本地配置文件 config.json 有误
    005  :  参数错误
    
    101  :  YOLO检测成功
    102  :  YOLO检测失败
    
    201  :  遥感舰船检测成功
    202  :  遥感舰船检测失败
    
    301  :  SAR图像检测成功
    302  :  SAR图像检测失败
    
    10001  :  api_key 有误
    10005  :  Json参数错误，缺少参数或类型不对
    10006  :  Json body语法错误
    10007  :  图像数据格式错误(图片base64编码错误(删除头`data:image/jpg;base64`))
    10009  :  参数为空
    10010  :  授权错误
    
    
need : 
    cfg/*.json
    deps/lib/*.so
    font/*
    ObjectDetection
    