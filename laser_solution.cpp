#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <bitset>
using namespace std;

struct maze_point {
    int height;
    int width;
    char value;
     bool visited;
};

struct coordinate {
    int height;
    int width;
};

int height, width, mirror_no;
vector<vector<maze_point>> maze;
std::vector<bool> bit_mask;
vector<maze_point *> diamonds;
vector<vector<maze_point>> solution;
bool found_solution = false;

void readInput() {
    string line;
    cin >> height;
    cin >> width;
    cin >> mirror_no;
    getline(cin, line);
    maze.resize(height);
     for(int i=0; i<height; i++) {
            maze[i].resize(width);
     }

    for (int height_no = 0; height_no < height; height_no++) {
        getline(cin, line);
       for (int width_no = 0; width_no < width; width_no++) {
                maze[height_no][width_no].height = height_no;
                maze[height_no][width_no].width = width_no;
                maze[height_no][width_no].value = line[width_no];
                if(maze[height_no][width_no].value == '*'){
                    diamonds.push_back(&maze[height_no][width_no]);
                }
       }
    }
}

void printMaze(vector<vector<maze_point>> maze_to_print) {
    for(int height_no=0; height_no<height; height_no++) {
        for(int width_no=0; width_no<width; width_no++) {
            cout << maze_to_print[height_no][width_no].value;
        }
        cout << endl;
    }
}

bool checkIfMirrorSetupCollectsAllDiamonds(vector<vector<maze_point>> maze, vector<maze_point*> diamonds) {
    int current_laser_height = 1;
    int current_laser_width = 0;
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

//        if(maze[current_laser_height][current_laser_width].value == ' '){
//            maze[current_laser_height][current_laser_width].value = 'X';
//        }
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

bool is_starting_point(coordinate cord) {
    if(cord.height == 1 && cord.width == 0) {
        return true;
    }
    return false;
}

void solve(vector<vector<maze_point>> maze, coordinate current_coordinate, int mirrors_left) {
//    if(current_coordinate.height == height-1 && current_coordinate.width == width-1) return; // reached the limit
    // crossed the maze boundaries
    if(current_coordinate.height == height || current_coordinate.height < 0 || current_coordinate.width == width || current_coordinate.width < 0) {
        return;
    }
//    cout << current_coordinate.height << " " << current_coordinate.width << " " << endl;
//    printMaze(maze);

    if(found_solution) {
       return;
    }

    if(checkIfMirrorSetupCollectsAllDiamonds(maze, diamonds)) {
           found_solution = true;
           solution = maze;
           return;
    }

    if(mirrors_left == 0) {
        return;
    }
    // we need to go deeper
    else {
    coordinate down_coordinate, right_coordinate, left_coordinate, top_coordinate;
    down_coordinate.width = current_coordinate.width;
    down_coordinate.height = current_coordinate.height + 1;
    right_coordinate.width = current_coordinate.width + 1;
    right_coordinate.height = current_coordinate.height;
    left_coordinate.width = current_coordinate.width - 1;
    left_coordinate.height = current_coordinate.height;
    top_coordinate.width = current_coordinate.width;
    top_coordinate.height = current_coordinate.height - 1;
    if(maze[current_coordinate.height][current_coordinate.width].value == ' ' && !is_starting_point(current_coordinate)) {
        maze[current_coordinate.height][current_coordinate.width].value = '/';
        solve(maze, down_coordinate, mirrors_left-1);
        solve(maze, right_coordinate, mirrors_left-1);
//        solve(maze, top_coordinate, mirrors_left-1);
//        solve(maze, left_coordinate, mirrors_left-1);
        maze[current_coordinate.height][current_coordinate.width].value = '\\';
        solve(maze, down_coordinate, mirrors_left-1);
        solve(maze, right_coordinate, mirrors_left-1);
//        solve(maze, top_coordinate, mirrors_left-1);
//        solve(maze, left_coordinate, mirrors_left-1);
        maze[current_coordinate.height][current_coordinate.width].value = ' ';
        solve(maze, down_coordinate, mirrors_left);
        solve(maze, right_coordinate, mirrors_left);
//        solve(maze, top_coordinate, mirrors_left);
//        solve(maze, left_coordinate, mirrors_left);
    } else {
        solve(maze, down_coordinate, mirrors_left);
        solve(maze, right_coordinate, mirrors_left);
//        solve(maze, top_coordinate, mirrors_left);
//        solve(maze, left_coordinate, mirrors_left);
        }
    }
}



int main() {
    readInput();
    cout << height << " " << width << endl;
    coordinate start_coordinate;
    start_coordinate.height = 0;
    start_coordinate.width = 0;
    solve(maze, start_coordinate, mirror_no);
    if(found_solution) {
        cout << mirror_no << endl;
        printMaze(solution);
    }
    return 0;
}
