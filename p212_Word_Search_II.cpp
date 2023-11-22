#include<string>
#include<iostream>
#include<stack>
#include<vector>

using namespace std;

struct Pos {
    int i;
    int j;
    char l;
};

class Node {
    public:
        Node() {
        }
        ~Node() {
            pos_history.clear();
        }
        vector<Pos> pos_history;
        Pos get_pos() const {
            return pos_history.back();
        }
        const vector<Pos> get_pos_history() const {
            return pos_history;
        }
        bool has_visited(Pos pos) const {
            for (int i = 0; i < pos_history.size(); i++) {
                if (pos_history[i].i == pos.i && pos_history[i].j == pos.j) {
                    return true;
                }
            }
            return false;
        }
        Node* create_child(Pos pos) const {
            Node* child = new Node();
            child->pos_history = pos_history;
            child->pos_history.push_back(pos);
            return child;
        }
        void set_root(Pos pos) {
            pos_history.push_back(pos);
        }
        
        int word_len() const {
            return pos_history.size();
        }
        string word() const {
            string out;
            for (int i = 0; i < pos_history.size(); i++) {
                out += pos_history[i].l;
            }
            return out;
        }
        string word_reversed()  const {
            string out;
            for (int i = pos_history.size() - 1; i >= 0; i--) {
                out += pos_history[i].l;
            }
            return out;
        }
};

bool is_prefix(const string& word, vector<string>& words) {
    for (int i = 0; i < words.size(); i++) {
        if (words[i].size() < word.size()) {
            continue;
        }
        if (word == words[i].substr(0, word.size())) {
            // where = i;
            return true;
        }
    }
    return false;
}

bool is_pre_or_postfix(Node* current, vector<string>& words) {
    return (is_prefix(current->word(), words) || is_prefix(current->word_reversed(), words));
}

bool is_word(const string& word, vector<string>& words, int& where) {
    for (int i = 0; i < words.size(); i++) {
        if (word == words[i]) {
            where = i;
            return true;
        }
    }
    return false;
}

void create_neighbors(Node* current, stack<Node*>& neighbors, vector<vector<char>>& board, int max_word_length, vector<string>& unfound_words) {
    if (current->word_len() >= max_word_length) { return; }
    Pos neighbor_pos;
    neighbor_pos = current->get_pos();
    Node* neighbor = current->create_child(neighbor_pos);
    if (true) {
    for (int k = 0; k < 4; k++) {
        Pos neighbor_pos;
        neighbor_pos = current->get_pos();
        if (k == 0) { // up (i-)
            if (neighbor_pos.i == 0) continue; 
            neighbor_pos.i -= 1;
        }
        if (k == 1) { // right (j+)
            const int n = board[neighbor_pos.i].size();
            if (neighbor_pos.j == (n - 1)) continue; 
            neighbor_pos.j += 1;
        }
        if (k == 2) { // down (i+)
            const int m = board.size();
            if (neighbor_pos.i == (m - 1)) continue; 
            neighbor_pos.i += 1;
        }
        if (k == 3) { // left (j-)
            if (neighbor_pos.j == 0) continue; 
            neighbor_pos.j -= 1;
        }
        neighbor_pos.l = board[neighbor_pos.i][neighbor_pos.j];
        if (current->has_visited(neighbor_pos)) continue;
        Node* neighbor = current->create_child(neighbor_pos);
        if (!is_pre_or_postfix(neighbor, unfound_words)) continue;
        neighbors.push(neighbor);
    }
    }
}
class Solution {
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        vector<string> words_in_board;
        vector<string> unfound_words;
        for (int i = 0; i < words.size(); i++) {
            unfound_words.push_back(words[i]);
        }
        // expand tree
        vector<Node*> root_nodes;
        // root nodes
        int m = board.size();
        for (int i = 0; i < m; i++) {
            int n = board[i].size();
            for (int j = 0; j < n; j++) {
                Node* root_node = new Node();
                Pos pos;
                pos.i = i;
                pos.j = j;
                pos.l = board[i][j];
                root_node->set_root(pos);
                root_nodes.push_back(root_node);
                cout << pos.l;
            }
            cout << endl;
        }
        // expand recursively
        for (int i = 0; i < root_nodes.size(); i++) {
            stack<Node*> open_nodes;
            open_nodes.push(root_nodes[i]);
            while (true) {
                if (open_nodes.empty()) {
                    break;
                }
                Node* current = open_nodes.top();
                open_nodes.pop();
                // cout << current->word() << endl;
                int where;
                if (is_word(current->word(), unfound_words, where)) {
                    words_in_board.push_back(current->word());
                    unfound_words.erase(unfound_words.begin()+where);
                }
                if (is_word(current->word_reversed(), unfound_words, where)) {
                    words_in_board.push_back(current->word_reversed());
                    unfound_words.erase(unfound_words.begin()+where);
                }
                int max_word_length = 10;
                create_neighbors(current, open_nodes, board, max_word_length, unfound_words);
                delete current;
                if (words.size() == words_in_board.size()) { return words_in_board;}
            }
            
        }
        return words_in_board;
    }
};

