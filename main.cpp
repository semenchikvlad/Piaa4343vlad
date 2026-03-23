#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <chrono>
#include <cmath>

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
};

void print_solution(vector<Square> solution, int N) {
    cout << solution.size() << endl;
    for (auto& square : solution) {
        cout << square.x + 1 << " " << square.y + 1 << " " << square.size << "\n";
    }
}

uint64_t Board::bit_prefixes[30];

int smallest_divisor(int N) {
    for (int div = 3; div * div <= N; div++) {
        if (N % div == 0) return div;
    }
    return N;
}

void find_solution(int N, vector<Square> &solution) {
    stack<Board> st;
    Board board(N);
    
    solution.clear();
    solution.push_back({0, 0, N - 1});

    for (int i = 0; i < N; i++) solution.push_back({N - 1, i, 1});
    for (int i = 0; i < N - 1; i++) solution.push_back({i, N - 1, 1});

    int best_side = N / 2 + 1;
    board.place_square(0, 0, best_side);
    board.place_square(0, best_side, N - best_side);
    board.place_square(best_side, 0, N - best_side);
    st.push(board);
    
    while(!st.empty()) {
        Board current = st.top();
        st.pop();
        
        int x, y;
        if (!current.first_empty(x, y)) {
            if (current.placed_squares.size() < solution.size()) 
                solution = current.placed_squares;
            continue;
        }
        
        if (current.placed_squares.size() >= solution.size())  continue;
        
        int max_size = min({N - x, N - y});
        if (current.placed_squares.size() + current.empty_cells_count() / (max_size * max_size) >= solution.size()) continue;
        
        
        while (max_size > 0 && !current.can_place(x, y, max_size)) max_size--;
        
        for(int size = max_size; size > 0; size--) {
            if (current.can_place(x, y, size)) {
                current.place_square(x, y, size);
                st.push(current);
                current.placed_squares.pop_back();
                current.remove_square(x, y, size);
            }   
        }
    }
}
int main() {
    int N;
    cin >> N;
    auto start = std::chrono::high_resolution_clock::now();

    vector<Square> solution;
    if (N % 2 == 0) {
        int half = N / 2;
        solution.clear();
        solution.push_back({0, 0, half});
        solution.push_back({0, half, half});
        solution.push_back({half, 0, half});
        solution.push_back({half, half, half});
    } else {
        int d = smallest_divisor(N);

        find_solution(d, solution);

        if (d != N) {
            int k = N / d;
            
            for (auto& square : solution) {
                square.x *= k;
                square.y *= k;
                square.size *= k;
            }
        }
    }
    
    print_solution(solution, N);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //std::cout << ms << " ms\n";
    return 0;
}