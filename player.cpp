#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#include <set>
#define score 20

using namespace std;
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;
struct position{
    int row;
    int column;
    position(int row, int column):row(row), column(column){}
};

vector<position> blacky;
vector<position> whity;
set<position> possible_new_me;
int horizontal[15][15], vertical[15][15], right_up[15][15], left_up[15][15];
int position_score[15][15];



void record_position(int id, int i, int j){
    if(id == 1){
        position temp(i, j);
        blacky.push_back(temp);
    }
    else if (id == 2){
        position temp(i, j);
        whity.push_back(temp);
    }
}

void statefunction(int id){
    int appear, flag;
    for(auto c: (id == 1?blacky:whity)){
        appear = 0;
        for(int i = -4; i <= 4; i++){
            if(c.column+i >= 0 && c.column+i < 15) {
                //horizontal[c.row][c.column+i] += score;
                flag = 1;
                for(int j = 0; j < 5; j++){
                    if(board[c.row][c.column+i+j] != id){
                        flag = 0;
                        break;
                    }
                }
                if(flag){
                    for(int j = 0; j < 5; j++){
                        position_score[c.row][c.column+i+j] += score;
                        possible_new_me.insert(position(c.row, c.column+i+j));
                    }
                }
            }
            if(c.row+i >= 0 && c.row+i < 15) {
                //vertical[c.row+i][c.column] += score;
                flag = 1;
                for(int j = 0; j < 5; j++){
                    if(board[c.row+i+j][c.column] != id){
                        flag = 0;
                        break;
                    }
                }
                if(flag){
                    for(int j = 0; j < 5; j++){
                        position_score[c.row+i+j][c.column] += score;
                        possible_new_me.insert(position(c.row+i+j, c.column));
                    }
                }
            }
            if(c.row+i >= 0 && c.row+i < 15 && c.column +i >= 0 && c.column+i < 15){
                //left_up[c.row+i][c.column+i] += score;
                flag = 1;
                for(int j = 0; j < 5; j++){
                    if(board[c.row+i+j][c.column+i+j] != id){
                        flag = 0;
                        break;
                    }
                }
                if(flag){
                    for(int j = 0; j < 5; j++){
                        position_score[c.row+i+j][c.column+i+j] += score;
                        possible_new_me.insert(position(c.row+i+j, c.column+i+j));
                    }
                }

            } 
            if(c.row+i >= 0 && c.row+i < 15 && c.column-i >= 0 && c.column-i < 15) {
                //right_up[c.row+i][c.column-i] += score;
                flag = 1;
                for(int j = 0; j < 5; j++){
                    if(board[c.row+i+j][c.column-i-j] != id){
                        flag = 0;
                        break;
                    }
                }
                if(flag){
                    for(int j = 0; j < 5; j++){
                        position_score[c.row+i+j][c.column-i-j] += score;
                        possible_new_me.insert(position(c.row+i+j, c.column-i-j));
                    }
                }
            }
        }
    }
}

void read_board(std::ifstream& fin) {
    int id;
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
            record_position(board[i][j], i, j);
        }
    }
    
    if(blacky.size() == whity.size()) id = 1;
    else id = 2;
    statefunction(id);
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    int x, y;
    // Keep updating the output until getting killed.
    while(true) {
        // Choose a random spot.
        int x = (rand() % SIZE);
        int y = (rand() % SIZE);
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
    }
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
