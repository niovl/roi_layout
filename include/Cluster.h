#ifndef CLUSTER_
#define CLUSTER_

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

#define X0 rect.x - region.x + 1
#define Y0 rect.y - region.y + 1


void external_region(std::vector<cv::Rect> rects, cv::Rect& region);


float duty_cycle(std::vector<cv::Rect> rects);


uint splitLine(char* line, uint len);


void cluster(std::vector<cv::Rect> rects, std::vector<std::vector<cv::Rect>>& dividable, std::vector<cv::Rect>& individable);


//TODO: 2D-Maximum Separable Projection Method.
class Cluster {
public:
    ~Cluster();
    Cluster(const Size frame);
    Cluster(const Size frame, const Size r);
    void do_cluster(const vector<Rect> rects);
    void show(Mat src, uchar func);  //0000-0000, (ori-rects, dilate-rects, region-rects)

private:
    std::vector<Rect> orignal_rects, dilate_rects;
    std::vector<Rect> indivisible_regions;
    std::vector<std::vector<cv::Rect>> dividable_regions;
    Size frame, r;
    void dilate(const vector<Rect> rects, vector<Rect>& dst);
};

#endif