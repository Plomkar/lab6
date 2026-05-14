#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <string>

using namespace std;

// Преобразуем доску в строку для хранения в set
string boardToString(const vector<vector<int>>& board) {
    string s;
    for (const auto& row : board)
        for (int cell : row)
            s += to_string(cell);
    return s;
}

// Переворачиваем две соседние клетки
void flip(vector<vector<int>>& board, int x1, int y1, int x2, int y2) {
    board[x1][y1] ^= 1;
    board[x2][y2] ^= 1;
}

// Проверяем, достигли ли мы целевого состояния (верх = targetTop, низ = targetBottom)
bool isTarget(const vector<vector<int>>& board, int targetTop, int targetBottom) {
    int rows = board.size();
    int cols = board[0].size();
    int half = rows / 2;
    
    for (int i = 0; i < rows; i++) {
        int target = (i < half) ? targetTop : targetBottom;
        for (int j = 0; j < cols; j++) {
            if (board[i][j] != target)
                return false;
        }
    }
    return true;
}

// BFS поиск минимального количества ходов
int bfs(vector<vector<int>> start, int targetTop, int targetBottom) {
    int rows = start.size();
    int cols = start[0].size();
    
    queue<pair<vector<vector<int>>, int>> q;
    set<string> visited;
    
    q.push({start, 0});
    visited.insert(boardToString(start));
    
    while (!q.empty()) {
        auto [board, steps] = q.front();
        q.pop();
        
        if (isTarget(board, targetTop, targetBottom)) {
            return steps;
        }
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                // Горизонтальная пара
                if (j + 1 < cols) {
                    vector<vector<int>> newBoard = board;
                    flip(newBoard, i, j, i, j + 1);
                    string state = boardToString(newBoard);
                    if (!visited.count(state)) {
                        visited.insert(state);
                        q.push({newBoard, steps + 1});
                    }
                }
                // Вертикальная пара
                if (i + 1 < rows) {
                    vector<vector<int>> newBoard = board;
                    flip(newBoard, i, j, i + 1, j);
                    string state = boardToString(newBoard);
                    if (!visited.count(state)) {
                        visited.insert(state);
                        q.push({newBoard, steps + 1});
                    }
                }
            }
        }
    }
    return -1;
}

int main() {
    vector<vector<int>> board = {
        {1, 0, 1},
        {0, 1, 0},
        {1, 1, 0},
        {0, 0, 1}
    };
    
    int rows = board.size();
    int cols = board[0].size();
    int half = rows / 2;
    
    cout << "Исходная доска (" << rows << "x" << cols << "):" << endl;
    for (const auto& row : board) {
        for (int cell : row)
            cout << cell << " ";
        cout << endl;
    }
    cout << endl;
    
    // Целевое состояние: верх = 0, низ = 1
    int result = bfs(board, 0, 1);
    
    cout << "Целевое состояние: верхняя половина = 0, нижняя половина = 1" << endl;
    cout << "Минимальное количество ходов: " << result << endl;
    
    return 0;
}