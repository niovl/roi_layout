
from json import load
import cv2 as cv
from os.path import splitext, join, exists
from os import walk
import numpy as np

def file_ignore_suffix(files):
    lst = list()
    for f in files:
        fn, ext = splitext(f)
        lst.append(fn)
    lst = np.unique(lst).tolist()
    use = list()
    for f in lst:
        if exists(f + '.jpg') and exists(f + '.json'):
            use.append(f)
        else:
            print('{}: image and label did not exist at the same time.'.format(f))
    return lst


def file_list(file_dir, suffix):
    files_ = list()
    for root, dirs, files in walk(file_dir):
        if len(files) == 0:
            continue
        for f in files:
            fn, ext = splitext(f)
            if ext in suffix:
                # files_.append(join(root, f).replace(file_dir + '/', ''))
                files_.append(join(root, f))
    return files_


def load_labelme(file_path):
    input_ = list()
    with open(file_path, 'r', encoding='utf-8') as fd:
        dct = load(fd)
    ind = 0
    for i in dct['shapes']:
        if i['shape_type'] == 'rectangle':
            rec = np.array(i['points'], dtype=np.int)
            x = np.min(rec[:, 0])
            y = np.min(rec[:, 1])
            w = np.max(rec[:, 0]) - x
            h = np.max(rec[:, 1]) - y
            input_.append([x, y, w, h])
            ind += 1
    return input_


def load_image(file_path):
    return cv.imread(file_path)


def main():
    inp = load_labelme('kite.json')
    print(len(inp))
    ori = cv.imread('kite.jpg')
    tool = RectanglesLayout()
    tool.infer(inp)
    mat_ = np.zeros((tool.H, tool.W, 3), dtype=np.uint8)
    for i in range(len(tool.layouted)):
        _a = tool.layouted[i]
        b = tool.rectangles[i]
        mat_[_a.y: _a.bottom(), _a.x: _a.right(), :] = ori[b.y: b.bottom(), b.x: b.right(), :]
    cv.imwrite('layout.jpg', mat_)


if __name__ == '__main__':
    main()

