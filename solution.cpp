#include <iostream>
#include <string>
using namespace std;

int height, width, mirror_no;
char maze[100][100];

void readInput() {
    string line;
    cin >> height;
    cin >> width;
    cin >> mirror_no;
    getline(cin, line);
    for (int height_no = 0; height_no < height; height_no++) {
        getline(cin, line);
       for (int width_no = 0; width_no < width; width_no++) {
                maze[height_no][width_no] = line[width_no];
       }
    }
}

void printMaze() {
    for(int height_no=0; height_no<height; height_no++) {
        for(int width_no=0; width_no<width; width_no++) {
            cout << maze[height_no][width_no];
        }
        cout << endl;
    }
}

int main() {
    readInput();
    printMaze();
    return 0;
}
