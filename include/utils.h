#ifndef UTILS_H_
#define UTILS_H_


#include <iostream>
#include <opencv/cv.h>

using namespace std;
using namespace cv;


template < typename Dtype> vector<size_t>  sort_indexes(const vector<Dtype>  &v);
float get_v(int a, int b);
void adjust_bounds(Rect &box, Rect ref);

template<typename Dtype> static inline
double intersectionOverUnion(const Rect_<Dtype>& a, const Rect_<Dtype>& b);


template<typename Dtype> static inline
double generalizedIntersectionOverUnion(const Rect_<Dtype>& a, const Rect_<Dtype>& b);


template<typename Dtype> static inline
double distancIntersectionOverUnion(const Rect_<Dtype>& a, const Rect_<Dtype>& b);


template<typename Dtype> static inline
double completeIntersectionOverUnion(const Rect_<Dtype>& a, const Rect_<Dtype>& b);




#endif   // UTILS_H_
