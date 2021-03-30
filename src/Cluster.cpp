#include "Cluster.h"


void external_region(std::vector<cv::Rect> rects, cv::Rect& region) {
    int x0, y0, x1, y1;
    region = rects[0];

    for (size_t i = 1; i < rects.size(); i++) {
        cv::Rect rect = rects[i];
        x0 = std::min(region.x, rect.x);
        y0 = std::min(region.y, rect.y);
        x1 = std::max(region.x + region.width, rect.x + rect.width);
        y1 = std::max(region.y + region.height, rect.y + rect.height);
        region = cv::Rect(x0, y0, x1 - x0, y1 - y0);
    }
}


uint splitLine(uint* line, uint len) {
    bool continuous = false;
    uint ind = 0;
    uint buffer[len] = {0};

    for (uint i = 1; i < len; i++) {
        int diff = line[i] - line[i - 1];
        if (diff == 1) {
            continuous = true;
            ind += 1;
        } else if (diff == -1) {
            continuous = false;
        }
        if (continuous) {
            buffer[i] = ind;
        }
    }
    for (uint i = 1; i < len; i++) {
        line[i] = buffer[i];
    }
    return ind;
}


float duty_cycle(std::vector<cv::Rect> rects) {
    cv::Rect region;
    external_region(rects, region);
    char xline[region.width + 2] = {0};
    char yline[region.height + 2] = {0};

    for (auto rect: rects) {
        for (int i = 0; i < rect.width; i++) {
            xline[X0 + i] = 1;
        }
        for (int i = 0; i < rect.height; i++) {
            yline[Y0 + i] = 1;
        }
    }
    float count = 0;
    for (int i = 0; i < region.width; i++) {
        if (xline[i + 1] == 1) count += 1;
    }
    for (int i = 0; i < region.height; i++) {
        if (yline[i + 1] == 1) count += 1;
    }

    return count / (region.width + region.height);
}

        
void cluster(std::vector<cv::Rect> rects, std::vector<std::vector<cv::Rect>>& dividable, std::vector<cv::Rect>& individable) {
    cv::Rect region;
    std::map<uint, std::vector<Rect>> group;
    external_region(rects, region);
    uint xline[region.width + 2] = {0};
    uint yline[region.height + 2] = {0};

    for (auto rect: rects) {
        for (int i = 0; i < rect.width; i++) {
            xline[X0 + i] = 1;
        }
        for (int i = 0; i < rect.height; i++) {
            yline[Y0 + i] = 1;
        }
    }
    uint indx = splitLine(xline, region.width + 2);
    splitLine(yline, region.height + 2);
    std::vector<uint> ind;
    for (auto rect: rects) {
        uint x = xline[X0];
        uint y = yline[Y0];
        ind.push_back(y * indx + x);
    }
    std::vector<uint> buffer = ind;
    std::set<uint> inds(buffer.begin(), buffer.end());
    buffer.assign(inds.begin(), inds.end());

    for (auto b: buffer) {
        std::vector<Rect> g;
        group.insert(make_pair(b, g));
    }
    for (auto rect: rects) {
        int xi = xline[X0];
        int yi = yline[Y0];
        group[yi * indx + xi].push_back(rect);
    }
    std::map<uint, std::vector<Rect>>::iterator iter;
    for (iter = group.begin(); iter != group.end(); iter++) {
        float v = duty_cycle(iter->second);
        if (v == 1) {
            cv::Rect region_;
            external_region(iter->second, region_);
            individable.push_back(region_);
        } else {
            dividable.push_back(iter->second);
        }
    }
}


Cluster::~Cluster() {

}


Cluster::Cluster(const Size frame) {
    this->r = Size(5, 5);
    this->frame = frame;
}


Cluster::Cluster(const Size frame, const Size r) {
    this->r = r;
    this->frame = frame;
}


void Cluster::dilate(const vector<Rect> rects, vector<Rect>& dst) {
    vector<Rect>().swap(dst);
    for (auto rect: rects) {
        rect = rect + r + r;
        rect = rect + Point(-r.width, -r.height);
        rect.width = std::min(rect.br().x, frame.width) - rect.x;
        rect.height = std::min(rect.br().y, frame.height) - rect.y;
        rect.x = std::max(0, rect.x);
        rect.y = std::max(0, rect.y);
        dst.push_back(rect);
    }
}


void Cluster::show(Mat src, uchar func) {
    Scalar c1(0, 255, 255);
    Scalar c2(255, 255, 0);
    Scalar c3(255, 0, 255);

    rectangle(src, Rect(0, 0, 160, 130), Scalar(150, 150, 150), -1, 8);
    putText(src, "O-Rects", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, c1, 2, 8, false);
    putText(src, "D-Rects", Point(10, 70), FONT_HERSHEY_SIMPLEX, 1, c2, 2, 8, false);
    putText(src, "R-Rects", Point(10, 110), FONT_HERSHEY_SIMPLEX, 1, c3, 1, 8, false);

    if (func & 0x01) {
        for (auto rect: orignal_rects) {
            rectangle(src, rect, c1, 2, 8);
        }
    }
    if (func & 0x02) {
        for (auto dr: dilate_rects) {
            rectangle(src, dr, c2, 2, 8);
        }
    }
    if (func & 0x04) {
        for (auto region: indivisible_regions) {
            src(region) = 0.5 * c3 + 0.5 * src(region);
            //rectangle(src, region, c3, -1, 8);
        }
    }
    imwrite("cluster.jpg", src);
}


void Cluster::do_cluster(const vector<Rect> rects) {
    this->orignal_rects = rects;
    dilate(rects, dilate_rects);
    dividable_regions.push_back(dilate_rects);
    while (true) {
        std::vector<std::vector<cv::Rect>> temp;
        for (auto d: dividable_regions) {
            std::vector<std::vector<cv::Rect>> dividable;
            std::vector<cv::Rect> individable;
            cluster(d, dividable, individable);
            temp.insert(temp.end(), dividable.begin(), dividable.end());
            indivisible_regions.insert(indivisible_regions.end(), individable.begin(), individable.end());
        }
        dividable_regions = temp;
        if (dividable_regions.size() == 0) break;
    }
}
