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
#define BLACK_FG "\033[38;5;235m"
#define RESET "\033[m"
// clr screen
#define clear() std::cout << "\033c";

// global variables
char grid[HEIGHT][WIDTH];
unsigned int cursor_y = HEIGHT / 2, cursor_x = WIDTH / 2;
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
    for (unsigned int i = 0; i < HEIGHT; i++) {
        for (unsigned int j = 0; j < WIDTH; j++) {
            if (cursor_y == i && cursor_x == j) {
                std::cout << CURS_COLOUR << grid[i][j] << RESET << "   ";
            } else if (grid[i][j] == '|' || grid[i][j] == '-' || grid[i][j] == 'X'){
                std::cout << BLACK_FG << grid[i][j] << RESET << "   ";
            } else { std::cout << grid[i][j] << "   "; }
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
            removed_x = true; // if a 'X' was erased
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
    unsigned int y_dot, x_dot;
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
        // it just lets me see the position my cursor is on :)
        if (cursor_y == (*i).y_dot && cursor_x == (*i).x_dot) {
            std::cout << CURS_COLOUR << "Y: " << (*i).y_dot << '\t';
            std::cout << "X: " << (*i).x_dot << RESET << std::endl;
        } else {
            std::cout << "Y: " << (*i).y_dot << '\t';
            std::cout << "X: " << (*i).x_dot << std::endl;
        }

    }
}

void connectHorizontal(Positions head, Positions tail) {
    unsigned x_begin = head.x_dot + 1, x_end = tail.x_dot;
    for (; x_begin < x_end; x_begin++) {
        grid[head.y_dot][x_begin] = '_';
    }
}

void connectVertical(Positions head, Positions tail) {
    unsigned y_begin = head.y_dot + 1, y_end = tail.y_dot;
    for (; y_begin < y_end; y_begin++) {
        grid[y_begin][head.x_dot] = '|';
    }
    
}

void findConnects(vector<Positions> &pos) {

    vector<Positions>::iterator i;
    vector<Positions>::iterator j;
    for (i = pos.begin(); i != pos.end(); ++i) {
        for (j = pos.begin(); j != pos.end(); ++j) {
            if ((*i).y_dot == (*j).y_dot) { // points are on the same row
                if ((*i).x_dot < (*j).x_dot) {
                    connectHorizontal(*i, *j);
                } else { connectHorizontal(*j, *i); }
            }
            if ((*i).x_dot == (*j).x_dot) { // points are on the same column
                if ((*i).y_dot < (*j).y_dot) {
                    connectVertical(*i, *j);
                } else { connectVertical(*j, *i); }
            }
        }
    }
}

int main() {
    vector<Positions> pos;
    initGrid();
    while (!connect_dots) {
        findPositions(pos);
        printGrid();
        printPositions(pos); // just to test
        moveCursor();
    }
    findConnects(pos);
    printGrid();

    return EXIT_SUCCESS;
}