from point import Point
from line import Line

class Street:
    name = ''
    vertexes = []
    segments = []
    def __init__(self, name, nodes):
        '''
        :param name:str
        :param nodes: [Point]
        '''
        self.name = name
        self.vertexes = nodes
        self.segments = []
        for index in range(1, len(nodes)):
            self.segments.append(Line(nodes[index-1], nodes[index]))

    def __repr__(self):
        return "{0} : {1}".format(self.name, self.segments)

    def __str__(self):
        return repr(self)

    def change(self, nodes):
        self.segments = []
        for index in range(1, len(nodes)):
            self.segments.append(Line(nodes[index-1], nodes[index]))

'''
p1 = Point(3,4)
p2 = Point(1,2)
p3 = Point(5,6)
p4 = Point(7,8)
street1 = Street("weber street", [p1, p2, p3, p4])
print(street1)
'''