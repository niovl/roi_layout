'''
kgsp 2020-12
'''
from point import Point


class Rectangle(object):
    def __init__(self, x_, y_, width_, height_):
        self.x = x_
        self.y = y_
        self.width = width_
        self.height = height_

    def _set(self, x_, y_, width_, height_):
        self.x = x_
        self.y = y_
        self.width = width_
        self.height = height_

    def __set(self, rect):
        self.x = rect.x
        self.y = rect.y
        self.width = rect.width
        self.height = rect.height
    
    def print(self, ):
        print('[XYWH] {} {} {} {}.'.format(self.x, self.y, self.width, self.height))

    def center(self, ):
        return Point(self.x + self.width / 2, self.y + self.height / 2)

    def right(self, ):
        return self.x + self.width

    def bottom(self, ):
        return self.y + self.height

    def lt(self, ):
        return Point(self.x, self.y)

    def lb(self, ):
        return Point(self.x, self.y + self.height)

    def rt(self, ):
        return Point(self.x + self.width, self.y)

    def rb(self, ):
        return Point(self.x + self.width, self.y + self.height)

    def area(self, ):
        return self.width * self.height
    
    def long_side(self, ):
        return self.width > self.height and self.width or self.height

    def short_side(self, ):
        return self.width > self.height and self.height or self.width

    def offset(self, p):
        self.x = p.x
        self.y = p.y

    def to_list(self, mode):
        if mode == 0:   # xywh
            return [self.x, self.y, self.width, self.height]
        elif mode == 1:   # x0y0x1y1
            return [self.x, self.y, self.x + self.width, self.y + self.height]
        else:
            pass

    def dilate(self, mode, xcoeff=1, ycoeff=1):
        p = self.center()
        if mode == 0:   # constant
            self.width += xcoeff
            self.height += ycoeff
        elif mode == 1:   # ratio
            self.width *= xcoeff
            self.height *= ycoeff
        else:
            pass
        self.x = p - self.width / 2
        self.y = p - self.height / 2

    def __or__(self, r):
        r0 = Rectangle(0, 0, 0, 0)
        r0.x = min(self.x, r.x)
        r0.y = min(self.y, r.y)
        r0.width = max(self.right(), r.right()) - r0.x
        r0.height = max(self.bottom(), r.bottom()) - r0.y
        return r0

    def __and__(self, r):
        r0 = Rectangle(0, 0, 0, 0)
        r0.x = max(self.x, r.x)
        r0.y = max(self.y, r.y)
        r0.width = min(self.right(), r.right()) - r0.x
        r0.height = min(self.bottom(), r.bottom()) - r0.y
        if r0.width < 0 or r0.height < 0:
            r0.width = 0; r0.height = 0
            r0.x = 0; r0.y = 0
        return r0
