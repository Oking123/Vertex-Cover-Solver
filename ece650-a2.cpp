// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
using namespace std;


void shortest_path(const vector<vector<int>>& graph, int src, int dst) {
    // print the shortest path from src to dst by a function, and the algorithm is from ECE606
    int size = graph.size();
    int color[size];    //  save the color of each vertex in graph,0 for white, 1 for grey, 2 for black
    int d[size];        //  d[i] = the distance between vertex i and src
    int pi[size];       //  the parent vertex of i, -1 means no parent

    // initialize the graph
    for (int i = 0; i < size; i++) {
        color[i] = 0;
        d[i] = -1;      //  -1 means not be searched
        pi[i] = -1;     //  -1 means no parent
    }

    //begin BFS
    color[src] = 1;
    d[src] = 1;
    pi[src] = -1;
    queue<int> Q;
    Q.push(src);

    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();
        for (int adj:graph[u]) {
            if (color[adj] == 0) {
                color[adj] = 1;
                d[adj] = d[u] + 1;
                pi[adj] = u;
                Q.push(adj);
            }
        }
        color[u] = 2;
    }

    if (pi[dst] == -1) {
        cerr << "Error: there is no shortest path from " << src << " to " << dst << endl;
    } else {
        vector<int> path;     //  print the answer in the format of "vertex-vertex"
        while (dst != -1) {
            path.push_back(dst);
            dst = pi[dst];
        }
        while (!path.empty()) {
            cout << path[path.size() - 1];
            path.pop_back();
            if (!path.empty()) {
                cout << '-';
            }
        }
        cout <<endl;
        cout.flush();
    }
}

int main() {
    int sum = 0;   // save the number of vertexes of all the graph
    vector<vector<int>> graph;  //  save the graph by a 2-D vector

    // read from stdin until EOF
    while (!std::cin.eof()) {

        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);

        if(line == "exit"){
            break;
        }

        //  terminate the program when read a EOF
        if(cin.eof()){
            break;
        }

        // if input is NIL, print an Error and continue
//        if(line.empty()){
//            cerr << "Error: No input"<<endl;
//            continue;
//        }

        // create an input stream based on the line
        // we will use the input stream to parse the line
        std::istringstream input(line);
        char opt;
        input >> opt;  // input the first character and decide what to do
        switch (opt) {
            //  initialize the graph with sum vertexes
            case 'V': {
                std::cout<<line<<std::endl;
                graph.clear();
                input >> sum;
                graph.resize(sum);
                for (int i = 0; i < sum; i++) {
                    graph[i] = {};
                }
                break;
            }
            //  input the edge info and store in a 2D vector
            case 'E': {
                std::cout<<line<<std::endl;
                while (!input.eof()) {
                    int num1, num2;
                    char separator;
                    // parse two integers together
                    input >> separator;
                    if (separator == '<') {
                        input >> num1;
                        input >> separator;
                        input >> num2;
                        if (num1 >= sum || num2 >= sum) {
                            cerr << "Error: Out of index." << endl;
                            break;
                        } else {
                            graph[num1].push_back(num2);
                            graph[num2].push_back(num1);
                        }
                    }
                    if (input.eof())
                        break;
                }
                break;
            }
            //  get the src(start vertex) and dst(destination vertex) and output the result
            case 's': {
                int src, dst;
                input >> src;
                input >> dst;
                if (src >= sum || dst >= sum) {
                    cerr << "Error: the vertex doesn't exist." << endl;
                }else if(src == dst){
                    cout << src<<'-'<<dst<<endl;
                }
                else{
                    shortest_path(graph, src, dst);
                }
                break;
            }
            default: {
                cerr << "Error: illegal input"<<endl;
                break;
            }
        }
    }
    return 0;
}