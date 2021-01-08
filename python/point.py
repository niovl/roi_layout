'''
niovl 2020-12
'''
class Point(object):
    def __init__(self, x_, y_):
        self.x = x_
        self.y = y_
    
    def _set(self, x_, y_):
        self.x = x_
        self.y = y_
    
    def __set(self, p):
        self.x = p.x
        self.y = p.y
    
    def to_list(self, ):
        return [self.x, self.y]
    
    def offset(self, p):
        self.x += p.x
        self.y += p.y
