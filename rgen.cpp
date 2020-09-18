//
// Created by jzx94 on 2019/10/26.
//
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>





//  return a random number between in [low, high]
int random(int low, int high){
    if(low == high){
        return low;
    }
    std::ifstream urandom("/dev/urandom");
    if(urandom.fail()){
        std::cerr<<"Error:cannot open /dev/urandom"<<std::endl;
    }
    if(low>high){
        std::cerr<<"Error:random() error"<<std::endl;
    }
    int result = 0;
    urandom.read((char*)&result, sizeof(int));
    return low+abs(result)%(high-low+1);
}



class point{
    public:
        int x;
        int y;
        point(int x, int y) : x(x), y(y) {}

        bool operator==(const point &rhs) const {
            return x == rhs.x &&
                   y == rhs.y;
        }

        bool operator!=(const point &rhs) const {
            return !(rhs == *this);
        }

        friend std::ostream &operator<<(std::ostream &os, const point &point) {
            os << "(" << point.x << "," << point.y<<")";
            return os;
        }
};


class line{
    public:
        point src;
        point dst;
        line(const point &src, const point &dst) : src(src), dst(dst) {
        }

        bool operator==(const line &rhs) const {
            return (src == rhs.src &&
                   dst == rhs.dst )||
                   ( src == rhs.dst &&
                   dst == rhs.src);
        }

        bool operator!=(const line &rhs) const {
            return !(rhs == *this);
        }

        friend std::ostream &operator<<(std::ostream &os, const line &line) {
            os << "src: " << line.src << " dst: " << line.dst;
            return os;
        }
};

// following algorithm for intersection check is from https://www.geeksforgeeks.org/orientation-3-ordered-points/
bool onSegment(point p, point q, point r)
{
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(point p, point q, point r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}
// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(point p1, point q1, point p2, point q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}
std::vector<line> all_segments;
int startstreet = 0;

class street{
    public:
        std::string name;
        std::vector<point> vertexes;
        std::vector<line> in_segment;
        int segments_num;
        int range;


        street(int segments_num, int coor_range){
            this->segments_num = random(1, segments_num);
            this->range = coor_range;
            this->name = "street" + std::to_string(startstreet);
            startstreet++;
        }

        //check if two lines are parallel
        static bool parallel(const line& lin1, const line& lin2) {
            int x1 = lin1.src.x;
            int y1 = lin1.src.y;
            int x2 = lin1.dst.x;
            int y2 = lin1.dst.y;
            int x3 = lin2.src.x;
            int y3 = lin2.src.y;
            int x4 = lin2.dst.x;
            int y4 = lin2.dst.y;

            return (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4) == 0;
        }



        //check if two lines intersects
        static bool intersection(const line &l1, const line &l2)
        {return doIntersect(l1.src, l1.dst, l2.src, l2.dst);};

        static bool contains(const line& l1, const line& l2){
            if(intersection(l1,l2)&&parallel(l1,l2)){
                if(l1.src.x==l1.dst.x){
                    if((l1.src.y<=l2.src.y&&l1.dst.y>=l2.dst.y)or(l2.src.y<=l1.src.y&&l2.dst.y>=l1.dst.y)){
                        return true;
                    }
                }else{
                    if((l1.src.x<=l2.src.x&&l1.dst.x>=l2.dst.x)or(l2.src.x<=l1.src.x&&l2.dst.x>=l1.dst.x)){
                        return true;
                    }
                }
            }
            return false;
        }
        //check if  newlines meets all the requirements
        bool check(const line &li){
            if(li.dst == li.src) return false;

            //check if temp in all_segments
            for(line temp:all_segments){
                if(contains(temp, li)) {
                    return false;}
            }

            //check if temp intersects with in_segment
            std::vector<line> temp = in_segment;
            if(parallel(li, in_segment[in_segment.size()-1])) return false;
            temp.pop_back();
            for(line te:temp){
                if(intersection(te, li)) {
                    return false;
                }
            }
            return true;

        }

        //generate a single street
        bool construct(){
            int maxattempts = 0;
            point A(random(-range, range), random(-range,range));
            point B(random(-range, range), random(-range,range));
            while(maxattempts<=25) {
                if (A != B) {
                    in_segment.emplace_back(A, B);
                    vertexes.push_back(A);
                    vertexes.push_back(B);
                    break;
                }else{
                    B = point(random(-range, range), random(-range,range));
                    maxattempts++;
                }
            }


            for(int i = 1;i<segments_num;i++) {
                while (maxattempts <= 25) {
                    point C(random(-range, range), random(-range, range));
                    line newline(vertexes[vertexes.size()-1], C);
                    if(check(newline)){
                        vertexes.push_back(C);
                        in_segment.push_back(newline);
                        break;
                    }else{
                        maxattempts++;
                    }
                if(maxattempts>=25) return false;
            }
            }
            return true;
        }

        //add all the in_segments to all_segments
        void add_street(){
            for(line seg:in_segment){
                all_segments.push_back(seg);
            }
        }

        bool check_goodgraph(){
            for(line in:in_segment){
                for(line out:all_segments){
                    if(intersection(in, out)){
                        return true;}
                }
            }
            return false;
        }
        void output(){
            std::cout<<"a \""<<name<<"\" ";
            for(point temp:vertexes){
                std::cout<<temp;
            }
            std::cout<<std::endl;

        }
        friend std::ostream &operator<<(std::ostream &os, const street &street) {
            os << "name: " << street.name;
            return os;
        }



};


int main(int argc, char **argv){
    int sint_value = 10;
    int nint_value = 5;
    int lint_value = 5;
    int cint_value = 20;
    int temp = 0;
    std::string svalue;
    std::string nvalue;
    std::string lvalue;
    std::string cvalue;
    int c;

    opterr = 0;
    while((c = getopt(argc, argv,"s:n:l:c:"))!=-1){
        switch(c){
            case 's':
                svalue = optarg;
                temp = atoi(svalue.c_str());
                if(temp>=2) sint_value=temp;
                break;
            case 'n':
                nvalue = optarg;
                temp = atoi(nvalue.c_str());
                if(temp>=1) nint_value=temp;
                break;
            case 'l':
                lvalue = optarg;
                temp = atoi(lvalue.c_str());
                if(temp>=5) lint_value=temp;
                break;
            case 'c':
                cvalue = optarg;
                temp = atoi(cvalue.c_str());
                if(temp>=1) cint_value=temp;
                break;
            case '?':
                std::cerr << "Error: unknown option: " << optopt << std::endl;
                break;
            default:
                return 0;
        }
    }
    int street_sum;
    int segment_max =nint_value;
    int waittime;
    int coor_range = cint_value;

    int maxattemp = 0;
    startstreet = 0;
    std::vector<street> streets;
    while(true){
        // remove all the streets before
        all_segments.clear();
        bool good_graph = false;// whether the output graph have intersections

        while(!streets.empty()){
            street street_temp = streets[streets.size()-1];
            streets.pop_back();
            std::cout<<"r \""<<street_temp.name<<"\""<<std::endl;
        }
        street_sum = random(2, sint_value);
        maxattemp = 0;
        startstreet = 0;
        for(int i=0;i<street_sum - 1;i++){
            while(maxattemp<=25){
                street newstreet(segment_max, coor_range);
                if(newstreet.construct()){
                    good_graph = good_graph||newstreet.check_goodgraph();
//                    std::cout<<good_graph<<std::endl;
                    newstreet.add_street();
                    newstreet.output();
                    streets.push_back(newstreet);
                    break;
                }else{
                    maxattemp++;
                }
            }
            if(maxattemp >= 25){
                std::cerr<<"Error: failed to generate valid input for 25 simultaneous attempts"<<std::endl;
                return 1;
            }
        }

        //generate the last street
        maxattemp = 0;
        while(maxattemp <= 25
        ){
            street newstreet(segment_max, coor_range);
            if(newstreet.construct()){
                good_graph = good_graph||newstreet.check_goodgraph();
//                std::cout<<good_graph<<std::endl;
//                newstreet.output();
                if(good_graph){
                    newstreet.output();
                    streets.push_back(newstreet);
                    break;
                }
            }
            maxattemp++;
        }
        if(maxattemp >= 25){
            std::cerr<<"Error: failed to generate valid input for 25 simultaneous attempts"<<std::endl;
            return 1;
        }

        std::cout<<'g'<<std::endl;
        std::cout.flush();
        waittime = random(5, lint_value);
        sleep(waittime);
    }
    return 0;
}