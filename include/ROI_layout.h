#ifndef ROI_LAYOUT_H_
#define ROI_LAYOUT_H_

#include <iostream>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include "utils.h"

using namespace std;
using namespace cv;

class ROILayout {
public:
    void get_layout(vector<Rect> &layout_);
    void get_layout_mat(Mat src_mat, Mat &mat);
    void TryLayout(vector<Rect> rois_);
    void MapLayoutBoxToROI(Rect layout_box, Rect &roi);

private:
    vector<Rect> rois;        // before
    uint16_t rois_area;       // sum of roi's area.
    vector<Rect> layout;      // after
    Mat mask;                 // mask of layout.
    uint16_t roi_count;
    vector<Point> candidates;
    uint16_t W, H;

    void FindBestLocation(Rect &rect);
    bool CoverageDetectByMask(Point candidate, Rect rect);
    bool CoverageDetectByIou(Point candidate, Rect rect);
    void Update(Rect r, size_t ind);
    bool Init(vector<Rect> rois_);
    void ShuffleRois(vector<Rect> rois_);

};   // ROILayout


#endif   // ROI_LAYOUT_H_
