#!/usr/bin/env python

import sys
import re
from line import Line
from point import Point
from street import Street

# YOUR CODE GOES HERE
streets = []  # database of streets [Street]
vertexes = []  # database of vertexes that required [Point]
segments = []  # database of segments [Line]
e_output = []  # database of segments that required [Line]
itersecs = []  # database of intersections [Point]


def parser(input_string):
    global streets
    input_str = input_string.replace(' ', '')
    input_str = input_str.replace('\n', '')

    if input_str[0] == 'a':
        te = input_string.split('"')
        if te[0][1] != ' ' or te[2][0] != ' ':
            sys.stderr.write("Error: Lack of space\n")
            return
        check = re.compile(r'a"[A-Za-z0-9]+"([(][-]?[0-9]+,[-]?[0-9]+[)])+$')
        if check.match(input_str).group() == input_str:
            inputs = input_str.split('"')
            new_streetname = input_string.split('"')[1].lower()  # judge if the new street already exist
            for street in streets:
                if new_streetname == street.name:
                    sys.stderr.write("Error: This Street already exists.\n")
                    return
            pattern = re.compile(r'[-]?[0-9]+,[-]?[0-9]+')
            inputpoints = pattern.findall(inputs[2])
            innerpoints = []
            for point in inputpoints:
                temp = point.split(',')
                innerpoints.append(Point(float(temp[0]), float(temp[1])))
            add_street(new_streetname, innerpoints)
        else:
            sys.stderr.write("Error: Illegal input\n")

    elif input_str[0] == 'c':
        te = input_string.split('"')
        if te[0][1] != ' ' or te[2][0] != ' ':
            sys.stderr.write("Error: Lack of space.\n")
            return
        check = re.compile(r'c"[A-Za-z0-9]+"([(][-]?[0-9]+,[-]?[0-9]+[)])+$')
        if check.match(input_str).group() == input_str:
            new_steertname = input_string.split('"')[1].lower()
            findit = False
            for street in streets:
                if street.name == new_steertname:
                    findit = True
                    break
            if not findit:
                sys.stderr.write("Error: This street doesn't exist.\n")
                return
            pattern = re.compile(r'[-]?[0-9]+,[-]?[0-9]+')
            inputpoints = pattern.findall(input_str.split('"')[2])
            points = []
            for point in inputpoints:
                temp = point.split(',')
                points.append(Point(float(temp[0]), float(temp[1])))
            change_street(new_steertname, points)
        else:
            print(False)

    elif input_str[0] == 'r':
        streets = []
        p = Point(0, 0)
        p.rename()

        # if input_string[1] != ' ':
        #     sys.stderr.write("Error: Lack of space.\n")
        #     return
        # check = re.compile(r'r"[A-Za-z0-9]+"$')
        # if check.match(input_str).group() == input_str:
        #     rstrname = input_string.split('"')[1].lower()
        #     for street in streets:
        #         if street.name == rstrname:
        #             remove_street(rstrname)
        #             return
        #     sys.stderr.write("Error: This street doesn't exist\n")

    elif input_str[0] == 'g':
        graph_output()
    else:
        sys.stderr.write('Error: Wrong command\n')
    # except AttributeError:
    #     sys.stderr.write("Error: Illegal input\n"+input_str)
    # except Exception:
    #     sys.stderr.write("Error: Illegal input\n")


def processing_graph():
    global vertexes
    global segments
    global e_output
    global itersecs

    vertexes = []
    segments = []
    e_output = []
    itersecs = []
    for street in streets:
        for newseg in street.segments:
            for oldseg in segments:
                if check_intersection(newseg, oldseg) == 1:
                    newiter = calculate_intersection(newseg, oldseg)
                    oldseg.add_inter(newiter)
                    newseg.add_inter(newiter)
                elif check_intersection(newseg, oldseg) == 2:
                    points = [newseg.dst, newseg.src, oldseg.dst, oldseg.src]
                    if newseg.src.x != newseg.dst.x:
                        points.sort(key=lambda y: y.x)
                    else:
                        points.sort(key=lambda x: x.y)

                    # if two segments are parallel, add the median two vertices in both two edges
                    oldseg.add_inter(points[1])
                    oldseg.add_inter(points[2])
                    newseg.add_inter(points[1])
                    newseg.add_inter(points[2])
                else:
                    pass
        # after finish one intersection check, add all segments of new street in segments[]
        segments += street.segments
    # after finished all the intersection checks, get the answer of all the v and e
    for seg in segments:
        new_v = seg.outputpoint()
        for v in new_v:
            if v not in vertexes:
                vertexes.append(v)
        new_seg = seg.outputline()
        for s in new_seg:
            if s not in e_output:
                e_output.append(s)


def add_street(street_name, nodes):
    """
    add street to database and process
    :param street_name: str
    :param nodes: [Point]
    """
    streets.append(Street(street_name, nodes))


def change_street(street_name, nodes):
    """
    change the information of street
    :param street_name: str
    :param nodes: [Point]
    """
    if not nodes:
        print("Empty nodes")
        return False
    for street in streets:
        if street.name == street_name:
            street.change(nodes)
    # processing_graph()


def remove_street(street_name):
    """
    remove a street from database
    :param street_name: str
    :return boolean
    """
    for street in streets:
        if street.name == street_name:
            streets.remove(street)
            processing_graph()
            return True
    return False


def check_intersection(line1, line2):
    """

    :param line1: Line
    :param line2: Line
    :return:
    """

    def parallel(lin1, lin2):
        x1, y1 = lin1.src.x, lin1.src.y
        x2, y2 = lin1.dst.x, lin1.dst.y
        x3, y3 = lin2.src.x, lin2.src.y
        x4, y4 = lin2.dst.x, lin2.dst.y

        if (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4) == 0:
            return True
        else:
            return False

    def cross(p1, p2, p3):
        x1 = p2.x - p1.x
        y1 = p2.y - p1.y
        x2 = p3.x - p1.x
        y2 = p3.y - p1.y
        return x1 * y2 - x2 * y1

    p1, p2 = line1.src, line1.dst
    p3, p4 = line2.src, line2.dst

    if (max(p1.x, p2.x) >= min(p3.x, p4.x)
            and max(p3.x, p4.x) >= min(p1.x, p2.x)
            and max(p1.y, p2.y) >= min(p3.y, p4.y)
            and max(p3.y, p4.y) >= min(p1.y, p2.y)):

        if (cross(p1, p2, p3) * cross(p1, p2, p4) <= 0
                and cross(p3, p4, p1) * cross(p3, p4, p2) <= 0):
            if not parallel(line1, line2):
                D = 1
            else:
                D = 2
        else:
            D = 0
    else:
        D = 0
    return D


def calculate_intersection(line1, line2):
    """
    calculate the intersection of line1 and line2
    :param line1: Line
    :param line2: Line
    :return: Point
    """
    x1, y1 = line1.src.x, line1.src.y
    x2, y2 = line1.dst.x, line1.dst.y
    x3, y3 = line2.src.x, line2.src.y
    x4, y4 = line2.dst.x, line2.dst.y

    xnum = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))
    xden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
    xcoor = xnum / xden

    ynum = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
    yden = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
    ycoor = ynum / yden

    return Point(xcoor, ycoor)


def graph_output():
    processing_graph()
    # rename the vertex from 0 to n
    for seg in e_output:
        for ver in vertexes:
            if seg.dst == ver:
                seg.dst = ver
            if seg.src == ver:
                seg.src = ver

    for i in range(len(vertexes)):
        vertexes[i].set_name(i)
    print ('V ' + str(vertexes[-1].index + 1))
    print ('E ' + '{' + str(e_output)[1:-1].replace(' ', '') + '}')
    sys.stdout.flush()


def main():
    ### YOUR MAIN CODE GOES HERE

    ### sample code to read from stdin.
    ### make sure to remove all spurious print statements as required
    ### by the assignment
    while True:
        line = sys.stdin.readline()
        if line == '' or line == "\n":
            break
        parser(line)
        # print 'read a line:', line

    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == '__main__':
    main()
