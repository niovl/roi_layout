#include "ROI_layout.h"
#include <iostream>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// {82, 508, 18, 58}
int main() {
    int buffer[19][4] = {{280, 237, 25, 44}, {302, 375, 23, 33}, 
                        {467, 340, 17, 19}, {577, 344, 22, 25}, 
                        {592, 80, 79, 70}, {762, 378, 8, 12}, 
                        {1084, 393, 18, 30}, {216, 698, 53, 154},
                        {113, 612, 52, 150}, {176, 540, 15, 33}, 
                        {94, 537, 15, 30}, {82, 508, 18, 58},
                        {40, 510, 11, 51}, {23, 532, 15, 25}, 
                        {347, 487, 8, 16}, {521, 505, 12, 25}, 
                        {533, 514, 21, 20}, {1208, 452, 5, 11}, 
                        {696, 462, 5, 8}};

    vector<Rect> rects(19);

    Mat mat = imread("../kite.jpg");
    for (int i = 0; i < 19; i++) {
        rects[i].x = buffer[i][0];
        rects[i].y = buffer[i][1];
        rects[i].width = buffer[i][2];
        rects[i].height = buffer[i][3];
        //rectangle(mat, rects[i], Scalar(0, 255, 127), 2, 8);

    }
    ROILayout roi_layout;
    roi_layout.TryLayout(rects);
    Mat r_mat;
    vector<Rect> layout_;
    roi_layout.get_layout(layout_);
    roi_layout.get_layout_mat(mat, r_mat);
    Rect roi;
    roi_layout.MapLayoutBoxToROI(Rect(108, 67, 20, 20), roi);
    // rectangle(r_mat, Rect(108, 67, 15, 15), Scalar(255, 0, 127), 1, 8);
    // imshow("layout.jpg", r_mat);
    // cout << roi << endl;

    rectangle(mat, roi, Scalar(255, 0, 127), 1, 8);
    imshow("roi.jpg", mat);
    // imwrite("iou.png", r_mat);
    waitKey(0);

    return 0;
}