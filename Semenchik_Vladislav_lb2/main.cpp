#define LOG false

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    int n;
    cin >> n;
    
    vector<vector<int>> graph(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> graph[i][j];
        }
    }

    vector<vector<int>> dp(1 << n, vector<int>(n, INT_MAX));
    
    vector<vector<int>> parent(1 << n, vector<int>(n, -1));
    
    dp[1 << 0][0] = 0;
    
    for (int mask = 1; mask < (1 << n); mask++) {
        if (!(mask & 1)) continue;

        if (LOG) {
            cout << "Обработка маски: " << mask << " (";
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) cout << i << " ";
            }
            cout << ")" << endl;
        }
        
        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u))) continue;
            if (dp[mask][u] == INT_MAX) continue;

            if (LOG) cout << "Текущий город u=" << u << ", стоимость пути: " << dp[mask][u] << endl;
            
            for (int v = 0; v < n; v++) {
                if (mask & (1 << v)) continue;
                if (graph[u][v] == 0) continue;
                
                int newMask = mask | (1 << v);
                int newCost = dp[mask][u] + graph[u][v];

                if (LOG) cout << "Рассматриваем переход из u=" << u << " в v=" << v 
                              << ", вес=" << graph[u][v] 
                              << ", новая маска=" << newMask
                              << ", новая стоимость=" << newCost << endl;
                
                if (newCost < dp[newMask][v]) {
                    dp[newMask][v] = newCost;
                    parent[newMask][v] = u;

                    if (LOG) cout << "Обновляем: dp[" << newMask << "][" << v 
                                  << "]. было " << dp[newMask][v] 
                                  << ", стало " << newCost << endl;
                    dp[newMask][v] = newCost;
                    parent[newMask][v] = u;
                } else if (LOG) {
                    cout << "Не обновляем: " << newCost << " >= " << dp[newMask][v] << endl;
                }
            }
        }
    }
    
    int fullMask = (1 << n) - 1;
    int minCost = INT_MAX;
    int lastCity = -1;
    
    for (int i = 1; i < n; i++) {
        if (dp[fullMask][i] == INT_MAX) continue;
        if (graph[i][0] == 0) continue;
        
        int totalCost = dp[fullMask][i] + graph[i][0];
        if (totalCost < minCost) {
            minCost = totalCost;
            lastCity = i;
        }
    }
    
    if (minCost == INT_MAX) {
        cout << "no path" << endl;
        return 0;
    }
    
    vector<int> path;
    path.push_back(0);
    int mask = fullMask;
    int current = lastCity;
    
    while (current != 0) {
        path.push_back(current);
        int prev = parent[mask][current];
        mask &= ~(1 << current);
        current = prev;
    }
    path.push_back(0);
    
    reverse(path.begin(), path.end());
    
    cout << minCost << endl;
    for (int i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) cout << " ";
    }
    cout << endl;
    
    return 0;
}