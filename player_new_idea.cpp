#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#include <set>
#include <climits>
//high winning chance baseline1
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
set<int> row, column, left_up, right_up;

int evaluate(int repeat, int alive){
    if(repeat == 2) return (alive == 0)?0:1;
    else if(repeat == 3) return (alive == 0)?2:3;
    else if (repeat == 4) return (alive == 0)?4:5;
    else if (repeat == 5) return 6;
    else return 8;
}

int calculate(void){
    int max_score = 0;
    int my_key[10] = {0}, opponent_key[10] = {0};
    int i, j;
    //horizontal
    for(auto c: row){
        for(i = 0; i < 15; i++){
            if(board[c][i] == player){
                for(j = 1; j <= 4; j++){
                    if(i+j >= 15) break;
                    if(board[c][i+j] != player) break;
                }
                if(j == 5) my_key[6]++;
                else{
                    if(i-1 < 0){
                        if(board[c][i+j] != (3-player)) my_key[evaluate(j, 0)]++;}
                    else if(i+j >= 15){
                        if(board[c][i-1] != (3-player)) my_key[evaluate(j, 0)]++;}
                    else if(board[c][i-1] == 0 && board[c][i+j] == 0){
                        my_key[evaluate(j, 1)]++;}
                    else if(board[c][i-1] == 0 || board[c][i+j] == 0){
                        my_key[evaluate(j, 0)]++;}
                }
                i = i + j - 1;
            }
            else if(board[c][i] != 0){
                for(j = 1; j <= 4; j++){
                    if(i+j >= 15) break;
                    if(board[c][i+j] != (3-player)) break;
                }
                if(j == 5) opponent_key[6]++;
                else{
                    if(i-1 < 0){
                        if(board[c][i+j] != player) opponent_key[evaluate(j, 0)]++;}
                    else if(i+j >= 15){
                        if(board[c][i-1] != player) opponent_key[evaluate(j, 0)]++;}
                    else if(board[c][i-1] == 0 && board[c][i+j] == 0){
                        opponent_key[evaluate(j, 1)]++;}
                    else if(board[c][i-1] == 0 || board[c][i+j] == 0){
                        opponent_key[evaluate(j, 0)]++;}
                }
                i = i + j - 1;
            }
        }
    }
    //vertical
    for(auto c: column){
        for(i = 0; i < 15; i++){
            if(board[i][c] == player){
                for(j = 1; j <= 4; j++){
                    if(i+j >= 15) break;
                    if(board[i+j][c] != player) break;
                }
                if(j == 5)  my_key[6]++;
                else{
                    if(i-1 < 0){
                        if(board[i+j][c] != (3-player)) my_key[evaluate(j, 0)]++;}
                    else if(i+j >= 15){
                        if(board[i-1][c] != (3-player)) my_key[evaluate(j, 0)]++;}
                    else if(board[i-1][c] == 0 && board[i+j][c] == 0){
                        my_key[evaluate(j, 1)]++;}
                    else if(board[i-1][c] == 0 || board[i+j][c] == 0){
                        my_key[evaluate(j, 0)]++;}
                }
                i = i + j - 1;
            }
            else if(board[i][c] != 0){
                for(j = 1; j <= 4; j++){
                    if(i+j >= 15) break;
                    if(board[i+j][c] != (3-player)) break;
                }
                if(j == 5) opponent_key[6]++;
                else{
                    if(i-1 < 0){
                        if(board[i+j][c] != player) opponent_key[evaluate(j, 0)]++;}
                    else if(i+j >= 15){
                        if(board[i-1][c] != player) opponent_key[evaluate(j, 0)]++;}
                    else if(board[i-1][c] == 0 && board[i+j][c] == 0){
                        opponent_key[evaluate(j, 1)]++;}
                    else if(board[i-1][c] == 0 || board[i+j][c] == 0){
                        opponent_key[evaluate(j, 0)]++;}
                }
                i = i + j - 1;
            }
        }
    }
    //right_up
    for(auto c: right_up){
        if(c < 4) continue;
        if(c > 26) break;
        for(i = 0; i < 15; i++){
            if(c-i < 0) break;
            if(board[i][c-i] == player){
                for(j = 1; j <= 4; j++){
                    if(i+j >= 15 || c-i-j < 0) break;
                    if(board[i+j][c-i-j] != player) break;
                }
                if(j == 5) my_key[6]++;
                else{
                    if(i-1 < 0){
                        if(board[i+j][c-i-j] != (3-player)) my_key[evaluate(j, 0)]++;}
                    else if(c-i+1 >= 15){
                        if(board[i+j][c-i-j] != (3-player)) my_key[evaluate(j, 0)]++;}
                    else if(i+j >= 15){
                        if(board[i-1][c-i+1] != (3-player)) my_key[evaluate(j, 0)]++;}
                    else if(c-i-j < 0){
                        if(board[i-1][c-i+1] != (3-player)) my_key[evaluate(j, 0)]++;}
                    else if(board[i-1][c-i+1] == 0 && board[i+j][c-i-j] == 0){
                        my_key[evaluate(j, 1)]++;}
                    else if(board[i-1][c-i+1] == 0 || board[i+j][c-i-j] == 0){
                        my_key[evaluate(j, 0)]++;}
                }
                i = i + j - 1;
            }
            else if(board[i][c-i] != 0){
                for(j = 1; j <= 4; j++){
                    if(i+j >= 15 || c-i-j < 0) break;
                    if(board[i+j][c-i-j] != (3-player)) break;
                }
                if(j == 5) opponent_key[6]++;
                else{
                    if(i-1 < 0){
                        if(board[i+j][c-i-j] != player) opponent_key[evaluate(j, 0)]++;}
                    else if(c-i+1 >= 15){
                        if(board[i+j][c-i-j] != player) opponent_key[evaluate(j, 0)]++;}
                    else if(i+j >= 15){
                        if(board[i-1][c-i+1] != player) opponent_key[evaluate(j, 0)]++;}
                    else if(c-i-j < 0){
                        if(board[i-1][c-i+1] != player) opponent_key[evaluate(j, 0)]++;}
                    else if(board[i-1][c-i+1] == 0 && board[i+j][c-i-j] == 0){
                        opponent_key[evaluate(j, 1)]++;}
                    else if(board[i-1][c-i+1] == 0 || board[i+j][c-i-j] == 0){
                        opponent_key[evaluate(j, 0)]++;}
                }
                i = i + j - 1;
            }
        }
    }
    //left_up
    for(auto c: left_up){
        if(c < 4) continue;
        if(c > 26) break;
        for(i = 0; i < 15; i++){
            if(14-c+i >= 15 || 14-c+i < 0) break;
            if(board[i][14-c+i] == player){
                for(j = 1; j <= 4; j++){
                    if(i+j>= 15 || 14-c+i+j >= 15) break;
                    if(board[i+j][14-c+i+j] != player) break;
                }
                if(j == 5) my_key[6]++;
                else{
                    if(i-1 < 0){
                        if(board[i+j][14-c+i+j] != (3-player)) my_key[evaluate(j, 0)]++;}
                    else if(14-c+i+j >= 15){
                        if(board[i-1][14-c+i-1] != (3-player)) my_key[evaluate(j, 0)]++;}
                    else if(i+j >= 15){
                        if(board[i-1][14-c+i-1] != (3-player)) my_key[evaluate(j, 0)]++;}
                    else if(14-c+i-1 < 0){
                        if(board[i+j][14-c+i+j] != (3-player)) my_key[evaluate(j, 0)]++;}
                    else if(board[i-1][14-c+i-1] == 0 && board[i+j][14-c+i+j] == 0){
                        my_key[evaluate(j, 1)]++;}
                    else if(board[i-1][14-c+i-1] == 0 || board[i+j][14-c+i+j] == 0){
                        my_key[evaluate(j, 0)]++;}    
                } 
            }
            else if(board[i][14-c+i] != 0){
                for(j = 1; j <= 4; j++){
                    if(i+j >= 15 || 14-c+i+j >= 15) break;
                    if(board[i+j][14-c+i+j] != (3-player)) break;
                }
                if(j == 5) opponent_key[6]++;
                else{
                    if(i-1 < 0){
                        if(board[i+j][14-c+i+j] != player) opponent_key[evaluate(j, 0)]++;
                    }
                    else if(14-c+i+j >= 15){
                        if(board[i-1][14-c+i-1] != player) opponent_key[evaluate(j, 0)]++;}
                    else if(i+j >= 15){
                        if(board[i-1][14-c+i-1] != player) opponent_key[evaluate(j, 0)]++;}
                    else if(14-c+i-1 < 0){
                        if(board[i+j][14-c+i+j] != player) opponent_key[evaluate(j, 0)]++;}
                    else if(board[i-1][14-c+i-1] == 0 && board[i+j][14-c+i+j] == 0){
                        opponent_key[evaluate(j, 1)]++;}  
                    else if(board[i-1][14-c+i-1] == 0 || board[i+j][14-c+i+j] == 0){
                        opponent_key[evaluate(j, 0)]++;}    
                }
            }
        }
    }
    max_score = 10*my_key[0]+ 100*my_key[1] + 1000*my_key[2] + 5000*my_key[3] + 8000*my_key[4] + 50000*my_key[5] + 200000*my_key[6];
    max_score -= (10*opponent_key[0] + 1000*opponent_key[1] + 1000*opponent_key[2]+ 10000*opponent_key[3]+10000*opponent_key[4] + 50000*opponent_key[5] + 100000*opponent_key[6]);  
    //cout << my_key[0] << my_key[1] << my_key[2] << my_key[3] << my_key[4] << "  "  << opponent_key[0] << opponent_key[1] << opponent_key[2] << opponent_key[3] << opponent_key[4] << endl;
    return max_score;
}

int statefunction(int id, int depth){
    set<position, cmp> possible_new_me;
    int final_score;
    if(id != player) final_score = INT_MAX;
    else final_score = INT_MIN;
    if(depth == 3) return(calculate());
    int flag;
    for(auto c: all){
        for(int i = -2; i <= 2; i++){ //有時間再修正
            if(c.column+i >= 0 && c.column+i < 15) {
                if(board[c.row][c.column+i] == 0) 
                    possible_new_me.insert(position(c.row, c.column+i));
            }
            if(c.row+i >= 0 && c.row+i < 15) {
                if(board[c.row+i][c.column] == 0) 
                    possible_new_me.insert(position(c.row+i, c.column));
            }
            if(c.row+i >= 0 && c.row+i < 15 && c.column +i >= 0 && c.column+i < 15){
                if(board[c.row+i][c.column+i] == 0)
                    possible_new_me.insert(position(c.row+i, c.column+i));
            } 
            if(c.row+i >= 0 && c.row+i < 15 && c.column-i >= 0 && c.column-i < 15) {
                if(board[c.row+i][c.column-i] == 0) 
                    possible_new_me.insert(position(c.row+i, c.column-i));
            }
        }
    }
    for(auto c: possible_new_me){
        all.push_back(c);
        //row.insert(c.row);
        //column.insert(c.column);
        //right_up.insert(c.row+c.column);
        //left_up.insert(14-(c.column-c.row));
        board[c.row][c.column] = id;
        int here = statefunction(3-id, depth+1); 
        if(id != player) {
            //cout << "2:: "<< c.row << " " << c.column << " " << here << endl;
            if(here < final_score){
                final_score = here;
                //cout << "2:: "<< c.row << " " << c.column << " " << final_score << endl;
            }
        }
        else {
            //cout << c.row << " " << c.column << " " << here << endl;
            if(here >= final_score){
                final_score = here;
                if(depth == 0){
                    //cout << c.row << " " << c.column << " " << final_score << endl;
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
            if(board[i][j] != 0) {
                row.insert(i);
                column.insert(j);
                right_up.insert(i+j);
                left_up.insert(14-(j-i));
                all.push_back(position(i, j));
            }
        }
    }
    scoring = statefunction(player, 0);
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    int x, y;
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
