#ifndef DEV_COMMON_H_
#define DEV_COMMON_H_

#include <chrono>

#define PI 3.1415926

// INTERFACEID_COUNT : the info queue's size
// the others : the index of each method in queue
enum INTERFACEID
{
    INTERFACEID_REG = 0,
    INTERFACEID_SEARCH = 1,
    INTERFACEID_DELETE = 2,
    INTERFACEID_EXIST = 3,
    INTERFACEID_COMPARE = 4,
    INTERFACEID_YOLO = 5,
    INTERFACEID_SHIP = 6,
    INTERFACEID_SAR = 7,
    INTERFACEID_MONITOR = 8,
    INTERFACEID_COUNT = 9

};

namespace face
{

struct Point
{
    float x, y;
    Point(const float &x_ = 0, const float &y_ = 0) : x(x_), y(y_)
    {
    }
};

enum Gender
{
    Male = 0,
    Female,
    None
};

enum Age
{
    kid=0,
    adult,
    none
};

enum Hair
{
    shorthair=0,
    longhair,
    bald,
    braids,
    hat,
    NONE
};

enum DetectMode
{
    NORMAL = 0,
    PORTRAIT,
    TRACK
};

struct ImageFacesInfo
{
    ImageFacesInfo()
    {
        locations.reserve(4);
        landmarks.reserve(10);
    }
    std::vector<std::string> genders;
    std::vector<std::string> ages;
    std::vector<std::string> hair;
    std::vector<std::vector<int> > locations;
    std::vector<std::vector<face::Point> > landmarks;
    std::vector<std::vector<float> > features;
    // std::vector<float> blurs;
    // std::vector<float> brightness;
    std::vector<float> scores;
    std::vector<float> glasses;
    std::vector<float> sideFace;
    std::vector<float> quality;
    std::vector<double> pitchs;
    std::vector<double> yaws;
    std::vector<double> rolls;
    std::vector<double> blurs;
    std::vector<double> illus;

    void clear()
    {
        genders.clear();
        ages.clear();
        hair.clear();
        locations.clear();
        landmarks.clear();
        features.clear();
        // blurs.clear();
        // brightness.clear();
        scores.clear();
        glasses.clear();
        sideFace.clear();
        quality.clear();
        pitchs.clear();
        yaws.clear();
        rolls.clear();
        blurs.clear();
        illus.clear();
    }
};

/// @brief Bounding Box
struct BBox
{
    float x1, y1, x2, y2;
    explicit BBox(float x1 = 0, float y1 = 0, float x2 = 0, float y2 = 0) :
            x1(x1), y1(y1), x2(x2), y2(y2) {}
    BBox(const Point & pt1, const Point & pt2) :
            x1(pt1.x), y1(pt1.y), x2(pt2.x), y2(pt2.y) {};
};

/// @ brief Facial Points
using FPoints = std::vector<Point>;
struct FaceInfo
{
public:
    BBox bbox;		// bounding box
    float score;	// face confidence
    FPoints fpts;	// facial landmarks
    FaceInfo(BBox&& bbox, float score) :
            bbox(std::move(bbox)), score(score)
    {
        fpts.reserve(5);
    }
    FaceInfo(BBox&& bbox, float score, FPoints&& fpts) :
            bbox(std::move(bbox)), score(score), fpts(std::move(fpts))
    {
        fpts.reserve(5);
    }
};
}

namespace dev
{
    typedef std::vector<std::chrono::system_clock::time_point> TIMEPOINTS;
}

#define CURRENT_SYS_TIMEPOINT std::chrono::system_clock::now()
#define CURRENT_STEADY_TIMEPOINT std::chrono::steady_clock::now()

const int TRACK_DROP_NUM = 5;
const int TRACK_CATCH_NUM = 3;

#endif //DEV_COMMON_H_
