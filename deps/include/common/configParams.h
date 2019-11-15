#ifndef CONFIG_PARAMS_H_
#define CONFIG_PARAMS_H_
#include <string>

namespace dev
{

/**
 * @brief prase config parameters from json config file
 * 
 */
struct ConfigParams
{
    /// @brief constructor
    explicit ConfigParams(std::string const &configPath);
    /// @brief print configure parameters
    void printParams() const;

    std::string apiKey = "";
    std::string userID = "";
    int port = 33388;
    int threads = 2;
    int cpuCores = 0;
    std::string ip = "0.0.0.0";
    std::string modelPath = "../model/";
    std::string licensePath = "license/";
    int batchSize = 10;
    int detectShipPoolSize = 1;
    int detectSarPoolSize = 1;
    int detectYoloPoolSize = 1;
    int GPUDevices = 1;
    int redis_db_id = 5;
    std::string redisPasswd = "";
    std::string redisHost = "127.0.0.1";
    int redisPort = 6379;
    unsigned int topK = 80;
    unsigned int featureSize = 256;
    unsigned int distance = 250;
    float simThreshold = 0.66;
    float searchThreshold = 0.75;
    unsigned int maxRequests = 20;
    int blockQueueFactor = 10; // block queue size = poolSize * blockQueueFactor
    int LimitFactor = 20;   // reg request number control
    bool isDecryptModel = false;
    std::string licenseFile = "license-key.lic";

    //faiss
    std::string searchFactory = "IDMap,Flat";
    bool useGpu = false;
    std::string faissFile = "FaissFile/faiss.bin";

    /**
     * @brief data collection switch
     *        0: nothing to do
     *        1: only store to database
     *        2: store and upload
     * 
     */
    int collectFaceData = 1;
    /// @brief server url
    std::string serverURL = "127.0.0.1:8989";
    /// @brief upload timepoint 
    std::string uploadTimepoint = "3:0:0";
    int versionMajor;
    int versionMinor;
    int versionPatch;
    std::string buildType;
    struct LogConfigParams
    {
        bool traceEnabled = false;
        bool debugEnabled = false;
        bool fatalEnabled = false;
        bool errorEnabled = false;
        bool warningEnabled = false;
        bool infoEnabled = true;
        bool verboseEnabled = false;
    } logConfigParams;

    int zmqthreads = 2;
    std::string zmqAddr = "tcp://127.0.0.1:5560";
    std::string zmqAddr2 = "tcp://*:5050";

  private:
    /**
     * @brief load config
     * 
     * @param configPath path of configure file
     */
    void loadConfig(std::string const &configPath);

    /**
     * @brief generate default configure file when configure file don't exist
     * 
     * @param configPath path of default configure file
     * @return true success
     * @return false fail
     */
    bool generateDefaultConfig(std::string const &configPath) const;
    
    /**
     * @brief initialize log configure
     * 
     * @param logConfig log configure parameter
     */
    void initEasylogging(const LogConfigParams &logConfig) const;
};  // struct ConfigParams
};  // namespace dev

#endif //CONFIG_PARAMS_H_
