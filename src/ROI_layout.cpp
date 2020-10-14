#include "ROI_layout.h"


void ROILayout::get_layout(vector<Rect> &layout_) {
    layout_ = layout; 
}

void ROILayout::TryLayout(vector<Rect> rois_) {

    if(!Init(rois_)) return;
    ShuffleRois(rois_);
    candidates.push_back(Point(0, 0));
    for (int i = 0; i < roi_count; i++) {
        Rect roi = rois[i];
        FindBestLocation(roi);
        layout.push_back(roi);
    }
}

void ROILayout::FindBestLocation(Rect &rect) {
    int w = 0, h = 0;
    float ratio_wh = 0, ratio_area = 0;
    vector<float> scores;

    for (int i = 0; i < candidates.size(); i++) {
        if (CoverageDetectByIou(candidates[i], rect)) {
            scores.push_back(0);
            continue;
        }
        w = candidates[i].x + rect.width;
        h = candidates[i].y + rect.height;
        w = W > w? W: w;
        h = H > h? H: h;
        ratio_wh = get_v(w, h);  // Ratio of long side to short side. range => [0, 1]
        ratio_area = (rois_area + rect.area()) / float(w * h);  // Utilization
        float score = ratio_wh * ratio_area;  // Can be customized.
        scores.push_back(score);
    }
    
    size_t ind = sort_indexes(scores)[0];
    rect.x = candidates[ind].x, rect.y = candidates[ind].y;
    Update(rect, ind);
}

bool ROILayout::CoverageDetectByMask(Point candidate, Rect rect) {
    bool flag = false;
    if (candidate.x == 0 && candidate.y == 0) return flag;

    Mat coverage;
    Mat curr_mask = Mat::zeros(mask.rows, mask.cols, CV_8UC1);

    if (candidate.x <  mask.cols && candidate.y < mask.rows) {
        Point p = Point(candidate.x + rect.width, candidate.y + rect.height);
        p.x = p.x > mask.cols? mask.cols: p.x;
        p.y = p.y > mask.rows? mask.rows: p.y; 
        curr_mask(Range(candidate.y, p.y), Range(candidate.x, p.x)) = 1;
    }
    bitwise_and(mask, curr_mask, coverage);
    if(sum(coverage)[0] > 0) flag = true;

    return flag;
}

bool ROILayout::CoverageDetectByIou(Point candidate, Rect rect) {
    bool flag = false;
    if (candidate.x == 0 && candidate.y == 0) return flag;

    for (int i = 0; i < layout.size(); i++) {
        if (iou(layout[i], Rect(candidate.x, candidate.y, rect.width, rect.height)) > 0) {
            flag = true;
            break;
        }
    }
    return flag;
}

void ROILayout::Update(Rect r, size_t ind) {
    int w = r.x + r.width, h = r.y + r.height;
    W = W > w? W: w;
    H = H > h? H: h;
    // update rois_area
    rois_area += r.area();
    // update candidates
    candidates.erase(candidates.begin() + ind);
    candidates.push_back(Point(r.x, r.y + r.height));
    candidates.push_back(Point(r.x + r.width, r.y));
    // update mask
    Mat temp = Mat::zeros(H, W, CV_8UC1);
    
    if (!mask.empty()) mask.copyTo(temp(Rect(0, 0, mask.cols, mask.rows)));
    temp(r) = 1;
    mask = temp;
}


bool ROILayout::Init(vector<Rect> rois_) {
    bool flag = true;

    layout.clear();
    candidates.clear();
    rois = rois_;
    W = 0, H = 0;
    rois_area = 0;
    roi_count = rois_.size();
    if (roi_count == 0) flag = false;
    return flag;
}

void ROILayout::ShuffleRois(vector<Rect> rois_) {
    vector<int> max_side_len(roi_count);
    int w = 0, h = 0;
    
    for (int i = 0; i < roi_count; i++) {
        w = rois_[i].width;
        h = rois_[i].height;
        max_side_len[i] = (w > h? w: h);
    }
    vector<size_t> index = sort_indexes(max_side_len);
    for (int i = 0; i < roi_count; i++) {
        rois[i] = rois_[index[i]];
    }
}

void ROILayout::get_layout_mat(Mat src_mat, Mat &mat) {
    if (roi_count == 0) return;

    mat = Mat::zeros(H, W, CV_8UC3);
    cout << "[W x H] => " << W << ", " << H << endl;
    for (int i = 0; i< roi_count; i++) {
        src_mat(rois[i]).copyTo(mat(layout[i]));
    }
}

// box in layout image, coordinates in the original image.
void ROILayout::MapLayoutBoxToROI(Rect layout_box, Rect &roi) {
    for (int i = 0; i < roi_count; i++) {
        Rect r = layout[i];
        Point p = Point(layout_box.x + layout_box.width / 2, layout_box.y + layout_box.height / 2);
        if (p.x > r.x && p.y > r.y && p.x < (r.x + r.width) && p.y < (r.y + r.height)) {
            int cx = p.x - r.x + rois[i].x;
            int cy = p.y - r.y + rois[i].y;
            roi.width = min(layout_box.x + layout_box.width, r.x + r.width) - max(layout_box.x, r.x);
            roi.height = min(layout_box.y + layout_box.height, r.y + r.height) - max(layout_box.y, r.y);
            roi.x = cx - roi.width / 2, roi.y = cy - roi.height / 2;
            break;
        }
    }
}