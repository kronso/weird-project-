#include <iostream>
#include <conio.h>
#include <vector>
using std::vector;
// function keys
#define ENTER 13
#define BACKSPACE 8
#define SPACEBAR 32
// size of grid
#define HEIGHT 10
#define WIDTH 10
// colours
#define CURS_COLOUR "\033[38;5;49m"
#define RESET "\033[m"
// clr screen
#define clear() std::cout << "\033c";

char grid[HEIGHT][WIDTH];
int cursor_y = HEIGHT / 2, cursor_x = WIDTH / 2;
bool connect_dots = false;
bool removed_x = false;

void initGrid() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = '.';
        }
    }    
}
void printGrid() {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (cursor_y == i && cursor_x == j) {
                std::cout << CURS_COLOUR << grid[i][j] << RESET << "   ";
            } else {
                std::cout << grid[i][j] << "   ";
            }
        }
        std::cout << std::endl;
    } 
}
void moveCursor() {
    // resets
    removed_x = false;

    char input;
    kbhit();
    input = tolower(getch());
    switch(input) {
        case 'w':
            if (cursor_y > 0) { cursor_y -= 1; }
            break;
        case 'a':
            if (cursor_x > 0) { cursor_x -= 1; }
            break;
        case 's':
            if (cursor_y <(HEIGHT - 1)) { cursor_y += 1; }
            break;
        case 'd':
            if (cursor_x < (WIDTH - 1)) { cursor_x += 1; }        
            break;
        case SPACEBAR:
            grid[cursor_y][cursor_x] = 'X';
            break;
        case BACKSPACE:
            removed_x = true;
            grid[cursor_y][cursor_x] = '.';
            break;
        case ENTER:
            connect_dots = true;
            // connect dots 
            break;
        default:
            break;
    }
}
// save x and y positions of 'X' plots
struct Positions {
    int y_dot, x_dot;
};
// ensures the findPositions func doesn't store an existing pairs
bool repeatedPositions(vector<Positions> &pos) {
    vector<Positions>::iterator i = pos.begin();
    for (; i < pos.end(); i++) {
        if ((*i).y_dot == cursor_y && (*i).x_dot == cursor_x) {
            return true;
        }
    }
    return false;
}
// if 'X' is erased from the grid, so are the positions
void deletePositions(vector<Positions> &pos) {
    vector<Positions>::iterator i = pos.begin();
    for (; i < pos.end(); i++) {
        if ((*i).y_dot == cursor_y && (*i).x_dot == cursor_x) {
            pos.erase(i);
        }
    }
}
// searches for all 'X' in the grid and 
// stores the x and y positions
void findPositions(vector<Positions> &pos) {
    if (grid[cursor_y][cursor_x] == 'X') {
        if (!repeatedPositions(pos)) { 
            // saves x and y coords in a new struct for every 'X'
            // constructor inside 'push_back()'
            pos.push_back(Positions({cursor_y , cursor_x}));
        }
    } else if (removed_x) {
        deletePositions(pos);
    }
}
// just to let me see if its actually working lol
void printPositions(vector<Positions> &pos) {
    vector<Positions>::iterator i = pos.begin();
    for (; i < pos.end(); i++) {
        std::cout << "Y: " << (*i).y_dot << '\t';
        std::cout << "X: " << (*i).x_dot << std::endl;
    }
}

void connectDots() {
    // horizontal
    
}


int main() {
    vector<Positions> pos;
    initGrid();
    while (!connect_dots) {
        printGrid();
        findPositions(pos);
        printPositions(pos); // just to test
        moveCursor();
    }

    return EXIT_SUCCESS;
}