from point import Point


class Line(object):

    def __init__(self, p1, p2):
        '''
        :param p1: Point
        :param p2: Point
        '''
        if p1 == p2:
            raise Exception("Error: Coordinates can not be same.")
        self.src = p1
        self.dst = p2
        self.inner_inter = []

    def add_inter(self, p1):
        if p1 not in self.inner_inter:
            self.inner_inter.append(p1)

    def outputpoint(self):
        if not self.inner_inter:
            return []
        else:
            if self.dst not in self.inner_inter:
                self.inner_inter.append(self.dst)
            if self.src not in self.inner_inter:
                self.inner_inter.append(self.src)
        return self.inner_inter[:]

    def outputline(self):
        ans = []
        if not self.inner_inter:
            return []
        else:
            if self.dst not in self.inner_inter:
                self.inner_inter.append(self.dst)
            if self.src not in self.inner_inter:
                self.inner_inter.append(self.src)
            if self.src.x != self.dst.x:
                self.inner_inter.sort(key=lambda y: y.x)
            else:
                self.inner_inter.sort(key=lambda x: x.y)
            for i in range(len(self.inner_inter)-1):
                ans.append(Line(self.inner_inter[i], self.inner_inter[i+1]))
        return ans[:]

    def __repr__(self):
        return "<{0},{1}>".format(self.src.index, self.dst.index)

    def __str__(self):
        return repr(self)

    def __eq__(self, other):
        if self.__dict__ == other.__dict__:
            return True
        elif self.dst == other.src and self.src == other.dst:
            return True
        else:
            return False


