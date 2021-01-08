'''
niovl 2020-12
'''
from rectangle import Rectangle
from point import Point
import numpy as np


def iou(r0, r1):
    return (r0 & r1).area() / (r0 | r1).area()


class RectanglesLayout(object):
    def __init__(self, ):
        self.rectangles = list()
        self.layouted = list()
        self.candidates = list()
        self.shuffle = list()
        self.area_used = 0
        self.gap = Point(3, 3) # Point(x, y) The distance between two adjacent ROIs in the x and y directions
        self.W = 0
        self.H = 0

    def coverage_detect(self, p, r):
        flag = False
        if (p.x == 0 and p.y == 0):
            return flag
        
        for lay in self.layouted:
            if iou(lay, Rectangle(p.x, p.y, r.width, r.height)):
                flag = True
                break
        return flag

    def find_best_loc(self, r):
        score = np.array([])
        for c in self.candidates:
            if self.coverage_detect(c, r):
                score = np.append(score, 0)
                continue
            w = c.x + r.width
            h = c.y + r.height
            w = self.W > w and self.W or w
            h = self.H > h and self.H or h
            wh_ratio = min(w, h) / max(w, h)
            area_ratio = (self.area_used + r.area()) / (w * h)
            score = np.append(score, wh_ratio * area_ratio)   # Can be customized.
        index = np.argsort(-score)[0]
        r.x = self.candidates[index].x
        r.y = self.candidates[index].y
        self.update(r, index)
    
    def update(self, r, index):
        w = r.x + r.width
        h = r.y + r.height
        self.W = self.W > w and self.W or w
        self.H = self.H > h and self.H or h
        self.area_used += r.area()
        del self.candidates[index]
        lb = r.lb(); rt = r.rt()
        lb.y = lb.y > 0 and (lb.y + self.gap.y) or lb.y
        rt.x = rt.x > 0 and (rt.x + self.gap.x) or rt.x
        self.candidates.append(lb)
        self.candidates.append(rt)
    
    def init(self, rectangles_):
        self.rectangles = list()
        self.layouted = list()
        self.W = 0
        self.H = 0
        self.area_used = 0
        self.candidates = list()
        
        sides = np.array([], dtype=np.int)
        for r in rectangles_:
            s = r[2] > r[3] and r[2] or r[3]
            sides = np.append(sides, s)
        self.shuffle = np.argsort(-sides)
        for i in range(self.shuffle.size):
            x, y, w, h = rectangles_[self.shuffle[i]]
            r = Rectangle(x, y, w, h)
            self.rectangles.append(r)
        self.candidates.append(Point(0, 0))

    def infer(self, rectangles_):
        self.init(rectangles_)
        
        for r in self.rectangles:
            r_ = Rectangle(r.x, r.y, r.width, r.height)
            self.find_best_loc(r_)
            self.layouted.append(r_)
 
    ''' box in layout image, coordinates in the original image. '''
    def map_layout_box(self, box):
        bbox = Rectangle()
        p = box.center()
        for lay in self.layouted:
            if p.x > lay.x and p.y > lay.y and p.x < lay.right() and p.y < lay.bottom():
                cx = p.x - lay.x + self.rectangles[i].x
                cy = p.y - lay.y + self.rectangles[i].y
                bbox.width = min(box.x + box.width, lay.x + lay.width) - max(box.x, lay.x)
                bbox.height = min(box.y + box.height, lay.y + lay.height) - max(box.y, lay.y)
                bbox.x = cx - bbox.width / 2
                bbox.y = cy - bbox.height / 2
                break
        return bbox
