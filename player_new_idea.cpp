#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#include <set>
#include <climits>

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

struct cmp{
    bool operator() (const position &lhs, const position &rhs) const{
        if(lhs.row != rhs.row) return lhs.row < rhs.row;
        else return lhs.column < rhs.column;
    }
};

vector<position> all;
int position_score[15][15];
position max_position(7, 7);

int give_score(int id, int count){
    if(id == player){
        if(count == 1) return 10;
        else if(count == 2) return 20;
        else if(count == 3) return 50;
        else if(count == 4) return 1000;
    }
    else{
        if(count == 1) return 10;
        else if(count == 2) return 25;
        else if(count == 3) return 500;
        else if(count == 4) return 5000;
    }
}

int calculate(void){
    int flag, max_score = INT_MIN, count;
    for(auto c: all){
        for(int i = -4; i <= 0; i++){
            if(c.column+i >= 0 && c.column+i < 11) {
                flag = 1;
                count = 0;
                for(int j = 0; j < 5; j++){
                    if(board[c.row][c.column+i+j] + board[c.row][c.column] == 3){
                        flag = 0;
                        break;
                    }
                    else if(board[c.row][c.column+i+j] != 0) count++;
                }
                if(flag){
                    for(int j = 0; j < 5; j++){
                        position_score[c.row][c.column+i+j] += give_score(board[c.row][c.column], count);
                        if(position_score[c.row][c.column+i+j] > max_score && board[c.row][c.column+i+j] == 0) {
                            max_score = position_score[c.row][c.column+i+j];
                        }
                    }
                }
            }
            if(c.row+i >= 0 && c.row+i < 11) {
                flag = 1; 
                count = 0;
                for(int j = 0; j < 5; j++){
                    if(board[c.row+i+j][c.column] + board[c.row][c.column] == 3){
                        flag = 0;
                        break;
                    }
                    else if(board[c.row+i+j][c.column] != 0) count++;
                }
                if(flag){
                    for(int j = 0; j < 5; j++){
                        position_score[c.row+i+j][c.column] += give_score(board[c.row][c.column], count);
                        if(position_score[c.row+i+j][c.column] > max_score && board[c.row+i+j][c.column] == 0) {
                            max_score = position_score[c.row+i+j][c.column];
                        }
                    }
                }
            }
            if(c.row+i >= 0 && c.row+i < 11 && c.column +i >= 0 && c.column+i < 11){
                flag = 1;
                count = 0;
                for(int j = 0; j < 5; j++){
                    if(board[c.row+i+j][c.column+i+j] + board[c.row][c.column] == 3){
                        flag = 0;
                        break;
                    }
                    else if(board[c.row+i+j][c.column+i+j] != 0) count++;
                }
                if(flag){
                    for(int j = 0; j < 5; j++){
                        position_score[c.row+i+j][c.column+i+j] += give_score(board[c.row][c.column], count);
                        if(position_score[c.row+i+j][c.column+i+j] > max_score && board[c.row+i+j][c.column+i+j] == 0) {
                            max_score = position_score[c.row+i+j][c.column+i+j];
                        }
                    }
                }

            } 
            if(c.row+i >= 0 && c.row+i < 11 && c.column-i >= 4 && c.column-i < 15) {
                flag = 1;
                count = 0;
                for(int j = 0; j < 5; j++){
                    if(board[c.row+i+j][c.column-i-j] + board[c.row][c.column] == 3){
                        flag = 0;
                        break;
                    }
                    else if(board[c.row+i+j][c.column-i-j] != 0) count++;
                }
                if(flag){
                    for(int j = 0; j < 5; j++){
                        position_score[c.row+i+j][c.column-i-j] += give_score(board[c.row][c.column], count);
                        if(position_score[c.row+i+j][c.column-i-j] > max_score && board[c.row+i+j][c.column-i-j] == 0) {
                            max_score = position_score[c.row+i+j][c.column-i-j];
                        }
                    }
                }
            }
        }
    }
    return max_score;
}

int statefunction(int id, int depth){
    set<position, cmp> possible_new_me;
    int final_score;
    if(id == player && depth != 0) final_score = INT_MAX;
    else final_score = INT_MIN;
    if(depth == 3) return(calculate());
    int flag;
    for(auto c: all){
        for(int i = -4; i <= 0; i++){
            if(c.column+i >= 0 && c.column+i < 11) {
                flag = 1;
                for(int j = 0; j < 5; j++){
                    if(board[c.row][c.column+i+j]+board[c.row][c.column] == 3){
                        flag = 0;
                        break;
                    }
                }
                if(flag){
                    for(int j = 0; j < 5; j++){
                        if(board[c.row][c.column+i+j] == 0) 
                            possible_new_me.insert(position(c.row, c.column+i+j));
                    }
                }
            }
            if(c.row+i >= 0 && c.row+i < 11) {
                flag = 1;
                for(int j = 0; j < 5; j++){
                    if(board[c.row+i+j][c.column] + board[c.row][c.column] == 3){
                        flag = 0;
                        break;
                    }
                }
                if(flag){
                    for(int j = 0; j < 5; j++){
                        if(board[c.row+i+j][c.column] == 0) 
                            possible_new_me.insert(position(c.row+i+j, c.column));
                    }
                }
            }
            if(c.row+i >= 0 && c.row+i < 11 && c.column +i >= 0 && c.column+i < 11){
                flag = 1;
                for(int j = 1; j < 5; j++){
                    if(board[c.row+i+j][c.column+i+j] + board[c.row][c.column] == 3){
                        flag = 0;
                        break;
                    }
                }
                if(flag){
                    for(int j = 0; j < 5; j++){
                        if(board[c.row+i+j][c.column+i+j] == 0)
                            possible_new_me.insert(position(c.row+i+j, c.column+i+j));
                    }
                }

            } 
            if(c.row+i >= 0 && c.row+i < 11 && c.column-i >= 4 && c.column-i < 15) {
                flag = 1;
                for(int j = 0; j < 5; j++){
                    if(board[c.row+i+j][c.column-i-j] + board[c.row][c.column] == 3){
                        flag = 0;
                        break;
                    }
                }
                if(flag){
                    for(int j = 0; j < 5; j++){
                        if(board[c.row+i+j][c.column-i-j] == 0) 
                            possible_new_me.insert(position(c.row+i+j, c.column-i-j));
                    }
                }
            }
        }
    }
    for(auto c: possible_new_me){
        all.push_back(c);
        board[c.row][c.column] = id;
        if(id == player && depth != 0) final_score = min(statefunction(3-id, depth+1), final_score);
        else {
            int here = statefunction(3-id, depth+1); 
            if(here > final_score){
                final_score = here;
                if(depth == 0){
                    max_position.row = c.row;
                    max_position.column = c.column;
                } 
            }
        }
        all.pop_back();   
        board[c.row][c.column] = 0;
    }
    return final_score;
}

void read_board(std::ifstream& fin) {
    int scoring;
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
            if(board[i][j] != 0) all.push_back(position(i, j));
        }
    }
    scoring = statefunction(player, 0);
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    int x, y;
    // Keep updating the output until getting killed.
    x = max_position.row;
    y = max_position.column;

    if (board[x][y] == EMPTY) {
        fout << x << " " << y << std::endl;
        // Remember to flush the output to ensure the last action is written to file.
        fout.flush();
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
