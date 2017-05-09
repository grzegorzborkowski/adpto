#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>


using namespace std;

struct maze_point {
    int height;
    int width;
    char value;
    vector<maze_point *> valid_neighbours;
    bool visited;
};

struct maze_wrapper {
    maze_point maze[100][100];
};

struct coordinate {
    int height;
    int width;
};

int height, width, mirror_no;
maze_point maze[100][100];
std::vector<bool> bit_mask(100);
vector<maze_point *> diamonds;

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
                maze[height_no][width_no].visited = false;
                if(maze[height_no][width_no].value == '*'){
                    diamonds.push_back(&maze[height_no][width_no]);
                }
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

void printMaze(maze_point maze_to_print[100][100]) {
    for(int height_no=0; height_no<height; height_no++) {
        for(int width_no=0; width_no<width; width_no++) {
            cout << maze_to_print[height_no][width_no].value;
        }
        cout << endl;
    }
}


/**
 *
 *  a)
 *  xxxxxxxx\
 *          x
 *          x
 *          x
 *
 *  left->right, \ => top->down
 *  down->top, \ =>  right->left
 *
 * b)
 *  x
 *  x
 *  x
 *  \ xxxxxxx
 *
 *  top->down, \ => left->right
 * right->left => down->top
 *
 * c)   x
 *      x
 *      x
 * xxxxx/
 *
 * top->down, / => right->left
 * left->right, / => down->top
 *
 * d)
 *
 *          /xxxxx
 *          x
 *          x
 *          x
 *
 *  right->left, / => top->down
 *  down->top, / => left->right
 * */


//bool checkIfMirrorSetupCollectsAllDiamonds(maze_point maze[100][100], vector<maze_point> diamonds) {
bool checkIfMirrorSetupCollectsAllDiamonds(maze_point maze[100][100], vector<maze_point*> diamonds) {
//    maze_point maze[100][100] = mazeWrapper.maze;
    int current_laser_height = 1;
    int current_laser_width = 1;
    bool left_to_right = true;
    bool right_to_left = false;
    bool top_to_down = false;
    bool down_to_top = false;
    int captured_diamonds = 0;
    while(captured_diamonds != diamonds.size()) {
        if(left_to_right) {
            // we reached the 'width' limit and didn't find a mirror on our way
            if (current_laser_width == width) {
                return false;
            }
            current_laser_width += 1;
            // case a)
            if(maze[current_laser_height][current_laser_width].value == '\\') {
                top_to_down = true;
                left_to_right = false;
            }
            // case c)
            else if(maze[current_laser_height][current_laser_width].value == '/') {
                down_to_top = true;
                left_to_right = false;
            }
        }
        // ---------------------------------
        else if(right_to_left) {
            if(current_laser_width == 0) {
                return false;
            }
            current_laser_width -= 1;
            // case b)
            if(maze[current_laser_height][current_laser_width].value == '\\') {
                down_to_top = true;
                right_to_left = false;
            }
            // case d)
            else if(maze[current_laser_height][current_laser_width].value == '/')  {
                top_to_down = true;
                right_to_left = false;
            }
        // ------------------------------------------------------
        } else if(top_to_down) {
            if(current_laser_height == height) {
                return false;
            }
            current_laser_height += 1;
            // case b)
            if(maze[current_laser_height][current_laser_width].value == '\\') {
                top_to_down = false;
                left_to_right = true;
            }
            // case c)
            else if(maze[current_laser_height][current_laser_width].value == '/') {
                top_to_down = false;
                right_to_left = true;
            }

        }
        // ------------------------------
        else if(down_to_top) {
            if(current_laser_height == 0) {
                return false;
            }
            current_laser_height -= 1;
            // case a
            if(maze[current_laser_height][current_laser_width].value == '\\') {
                down_to_top = false;
               right_to_left = true;
            }
            // case d
            else if(maze[current_laser_height][current_laser_width].value == '/') {
                down_to_top = false;
                left_to_right = true;
            }
        }

        // we found wall
        if(maze[current_laser_height][current_laser_width].value == '#') {
            return false;
        }

        // already been here
        if(maze[current_laser_height][current_laser_width].visited) {
            return false;
        }

        if(maze[current_laser_height][current_laser_width].value == ' '){
            maze[current_laser_height][current_laser_width].value = 'X';
        }
        maze[current_laser_height][current_laser_width].visited = true;

        // found diamond in this field of the maze
        for(auto item : diamonds) {
            if(item->height == current_laser_height && item->width == current_laser_width) {
                captured_diamonds += 1;
                if(captured_diamonds == diamonds.size()) {
                    return true;
                }
                continue;
            }
        }
    }
}

// http://www.cplusplus.com/forum/general/88692/
//maze_wrapper mirrorsNextPermutation(int numberofMirrors) {
//}

//void get_subset( std::vector<bool> bit_mask, std::size_t req_size )
//{
//    if( std::count( bit_mask.begin(), bit_mask.end(), true ) == req_size )
//    {
//        static int cnt = 0 ;
//        std::cout << ++cnt << ". [ " ;
//        for( std::size_t i = 0 ; i < bit_mask.size() ; ++i )
//            if( bit_mask[i] ) std::cout << i+1 << ' ' ;
//        std::cout << "]\n" ;
//    }
//}
//
//bool next_bitmask(vector<bool>& bit_mask) {
//    std::size_t i = 0 ;
//    for( ; ( i < bit_mask.size() ) && bit_mask[i] ; ++i )
//        bit_mask[i] = false ;
//
//    if( i < bit_mask.size() ) { bit_mask[i] = true ; return true ; }
//    else return false ;
//}
//
//
//void getMazeWithRandomlyPlacedMirrors() {
//    map<int, coordinate>::iterator it;
//    int number_of_possible_mirrors = 0;
//    map<int, coordinate> mapka;
//    maze_point maze_to_return[100][100];
//    for(int height_iter = 0; height_iter<height; height_iter++) {
//        for(int weight_iter=0; weight_iter<weight; weight_iter++) {
//            maze_to_return[height_iter][weight_iter] = maze[height_iter][weight_iter];
//            // we can have a mirror here
//            if(maze_to_return[height_iter][weight_iter].value == ' ') {
//                coordinate cord;
//                cord.height = height_iter;
//                cord.width = weight_iter;
//                mapka.insert(pair<int, coordinate>(number_of_possible_mirrors, cord));
//                number_of_possible_mirrors += 1;
//            }
//        }
//    }
//
//    if(next_bitmask(bit_mask)) {
//        for( std::size_t i = 0 ; i < bit_mask.size() ; ++i )
//            if( bit_mask[i] ) {
//                it = mapka.find(i);
//                if (it != mapka.end()) {
//                    int height_of_mirror = it -> height;
//                    int width_of_mirror = it -> width;
//                    maze_to_return[height_of_mirror][width_of_mirror].value = '?';
//                }
//            }
//    }
//}

int main() {
    readInput();
//    constructGraph();
//    printMaze();
//    printNeighbours();


    bool result = checkIfMirrorSetupCollectsAllDiamonds(maze, diamonds);
//    cout << wrap
// per.maze[0][0] << endl;
    printMaze(maze);
    if(result) {
        cout << "TAK" << endl;
    } else {
        cout << "NIE" << endl;
    }
    return 0;
}
