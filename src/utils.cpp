# include "utils.h"


template < typename Dtype> 
double get_v(Dtype a, Dtype b) {
    return double(std::min(a, b)) / std::max(a, b);
}


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

// double(* abc)(const Rect_<Dtype>& a, const Rect_<Dtype>& b) = generalizedIntersectionOverUnion;
// abc = distancIntersectionOverUnion;
// abc()

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
vector< size_t>  sort_indexes(const vector<Dtype>  &v) {
  // initialize original index locations
  vector< size_t>  idx(v.size());
  for (size_t i = 0; i != idx.size(); ++i) idx[i] = i;

  // sort indexes based on comparing values in v
  sort(idx.begin(), idx.end(),
       [& v](size_t i1, size_t i2) {return v[i1] >  v[i2];});

  return idx;
}



