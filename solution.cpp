#include <iostream>
#include <vector>

using namespace std;

struct maze_point {
    int height;
    int width;
    char value;
    vector<maze_point *> valid_neighbours;
};

int height, width, mirror_no;
maze_point maze[100][100];

void readInput() {
    string line;
    cin >> height;
    cin >> width;
    cin >> mirror_no;
    getline(cin, line);
    for (int height_no = 0; height_no < height; height_no++) {
        getline(cin, line);
       for (int width_no = 0; width_no < width; width_no++) {
                maze[height_no][width_no].height = height_no;
                maze[height_no][width_no].width = width_no;
                maze[height_no][width_no].value = line[width_no];
       }
    }
}

bool bothequalzero(int x, int y) {
    return x == 0 && y == 0;
}


void constructGraph() {
    for(int height_no=0; height_no<height; height_no++) {
        for(int width_no=0; width_no<width; width_no++) {
            for(int x=-1; x<=1; x++) {
                for(int y=-1; y<=1; y++) {
                    /** valid point in a maze, different from t */
                    if(x+width_no >= 0 && x+width_no < width && y+height_no>=0 && y+height_no<height
                       && !bothequalzero(x, y)) {
                        if(maze[height_no+y][width_no+x].value != '#') {
                            maze[height_no][width_no].valid_neighbours.push_back(&maze[height_no+y][width_no+x]);
                        }
                    }
                }
            }
        }
    }
}



void printNeighbours() {
    for(int height_no=0; height_no<height; height_no++) {
        for(int width_no=0; width_no<width; width_no++) {
            cout << "HEIGHT " << height_no << " WIDTH: " << width_no << " ";
            for(auto& values : maze[height_no][width_no].valid_neighbours) {
                cout << "(" << values->height << " " << values->width << "), ";
            }
            cout << endl;
        }
    }
}

void printMaze() {
    for(int height_no=0; height_no<height; height_no++) {
        for(int width_no=0; width_no<width; width_no++) {
            cout << maze[height_no][width_no].value;
        }
        cout << endl;
    }
}

int main() {
    readInput();
    constructGraph();
    printMaze();
    printNeighbours();
    return 0;
}
