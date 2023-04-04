#include <iostream>
#include <conio.h>
#include <vector>
using std::vector;
// function keys
#define ENTER 13
#define BACKSPACE 8
#define SPACEBAR 32
#define ESC 27
// size of grid
#define HEIGHT 15
#define WIDTH 15
// colours
#define CURS_COLOUR "\033[38;5;49m"
#define BLACK_FG "\033[38;5;235m"
#define RESET "\033[m"
// clr screen
#define clearScreen() std::cout << "\e[1;1H\e[2J";

// global variables
char grid[HEIGHT][WIDTH];
unsigned cursor_y = HEIGHT / 2, cursor_x = WIDTH / 2;
char previous_input;

// save x and y positions of 'X' plots
struct Positions {
    unsigned y_dot, x_dot;
    bool connection = true;
};

void initGrid() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = ' ';
        }
    }    
}
void printGrid() {
    clearScreen();
    for (unsigned i = 0; i < HEIGHT; i++) {
        for (unsigned j = 0; j < WIDTH; j++) {
            if (cursor_y == i && cursor_x == j) {
                std::cout << CURS_COLOUR << " " << '>' << " " << RESET; 
            } else { std::cout << " " << grid[i][j] << " "; }
        }
        std::cout << std::endl;
    } 
}
void moveCursor(vector<Positions> &pos) {
    char input;
    kbhit();
    input = tolower(getch());
    previous_input = input;
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
            grid[cursor_y][cursor_x] = '.';
            break;
        case BACKSPACE:
            grid[cursor_y][cursor_x] = ' ';
            break;
        case ENTER:
            // i don't know what this should be
            break;
        case ESC:
            // resets/clears grid
            pos.clear();
            initGrid();
            break;
        default:
            break;
    }
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
// searches for all 'X' in the grid and 
// stores the x and y positions
void findPositions(vector<Positions> &pos) {
    if (grid[cursor_y][cursor_x] == '.') {
        if (!repeatedPositions(pos)) { 
            // saves x and y coords in a new struct for every 'X'
            pos.push_back(Positions({cursor_y , cursor_x}));
        }
    } else if (previous_input == BACKSPACE) {
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

void connectHorizontal(Positions &head, Positions &tail) {
    bool found = false;
    for (unsigned x_iter = head.x_dot; x_iter < tail.x_dot; x_iter++) {
        if (cursor_y == head.y_dot && cursor_x == x_iter) {
            found = true;
        }
    }
    
    for (unsigned x_iter = head.x_dot; x_iter < tail.x_dot; x_iter++) {
        if (grid[head.y_dot][x_iter] == '.') { continue; } 
        if (found) {
            if (previous_input == ENTER) {
                head.connection = true;
                grid[head.y_dot][x_iter] = '-'; 
            } else if (previous_input == BACKSPACE) {
                head.connection = false;
                grid[head.y_dot][x_iter] = ' ';
            }
        }  
    }
}
void connectVertical(Positions &head, Positions &tail) {
    // this loop checks if the user wants to get rid or add a connection
    bool found = false;
    for (unsigned y_iter = head.y_dot; y_iter < tail.y_dot; y_iter++) {
        if (cursor_y == y_iter && cursor_x == head.x_dot) {
            found = true;
        }
    }

    for (unsigned y_iter = head.y_dot; y_iter < tail.y_dot; y_iter++) {
        if (grid[y_iter][head.x_dot] == '.') { continue; }
        if (found) {
            if (previous_input == ENTER) {
                grid[y_iter][head.x_dot] = '|';
            } else if (previous_input == BACKSPACE) { 
                grid[y_iter][head.x_dot] = ' '; 
            }
        }

    } 
}
void connectDiagonals(Positions &head, Positions &tail, int a = 1, int b = 1) {
    bool found = false;
    for (unsigned y_head = head.y_dot, x_head = head.x_dot, y_tail = tail.y_dot, x_tail = tail.x_dot; (y_head < y_tail || x_head < x_tail); y_head += a, x_head += b) { 
        if (cursor_y == y_head && cursor_x == x_head) {
            found = true;
        }
    }
    for (unsigned y_head = head.y_dot, x_head = head.x_dot, y_tail = tail.y_dot, x_tail = tail.x_dot; (y_head < y_tail || x_head < x_tail); y_head += a, x_head += b) { 
        if (grid[y_head][x_head] == '.') { continue; } // doesn't place on top of 'X'
        if (found) {
            if (previous_input == ENTER) {
                if (a > 0) {
                    grid[y_head][x_head] = '\\';
                } else {
                    grid[y_head][x_head] = '/';
                }
            } else if (previous_input == BACKSPACE) {
                grid[y_head][x_head] = ' '; 
            }
        }
    }
}
void findConnections(vector<Positions> &pos) {
    vector<Positions>::iterator i, j;
    for (i = pos.begin(); i != pos.end(); ++i) {
        for (j = pos.begin(); j != pos.end(); ++j) {
            // *i and *j are the struct 'Positions' 
            if ((*i).y_dot == (*j).y_dot) { // points are on the same row
                if ((*i).x_dot < (*j).x_dot) {
                    connectHorizontal(*i, *j);
                } else { connectHorizontal(*j, *i); }
            }
            if ((*i).x_dot == (*j).x_dot) { // points are on the same columnw
                if ((*i).y_dot < (*j).y_dot) {
                    connectVertical(*i, *j);
                } else { connectVertical(*j, *i); }
            }

            
            for (unsigned step = 1; step < HEIGHT; step++) {
                // point to top-left and bottom-right
                if ((*i).y_dot == (*j).y_dot - step && (*i).x_dot == (*j).x_dot - step) {
                    if ((*i).y_dot < (*j).y_dot && (*i).x_dot < (*j).x_dot) {
                        connectDiagonals(*i, *j);
                    } else { connectDiagonals(*j, *i); }
                }
                // point to top-right and bottom-left
                if ((*i).y_dot == (*j).y_dot - step && (*i).x_dot == (*j).x_dot + step) {
                    if ((*i).y_dot < (*j).y_dot && (*i).x_dot < (*j).x_dot) {
                        connectDiagonals(*i, *j, -1);
                    } else { connectDiagonals(*j, *i, -1); }
                }
            }
        }
    }
}

int main() {
    vector<Positions> pos;
    initGrid();
    while (1) {
        findPositions(pos);
        findConnections(pos);
        printGrid();
        std::cout << CURS_COLOUR << "WASD" << RESET << " to move" << std::endl;
        std::cout << '[' << CURS_COLOUR << "SPACEBAR" << RESET << ']' << " to place a vertex" << std::endl;
        std::cout << '[' << CURS_COLOUR << "ENTER" << RESET << ']' << " to connect vertices" << std::endl;
        std::cout << '[' << CURS_COLOUR << "BACKSPACE" << RESET << ']' << " to remove vertex or entire connection" << std::endl;
        std::cout << '[' << CURS_COLOUR << "ESC" << RESET << ']' << " to clear screen" << std::endl;
        // printPositions(pos); // just to test
        moveCursor(pos);
    }

    return EXIT_SUCCESS;
}