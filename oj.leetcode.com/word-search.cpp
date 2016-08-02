#include <iostream>
#include <vector>
#include <stdbool.h>
#include <string>
#include <algorithm>

using namespace std;

struct Coordinate {
    int h;
    int v;
    friend bool operator == (const Coordinate& lhs, const Coordinate& rhs) {
        if(lhs.h == rhs.h && lhs.v == rhs.v) {
            return 1;
        } else {
            return 0;
        }
    }
    friend bool operator != (const Coordinate& lhs, const Coordinate& rhs) {
        if(lhs.h == rhs.h || lhs.v == rhs.v) {
            return 0;
        } else {
            return 1;
        }
    }
};
typedef vector<Coordinate> CoordinateContainer;
typedef CoordinateContainer::iterator CoordinateIterator;

std::ostream& operator<<(std::ostream& stream, const Coordinate& coord) {
    stream << coord.h << "," << coord.v;
    return stream;
}

struct tWord {
    char w;
    Coordinate pos;
    friend bool operator == (const tWord& lhs, const tWord& rhs) {
        if(lhs.w == rhs.w && lhs.pos == rhs.pos) {
            return true;
        } else {
            return false;
        }
    }
};
typedef vector<tWord> tWordContainer;
typedef tWordContainer::iterator tWCIterator;

class Solution {
    public:
    bool exist(vector<vector<char> > &board, string word) {
        vector< vector<char> >::iterator board_itr = board.begin();
        uint h_size, v_size, w_size;
        h_size = board.size();
        v_size = board[0].size();
        w_size = word.size();
        if(w_size > h_size*v_size) {
            return false;
        }
        tWord tword_item;
        tWordContainer tword(w_size);
        string::iterator word_itr = word.begin();
        uint i=0;
        while (i < w_size) {
            tword_item.w = *(word_itr+i);
            tword_item.pos.h = 0;
            tword_item.pos.v = 0;
            tword[i] = tword_item;
            i++;
        }
        Coordinate pos;
        pos.h = 0;
        while (board_itr!=board.end()) {
            vector<char>::iterator board_word_itr =(*board_itr).begin();
            pos.v=0;
            while (board_word_itr!=(*board_itr).end()) {
                if (retrieve(board, h_size, v_size, pos, tword, 0))
                {
                    return true;
                }
                board_word_itr++;
                pos.v++;
            }
            board_itr++;
            pos.h++;
        }
        return false;
    }
    private:
    bool retrieve(vector< vector<char> > &board, uint h_size, uint v_size, Coordinate pos, tWordContainer word, uint word_pos) {
        tWCIterator word_itr = word.begin();
        word_itr = word_itr + word_pos;
        int count = 0;
        for(tWCIterator word_itr_tmp = word.begin(); word_itr_tmp != word.end(); word_itr_tmp++) {
            cout << word_itr_tmp->pos << "|" << word_itr_tmp->w << "|" << pos << "|" << word_itr->w << "|" << count << endl;
            if(word_itr_tmp->pos == pos && word_itr_tmp->w == word_itr->w) {
                count++;
                if(count>1) {
                    cout << "----" <<word_itr_tmp->pos << "|" << word_itr_tmp->w << "|" << pos << "|" << word_itr->w << "|" << count << endl;
                    return false;
                }
            }
        }
        word_itr = word.begin();
        word_itr = word_itr + word_pos;
        if (pos.h+1 > h_size || pos.v+1 > v_size) {
           return false;
        }
        if (word.end() == find(word.begin(), word.end(), *word_itr)) {
            return false;
        }
        //cout << pos << ":" << board[pos.h][pos.v] << ":" << word_itr->w << endl;
        if (board[pos.h][pos.v] && board[pos.h][pos.v] == word_itr->w) {
            Coordinate up_pos, right_pos, down_pos, left_pos;
            if(word_itr == word.end()) {
                return true;
            }
            word_itr->pos = pos;
            word[word_pos] = *word_itr;
            word_pos++;
            // 右
            right_pos = pos;
            right_pos.v++;
            //cout << "right:" << right_pos << endl;
            if (right_pos.v<0 || right_pos.h<0) {
                return false;
            }
            if(retrieve(board, h_size, v_size, right_pos, word, word_pos)) {
                return true;
            }
            // 下
            down_pos = pos;
            down_pos.h++;
            //cout << "down:" << down_pos << endl;
            if (down_pos.v<0 || down_pos.h<0) {
                return false;
            }
            if (retrieve(board, h_size, v_size, down_pos, word, word_pos)) {
                return true;
            }
            // 左
            left_pos = pos;
            left_pos.v--;
            //cout << "left:" << left_pos << endl;
            if (left_pos.v<0 || left_pos.h<0) {
                return false;
            }
            if (retrieve(board, h_size, v_size, left_pos, word, word_pos)) {
                return true;
            }
            // 上
            up_pos = pos;
            up_pos.h--;
            //cout << "up:" << up_pos << endl;
            if (up_pos.v<0 || up_pos.h<0) {
                return false;
            }
            if (retrieve(board, h_size, v_size, up_pos, word, word_pos)) {
                return true;
            }
        }
        return false;
    }
};

int main() {
    Solution s;
    string word;
    uint M = 4;
    uint N = 4;
    uint i, j;
    vector< vector<char> > board(N, vector<char>(M));

    string board_string = "ABCDEFGHJKLMNOPQ";
    vector<char>::iterator board_string_itr = board_string.begin();
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            board[i][j] = *board_string_itr;
            board_string_itr++;
        }
    }

    word = "ABCD";
    cout << s.exist(board, word) << endl;

    word = "ABCDHGFEJKLMQPON";
    cout << s.exist(board, word) << endl;

    word = "ABCE";
    cout << s.exist(board, word) << endl;

    word = "ABCGHDC";
    cout << s.exist(board, word) << endl;
}
