#include <iostream>
#include <vector>
#include <algorithm>
#include <map>


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

int height, width, mirror_no, coordinate_id = 0;
vector<vector<maze_point>> maze;
std::vector<bool> bit_mask;
vector<maze_point *> diamonds;
map<int, coordinate> map_coordinate;
map<int, coordinate>::iterator map_iterator;
vector<int> combination;
vector<int> choosen_combination;
//
vector<vector<vector<maze_point>>> mazes_with_question_marks;
vector<vector<vector<maze_point>>> mazes_with_mirrors;

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
    bit_mask.resize(mirror_no);

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
                if(maze[height_no][width_no].value == ' ') {
                    coordinate cord;
                    cord.height = height_no;
                    cord.width = width_no;
                    map_coordinate.insert(pair<int, coordinate>(coordinate_id, cord));
                    coordinate_id += 1;
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


bool checkIfMirrorSetupCollectsAllDiamonds(vector<vector<maze_point>> maze, vector<maze_point*> diamonds) {
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

// Mamy X miejsc w labiryncie gdzie moze byc lustro
// Mamy maksymalnie K luster
// Znajdujemy wszystkie K elementowe podzbiory X
// W ten sposob mamy odwzorowanie gdzie mozemy miec lustra
// Zaznaczamy to sobie np pytajnikiem
// Teraz bierzemy taki labirynt z pytajnikiem
// i generujemy wszystkie podzbiory juz ze wstawionymi odpoiwdnimi labiryntam
// dla tych labiryntow odpalamy checkIfMirrorSetupCollectsAllDiamonds

void putQuestionMarkIntoMaze(const vector<int>& v) {
    auto maze_to_return = maze;
//    cout << "combination no " << (++count) << ": ";
    for (int i = 0; i < v.size(); ++i) {
        map_iterator = map_coordinate.find(v[i]);
//        cout << endl;
//        cout << "HEIGHT" << map_iterator->second.height;
//        cout << "WIDTH" << map_iterator->second.width;
//        cout << endl;
        if( map_iterator == map_coordinate.end()) {
            // something went wrong
        }
        else {
            int height = map_iterator->second.height;
            int width = map_iterator->second.width;
            maze_to_return[height][width].value = '?';
//            cout << height << " " << width << " ";
        }
//        cout << v[i] << " ";

    }
//    cout << "] " << endl;
//    printMaze(maze_to_return);
    mazes_with_question_marks.push_back(maze_to_return);
//    cout << endl;
}


void findAllPossibleMirrorPlaces(int offset, int k) {
    if(k == 0 ) {
        putQuestionMarkIntoMaze(combination);
        return;
    }
    for (int i = offset; i <= choosen_combination.size() - k; ++i) {
        combination.push_back(choosen_combination[i]);
        findAllPossibleMirrorPlaces(i+1, k-1);
        combination.pop_back();
    }
}

// TODO: implemet
//vector<vector<vector<maze_point>>> findAllMirrorPlaces() {
//} // pytajnik zamienia na wszystkie mozliwe kombinacje / \


int main() {
    readInput();
//    constructGraph();
//    printNeighbours();


//    bool result = checkIfMirrorSetupCollectsAllDiamonds(maze, diamonds);
//    auto x = findAllPossibleMirrorPlaces(maze, 3);
//    cout << wrap
// per.maze[0][0] << endl;
//    printMaze(maze);
//    if(result) {
//        cout << "TAK" << endl;
//    } else {
//        cout << "NIE" << endl;
// }
    int n = coordinate_id;
    int k = mirror_no;
    for (int i = 0; i < n; ++i) { choosen_combination.push_back(i); }
    findAllPossibleMirrorPlaces(0, k);
    for(int i=0; i<mazes_with_question_marks.size(); i++) {
        printMaze(mazes_with_question_marks[i]);
        cout << endl;
    }
    return 0;
}
