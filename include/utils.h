#ifndef UTILS_H_
#define UTILS_H_


#include <iostream>
#include <opencv/cv.h>


#define PI    3.1415926535897932384626433832795
#define PI2   PI*PI
#define RAD2DEG(angle) (angle*float(180.0/PI))
// #define iou(a, b) (intersectionOverUnion(a, b))
// #define giou(a, b) (generalizedIntersectionOverUnion(a, b))
// #define diou(a, b) (distancIntersectionOverUnion(a, b))
// #define ciou(a, b) (distancIntersectionOverUnion(a, b))
using namespace std;
using namespace cv;


template < typename Dtype> vector<size_t>  sort_indexes(const vector<Dtype>  &v);
float get_v(int a, int b);


/**
 * @brief computing the intersection over the union.
 *
 * computes the complement of the iou as described in <https://en.wikipedia.org/wiki/Jaccard_index>.
 */
template<typename Dtype> static inline
double intersectionOverUnion(const Rect_<Dtype>& a, const Rect_<Dtype>& b);


/**
 * @brief optimize in the case of non-overlapping bounding boxes.
 *
 * computes the complement of the giou as described in <https://arxiv.org/pdf/1902.09630.pdf>.
 */
template<typename Dtype> static inline
double generalizedIntersectionOverUnion(const Rect_<Dtype>& a, const Rect_<Dtype>& b);


/**
 * @brief incor-porating the normalized distance between the predicted boxand the target box.
 *
 * computes the complement of the diou as described in <https://arxiv.org/pdf/1911.08287.pdf>.
 */
template<typename Dtype> static inline
double distancIntersectionOverUnion(const Rect_<Dtype>& a, const Rect_<Dtype>& b);


/**
 * @brief Summarize overlap area, central point distance and aspect ratio.
 *
 * Complete IoU: in <https://arxiv.org/pdf/1911.08287.pdf>.
 */
template<typename Dtype> static inline
double completeIntersectionOverUnion(const Rect_<Dtype>& a, const Rect_<Dtype>& b);


typedef double (* DOUBLE_RECT_)(const Rect_<Dtype>& a, const Rect_<Dtype>& b);
extern DOUBLE_RECT_ iou = intersectionOverUnion;
extern DOUBLE_RECT_ giou = generalizedIntersectionOverUnion;
extern DOUBLE_RECT_ diou = distancIntersectionOverUnion;
extern DOUBLE_RECT_ ciou = completeIntersectionOverUnion;



#endif   // UTILS_H_
