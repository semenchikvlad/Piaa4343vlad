#define LOG false
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <numeric>

using namespace std;

const int INF = INT_MAX / 2;

int primMST(const vector<vector<int>>& graph, const vector<bool>& visited) {
    int n = graph.size();
    vector<bool> inMST(n, false);
    vector<int> minEdge(n, INF);
    
    int totalVertices = 0;
    int start = -1;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            totalVertices++;
            if (start == -1) {
                start = i;
            }
        }
    }

    if (totalVertices <= 1) return 0;
    
    if (start == -1) return 0;
    
    minEdge[start] = 0;
    int mstWeight = 0;
    
    for (int count = 0; count < totalVertices; count++) {
        int u = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && !inMST[i] && (u == -1 || minEdge[i] < minEdge[u])) {
                u = i;
            }
        }
        
        if (u == -1) break;
        
        inMST[u] = true;
        mstWeight += minEdge[u];
        
        for (int v = 0; v < n; v++) {
            if (!visited[v] && !inMST[v] && graph[u][v] != 0 && graph[u][v] < minEdge[v]) {
                minEdge[v] = graph[u][v];
            }
        }
    }
    
    if (LOG) {
        cout << "МОД для множества {";
        for (int i = 0; i < n; i++) {
            if (!visited[i]) cout << i << " ";
        }
        cout << "} вес = " << mstWeight << endl;
    }
    
    return mstWeight;
}

pair<int, vector<int>> ALSH2(const vector<vector<int>>& graph) {
    int n = graph.size();

    vector<bool> visited(n, false);
    vector<int> path;
    
    int current = 0;
    path.push_back(current);
    visited[current] = true;
    
    int totalCost = 0;
    
    for (int step = 1; step < n; step++) {
        int bestNext = -1;
        int bestValue = INF;
        
        if (LOG) {
            cout << "Шаг " << step << ": текущий город = " << current << endl;
            cout << "Посещённые города: ";
            for (int i = 0; i < n; i++) {
                if (visited[i]) cout << i << " ";
            }
            cout << endl;
        }
        
        for (int v = 0; v < n; v++) {
            if (visited[v]) continue;
            if (graph[current][v] == 0) continue;
            
            if (LOG) {
                cout << " Рассматриваем кандидата v=" << v 
                     << ", вес ребра " << current << "->" << v << " = " << graph[current][v] << endl;
            }
            
            vector<bool> tempVisited = visited;
            tempVisited[v] = true;

            int L = primMST(graph, tempVisited);
            
            int value = graph[current][v] + L;
            
            if (LOG) {
                cout << "L = " << L 
                     << ", value = " << value << endl;
            }
            
            if (value < bestValue) {
                bestValue = value;
                bestNext = v;
                if (LOG) {
                    cout << "Новый лучший кандидат: v=" << v 
                         << ", value=" << value << endl;
                }
            }
        }
        
        if (bestNext == -1) {
            return {INF, {}};
        }
        
        if (LOG) {
            cout << "Выбран город: " << bestNext 
                 << " (значение = " << bestValue << ")" << endl;
        }
        
        totalCost += graph[current][bestNext];
        current = bestNext;
        path.push_back(current);
        visited[current] = true;
    }
    
    if (LOG) {
        cout << "Завершение маршрута" << endl;
        cout << "Проверка возврата в город 0, вес ребра " << current << "->0 = " 
             << graph[current][0] << endl;
    }
    
    if (graph[current][0] == 0) {
        if (LOG) cout << "Ошибка: нет ребра обратно в город 0!" << endl;
        return {INF, {}};
    }
    
    totalCost += graph[current][0];
    path.push_back(0);
    
    if (LOG) {
        cout << "Итоговая стоимость: " << totalCost << endl;
        cout << "Полный путь: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
    
    return {totalCost, path};
}

int main() {
    int n;
    cin >> n;
    
    vector<vector<int>> graph(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> graph[i][j];
        }
    }
    
    if (LOG) {
        cout << "Размер графа: " << n << " x " << n << endl;
        cout << "Матрица смежности:" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << graph[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    
    auto result = ALSH2(graph);
    
    if (result.first >= INF) {
        cout << "no path" << endl;
    } else {
        cout << result.first << endl;
        for (size_t i = 0; i < result.second.size(); i++) {
            cout << result.second[i];
            if (i < result.second.size() - 1) cout << " ";
        }
        cout << endl;
    }
    
    return 0;
}