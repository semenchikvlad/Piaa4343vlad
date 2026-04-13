#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(time(nullptr));
    
    int N;
    cin >> N;
    
    vector<vector<int>> g(N, vector<int>(N, 0));
    
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            int w = rand() % 100 + 1; // 1..100
            g[i][j] = w;
            g[j][i] = w;
        }
    }
    
    ofstream f("input.txt");
    f << N << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            f << g[i][j];
            if (j < N - 1) f << " ";
        }
        f << endl;
    }
    
    return 0;
}