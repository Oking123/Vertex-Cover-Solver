name = 1
points = []
class Point(object):
    x = 0
    y = 0
    index = 0

    def __init__(self, x, y):
        '''
        :param x: float
        :param y: float
        '''
        for point in points:
            if point.x == x and point.y == y:
                self.x = x
                self.y = y
                self.index = point.index
                return
        self.x = x
        self.y = y
        global name
        self.index = name
        name += 1
        points.append(self)

    def __repr__(self):
        return "{2}:({0:.2f},{1:.2f})".format(self.x, self.y, self.index)

    def __str__(self):
        return repr(self)

    def set_name(self, index):
        self.index = index

    def get_name(self):
        return self.index

    def __eq__(self, other):
        return self.__dict__ == other.__dict__

    def rename(self):
        global name
        name = 0
        global points
        points = []

'''
p = Point(3 ,4)
print(p)
p.set_name(8)
print(p.index)
'''