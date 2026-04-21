#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <chrono>
#include <cmath>

#define LOG false

using namespace std;

struct Square {
    int x, y, size; 
};

class Board {
public:
    int N;
    vector<uint32_t> rows;
    vector<Square> placed_squares;
    static uint64_t bit_prefixes[30];

    static void initialize_prefixes(int max_size) {
        bit_prefixes[0] = 0;
        for (int i = 1; i <= max_size + 1; i++) {
            bit_prefixes[i] = (bit_prefixes[i-1] | (1ULL << (i-1)));
        }
    }

    Board(int size) : N(size), rows(size, 0) {
        static bool initialized = false;
        if (!initialized) {
            initialize_prefixes(N);
            initialized = true;
        }
    }

    bool first_empty(int &x, int &y) {
        for (int i = 0; i < N; i++) {
            if (rows[i] != (uint64_t)((1ULL << N) - 1)) {
                for (int j = 0; j < N; j++) {
                    if (!(rows[i] & (1ULL << j))) {
                        x = i;
                        y = j;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool can_place(int x, int y, int size) {
        if (x + size > N || y + size > N) return false;
        
        uint64_t mask = bit_prefixes[y + size] ^ bit_prefixes[y];
        
        for (int i = x; i < x + size; i++) {
            if (rows[i] & mask) return false;
        }
        return true;
    }

    void place_square(int x, int y, int size) {
        uint64_t mask = bit_prefixes[y + size] ^ bit_prefixes[y];
        
        for (int i = x; i < x + size; i++) {
            rows[i] |= mask;
        }

        placed_squares.push_back({x, y, size});
    }

    void remove_square(int x, int y, int size) {
        uint64_t mask = bit_prefixes[y + size] ^ bit_prefixes[y];
        
        for (int i = x; i < x + size; i++) {
            rows[i] &= ~mask;
        }
    }

    int empty_cells_count() {
        int count = 0;
        uint64_t full_row_mask = (1ULL << N) - 1;
        
        for (int i = 0; i < N; i++) {
            if (rows[i] == full_row_mask) {
                continue;
            }
            count += N - __builtin_popcountll(rows[i]);
        }
        return count;
    }
    
    void print_board_state() {
        if (!LOG) return;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (rows[i] & (1ULL << j)) {
                    cout << "# ";
                } else {
                    cout << ". ";
                }
            }
            cout << "\n";
        }
    }
};

void print_solution(vector<Square> solution, int N) {
    cout << solution.size() << endl;
    for (auto& square : solution) {
        cout << square.x + 1 << " " << square.y + 1 << " " << square.size << "\n";
    }
}

uint64_t Board::bit_prefixes[30];

int smallest_divisor(int N) {
    if (LOG) cout << "Поиск наименьшего делителя для " << N << endl;
    for (int div = 3; div * div <= N; div++) {
        if (N % div == 0) {
            if (LOG) cout << "Найден делитель: " << div << endl;
            return div;
        }
    }
    if (LOG) cout << "Делителей не найдено" << endl;
    return N;
}

void find_solution(int N, vector<Square> &solution) {
    if (LOG) cout << "\nПоиск решения для квадрата " << N << "x" << N << endl;
    
    stack<Board> st;
    Board board(N);
    
    solution.clear();
    solution.push_back({0, 0, N - 1});
    for (int i = 0; i < N; i++) solution.push_back({N - 1, i, 1});
    for (int i = 0; i < N - 1; i++) solution.push_back({i, N - 1, 1});
    
    if (LOG) cout << "Начальное эвристическое решение: " << solution.size() << " квадратов" << endl;
    
    int best_side = N / 2 + 1;
    if (LOG) cout << "Начальное размещение: квадрат " << best_side << " в углу" << endl;
    
    board.place_square(0, 0, best_side);
    board.place_square(0, best_side, N - best_side);
    board.place_square(best_side, 0, N - best_side);
    st.push(board);
    
    if (LOG) {
        cout << "Начальное состояние доски:\n";
        board.print_board_state();
    }

    int steps = 0;
    
    while(!st.empty()) {
        steps += 1;
        Board current = st.top();
        st.pop();
        
        int x, y;
        if (!current.first_empty(x, y)) {
            if (LOG) cout << "Найдено полное решение из " << current.placed_squares.size() << " квадратов" << endl;
            if (current.placed_squares.size() < solution.size()) {
                if (LOG) cout << "Улучшение: " << solution.size() << " -> " << current.placed_squares.size() << endl;
                solution = current.placed_squares;
                if (LOG) current.print_board_state();
            }
            continue;
        }
        
        if (current.placed_squares.size() >= solution.size()) continue;
        
        int max_size = min({N - x, N - y});
        int empty_cells = current.empty_cells_count();
        int min_additional = empty_cells / (max_size * max_size);
        
        if (current.placed_squares.size() + min_additional >= solution.size()) continue;
        
        while (max_size > 0 && !current.can_place(x, y, max_size)) max_size--;
        
        if (LOG) {
            cout << "Позиция (" << x << "," << y << "), макс. размер=" << max_size 
                 << ", уже квадратов=" << current.placed_squares.size() << endl;
        }
        
        for(int size = max_size; size > 0; size--) {
            if (current.can_place(x, y, size)) {
                if (LOG) {
                    cout << "  Размещаем квадрат " << size << " в (" << x << "," << y << ")" << endl;
                }
                current.place_square(x, y, size);
                st.push(current);
                current.placed_squares.pop_back();
                current.remove_square(x, y, size);
            }   
        }
    }
    cout << steps << "steps\n";
    
    if (LOG) cout << "Поиск завершен." << endl;
}

int main() {
    int N;
    cin >> N;
    
    vector<Square> solution;
    
    if (LOG) cout << "Размер квадрата: " << N << endl;
    
    if (N % 2 == 0) {
        if (LOG) cout << "N четное, делим на 4 квадрата" << endl;
        int half = N / 2;
        solution.clear();
        solution.push_back({0, 0, half});
        solution.push_back({0, half, half});
        solution.push_back({half, 0, half});
        solution.push_back({half, half, half});
    } else {
        if (LOG) cout << "N нечетное, ищем разбиение" << endl;
        int d = smallest_divisor(N);
        
        if (LOG) cout << "Наименьший делитель: " << d << endl;
        
        if (d != N) {
            if (LOG) cout << "Составное N, найдем решение для " << d << "x" << d << endl;
            vector<Square> base_solution;
            find_solution(d, base_solution);
            
            if (LOG) cout << "Масштабируем решение в " << N/d << " раз" << endl;
            int k = N / d;
            solution = base_solution;
            for (auto& square : solution) {
                square.x *= k;
                square.y *= k;
                square.size *= k;
            }
        } else {
            if (LOG) cout << "Простое N, прямой поиск" << endl;
            find_solution(N, solution);
        }
    }
    
    print_solution(solution, N);
    
    return 0;
}