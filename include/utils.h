#ifndef UTILS_H_
#define UTILS_H_


#include <iostream>
#include <opencv/cv.h>

using namespace std;
using namespace cv;

template < typename T> vector<size_t>  sort_indexes(const vector<T>  &v);
float get_v(int a, int b);
void adjust_bounds(Rect &box, Rect ref);
float iou(Rect r0, Rect r1);



template < typename T> 
vector< size_t>  sort_indexes(const vector< T>  &v) {

  // initialize original index locations
  vector< size_t>  idx(v.size());
  for (size_t i = 0; i != idx.size(); ++i) idx[i] = i;

  // sort indexes based on comparing values in v
  sort(idx.begin(), idx.end(),
       [& v](size_t i1, size_t i2) {return v[i1] >  v[i2];});

  return idx;
}

#endif   // UTILS_H_