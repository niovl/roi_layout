# include "utils.h"


float get_v(int a, int b) {
    int mx = a > b? a: b;
    int mi = a <= b? a: b;
    return mi / float(mx);
}


float iou(Rect r0, Rect r1) {
    float v = 0;
    int w = min(r0.x + r0.width, r1.x + r1.width) - max(r0.x, r1.x);
    int h = min(r0.y + r0.height, r1.y + r1.height) - max(r0.y, r1.y);
    if (w > 0 && h > 0) {
        float area_n = w * h;
        v = area_n / (r0.area() + r1.area() - area_n);
    }
    return v;
}
