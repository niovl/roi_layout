# -*- coding: utf-8 -*- 
import numpy as np
from json import load
import cv2 as cv


def get_v(a, b):
    mx = (a > b) and a or b
    mi = (a <= b) and a or b
    return mi / float(mx)


class ROILayout:
    def __init__(self):
        self.ori = list()
        self.merge = list()
        self.candidates = list()
        self.mask = np.array([])
        self.output_size = 640
        self.delta_area = 0
        self.index = list()
        self.W = 0
        self.H = 0

    def set_output_size(self, output_size):
        self.output_size = output_size

    def init(self, input_):
        self.candidates = [[0, 0]]
        self.delta_area = 0
        self.W = 0
        self.H = 0
        self.index = list()
        self.ori = input_
        self.merge = list()
        self.mask = np.zeros((self.output_size, self.output_size), dtype=np.uint8)

    def deploy(self, r):
        scores = list()

        for i in range(len(self.candidates)):
            curr_mask = np.zeros(self.mask.shape, np.uint8)
            p = self.candidates[i]
            curr_mask[p[1]: p[1] + r[3], p[0]: p[0] + r[2]] = 1
            temp = cv.bitwise_and(self.mask, curr_mask)
            if np.sum(temp) > 0:
                scores.append(0)
            else:
                w = p[0] + r[2]; h = p[1] + r[3]
                w = (self.W > w) and self.W or w
                h = (self.H > h) and self.H or h
                i_area = float(w * h) / (self.delta_area + r[2] * r[3])
                i_wh = get_v(w, h)
                score = i_wh / i_area
                scores.append(score)
        ind = np.argsort(-np.array(scores))[0]
        p = self.candidates[ind]
        r[0] = p[0]
        r[1] = p[1]
        # update(W, H, delta_area, candidates)
        w = p[0] + r[2]
        h = p[1] + r[3]
        self.W = (self.W > w) and self.W or w
        self.H = (self.H > h) and self.H or h
        # cout << "[W, H] " << W << ", " << H << endl;
        self.delta_area += (r[2] * r[3])
        # candidates(delete add unique)
        self.update_candidates(r, ind)
        return r

    def update_candidates(self, rec, ind):
        del self.candidates[ind]
        p_lb = [rec[0], rec[1] + rec[3]]; p_ru = [rec[0] + rec[2], rec[1]]
        self.candidates.append(p_lb)
        self.candidates.append(p_ru)

    def re_layout(self, input_):
        self.init(input_)
        max_side_len = np.array([], dtype=np.int)
        for r in self.ori:
            w = r[2]; h = r[3]
            max_side_len = np.append(max_side_len, [(w > h) and w or h])
        self.index = np.argsort(-max_side_len)
        for i in range(self.index.size):
            rec = self.ori[self.index[i]]
            m_rec = [rec[0], rec[1], rec[2], rec[3]]
            if i == 0:
                m_rec[0] = 0
                m_rec[1] = 0
                self.W = m_rec[2]
                self.H = m_rec[3]
                self.delta_area += (m_rec[2] * m_rec[3])
                self.update_candidates(m_rec, 0)
            else:
                m_rec = self.deploy(m_rec)

            self.mask[m_rec[1]: m_rec[1] + m_rec[3], m_rec[0]: m_rec[0] + m_rec[2]] = 1
            self.merge.append(m_rec)

    def extract(self, mat, save_path):
        if self.H == 0 or self.W == 0:
            return
        print('[cp W H area_v w_h] {} {} {} {}'.format((self.W * self.H) / (mat.shape[0] * mat.shape[1]),
                                    self.W, self.H, self.delta_area / (self.W * self.H), get_v(self.W, self.H)))
        temp = np.zeros((self.H, self.W, 3), np.uint8)
        for i in range(len(self.merge)):
            x, y, w, h = self.merge[i]
            x0, y0, w0, h0 = self.ori[self.index[i]]
            # cv.imshow('demo', mat[y0: y0 + h0, x0: x0 + w0, :])
            # cv.waitKey(1000)
            temp[y: y + h, x: x + w, :] = mat[y0: y0 + h0, x0: x0 + w0, :]
        cv.imwrite(save_path, temp)


def load_labelme(file_path):
    input_ = list()
    with open(file_path, 'r', encoding='utf-8') as fd:
        dct = load(fd)
    ind = 0
    for i in dct['shapes']:
        if i['label'] == 'person' and i['shape_type'] == 'rectangle':
            rec = np.array(i['points'], dtype=np.int)
            x = np.min(rec[:, 0])
            y = np.min(rec[:, 1])
            w = np.max(rec[:, 0]) - x
            h = np.max(rec[:, 1]) - y
            # if w * h > 6400:
                # continue
            input_.append([x, y, w, h])
            ind += 1

    return input_


def add_label(mat, lst):
    for ll in lst:
        p0 = (ll[0], ll[1])
        p1 = (ll[0] + ll[2], ll[1] + ll[3])
        cv.rectangle(mat, p0, p1, (255, 255, 0), 2, 1)


def main():
    mb = ROILayout()
    input_ = load_labelme('kite.json')
    print(input_)
    # mat = cv.imread('SEQ_01_119.jpg')
    # mb.re_layout(input_)
    # mb.extract(mat, 'temp.jpg')


if __name__ == '__main__':
    main()
