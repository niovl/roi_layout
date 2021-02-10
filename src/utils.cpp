# include "utils.h"
#define PI    3.1415926535897932384626433832795
#define PI2   PI*PI
#define RAD2DEG(angle) (angle*float(180.0/PI))


float get_v(int a, int b) {
    int mx = a > b? a: b;
    int mi = a <= b? a: b;
    return mi / float(mx);
}


/**
 * @brief computing the intersection over the union.
 *
 * computes the complement of the iou as described in <https://en.wikipedia.org/wiki/Jaccard_index>.
 */
template<typename Dtype> static inline
double intersectionOverUnion(const Rect_<Dtype>& a, const Rect_<Dtype>& b) {
    Dtype Aa = a.area();
    Dtype Ab = b.area();

    if ((Aa + Ab) <= std::numeric_limits<Dtype>::epsilon()) {
        return 0.0;
    }

    double An = (a & b).area();
    return An / (Aa + Ab - An);
}


/**
 * @brief optimize in the case of non-overlapping bounding boxes.
 *
 * computes the complement of the giou as described in <https://arxiv.org/pdf/1902.09630.pdf>.
 */
template<typename Dtype> static inline
double generalizedIntersectionOverUnion(const Rect_<Dtype>& a, const Rect_<Dtype>& b) {
    Dtype Aa = a.area();
    Dtype Ab = b.area();

    if ((Aa + Ab) <= std::numeric_limits<Dtype>::epsilon()) {
        return 0.0;
    }
    double An = (a & b).area();
    double A = (a | b).area();
    double Au = Aa + Ab - An;

    return An / Au - ((A - Au) / A);
}


/**
 * @brief incor-porating the normalized distance between the predicted boxand the target box.
 *
 * computes the complement of the diou as described in <https://arxiv.org/pdf/1911.08287.pdf>.
 */
template<typename Dtype> static inline
double distancIntersectionOverUnion(const Rect_<Dtype>& a, const Rect_<Dtype>& b) {
    Dtype Aa = a.area();
    Dtype Ab = b.area();

    if ((Aa + Ab) <= std::numeric_limits<Dtype>::epsilon()) {
        return 0.0;
    }
    double An = (a & b).area();
    double Au = Aa + Ab - An;
    Point_<Dtype> ap(a.x + a.width / 2, a.y + a.height);
    Point_<Dtype> bp(b.x + b.width / 2, b.y + b.height);
    Size_<Dtype> C = (a | b).size();
    return An / Au + ap.ddot(bp) / (C.width * C.width + C.height * C.height);
}


/**
 * @brief Summarize overlap area, central point distance and aspect ratio.
 *
 * Complete IoU: in <https://arxiv.org/pdf/1911.08287.pdf>.
 */
template<typename Dtype> static inline
double completeIntersectionOverUnion(const Rect_<Dtype>& a, const Rect_<Dtype>& b) {
    Dtype Aa = a.area();
    Dtype Ab = b.area();

    if ((Aa + Ab) <= std::numeric_limits<Dtype>::epsilon()) {
        return 0.0;
    }
    double An = (a & b).area();
    double Au = Aa + Ab - An;
    Point_<Dtype> ap(a.x + a.width / 2, a.y + a.height);
    Point_<Dtype> bp(b.x + b.width / 2, b.y + b.height);
    Size_<Dtype> C = (a | b).size();
    double rad = RAD2DEG(cv::fastAtan2(a.width, a.height) - cv::fastAtan2(b.width, b.height));
    double v = (4 * rad * rad) / PI2;
    double iou = An / Au;
    double alpha = v / ((1 - iou) + v);
    return iou + ap.ddot(bp) / (C.width * C.width + C.height * C.height) + alpha * v;
}


template < typename Dtype> 
vector< size_t>  sort_indexes(const vector< Dtype>  &v) {
  // initialize original index locations
  vector< size_t>  idx(v.size());
  for (size_t i = 0; i != idx.size(); ++i) idx[i] = i;

  // sort indexes based on comparing values in v
  sort(idx.begin(), idx.end(),
       [& v](size_t i1, size_t i2) {return v[i1] >  v[i2];});

  return idx;
}
