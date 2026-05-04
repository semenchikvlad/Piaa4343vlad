#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using namespace std;

#define LOG false
#define INPUT true
#define OPERATIONS true
#define EXTENSION true


enum Operation { MATCH, REPLACE, DELETE, INSERT, DOUBLE_INSERT };

int main() {
    int replace_cost, insert_cost, delete_cost, double_insert_cost;
    replace_cost = insert_cost = delete_cost = double_insert_cost = 1;
    if (INPUT) {
        if (EXTENSION) cin >> replace_cost >> insert_cost >> delete_cost >> double_insert_cost;
        else cin >> replace_cost >> insert_cost >> delete_cost;
    }
    
    
    string A, B;
    cin >> A >> B;
    
    int n = A.length();
    int m = B.length();
    
    if (LOG) {
        cout << "строка a: " << A << "\n";
        cout << "строка b: " << B << "\n";
        cout << "стоимости: зам=" << replace_cost << " вст=" << insert_cost 
             << " уд=" << delete_cost << " дв_вст=" << double_insert_cost << "\n\n";
    }
    
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, INT_MAX));
    vector<vector<Operation>> op(n + 1, vector<Operation>(m + 1));
    
    dp[0][0] = 0;
    op[0][0] = MATCH;
    
    for (int j = 1; j <= m; j++) {
        dp[0][j] = dp[0][j-1] + insert_cost;
        op[0][j] = INSERT;
    }
    
    for (int i = 1; i <= n; i++) {
        dp[i][0] = dp[i-1][0] + delete_cost;
        op[i][0] = DELETE;
    }
    
    if (EXTENSION) {
        dp[0][2] = double_insert_cost;
        op[0][2] = DOUBLE_INSERT;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (LOG) {
                cout << "ячейка (" << i << "," << j << ") a[" << i-1 << "]=" << A[i-1] 
                     << " b[" << j-1 << "]=" << B[j-1] << "\n";
            }
            
            if (A[i-1] == B[j-1]) {
                if (dp[i][j] > dp[i-1][j-1]) {
                    dp[i][j] = dp[i-1][j-1];
                    op[i][j] = MATCH;
                    if (LOG) cout << "  совпадение: " << dp[i-1][j-1] << " -> " << dp[i][j] << "\n";
                }
            } else {
                if (dp[i][j] > dp[i-1][j-1] + replace_cost) {
                    dp[i][j] = dp[i-1][j-1] + replace_cost;
                    op[i][j] = REPLACE;
                    if (LOG) cout << "  замена: " << dp[i-1][j-1] << "+" << replace_cost 
                                   << "=" << dp[i][j] << "\n";
                }
            }
            
            if (dp[i][j] > dp[i-1][j] + delete_cost) {
                dp[i][j] = dp[i-1][j] + delete_cost;
                op[i][j] = DELETE;
                if (LOG) cout << "  удаление: " << dp[i-1][j] << "+" << delete_cost 
                               << "=" << dp[i][j] << "\n";
            }
            
            if (dp[i][j] > dp[i][j-1] + insert_cost) {
                dp[i][j] = dp[i][j-1] + insert_cost;
                op[i][j] = INSERT;
                if (LOG) cout << "  вставка: " << dp[i][j-1] << "+" << insert_cost 
                               << "=" << dp[i][j] << "\n";
            }
            
            if (j >= 2 && EXTENSION) {
                char first_char = B[j-2];
                bool can_double_insert = false;
                
                if (j >= 3) {
                    if (first_char == B[j-3]) {
                        can_double_insert = true;
                    }
                }
                
                if (can_double_insert) {
                    if (i >= 0 && dp[i][j] > dp[i][j-2] + double_insert_cost) {
                        dp[i][j] = dp[i][j-2] + double_insert_cost;
                        op[i][j] = DOUBLE_INSERT;
                        if (LOG) cout << "  двойная вставка: " << dp[i][j-2] << "+" << double_insert_cost 
                                       << "=" << dp[i][j] << "\n";
                    }
                }
            }
            
            if (LOG) {
                cout << "  dp[" << i << "][" << j << "] = " << dp[i][j] << "\n";
            }
        }
        if (LOG) cout << "\n";
    }
    
    if (LOG) {
        cout << "итоговая таблица:\n";
        cout << "    ";
        for (int j = 0; j <= m; j++) {
            if (j == 0) cout << "  ε  ";
            else cout << "  " << B[j-1] << "  ";
        }
        cout << "\n";
        
        for (int i = 0; i <= n; i++) {
            if (i == 0) cout << "ε  ";
            else cout << A[i-1] << "  ";
            
            for (int j = 0; j <= m; j++) {
                if (dp[i][j] == INT_MAX) cout << "  ∞  ";
                else printf("%3d  ", dp[i][j]);
            }
            cout << "\n";
        }
        cout << "\n";
    }
    
    if (OPERATIONS) {
        vector<string> operations;
        int i = n, j = m;
        
        while (i > 0 || j > 0) {
            switch (op[i][j]) {
                case MATCH:
                    operations.push_back("M");
                    i--; j--;
                    break;
                case REPLACE:
                    operations.push_back("R");
                    i--; j--;
                    break;
                case DELETE:
                    operations.push_back("D");
                    i--;
                    break;
                case INSERT:
                    operations.push_back("I");
                    j--;
                    break;
                case DOUBLE_INSERT:
                    operations.push_back("P");
                    j -= 2;
                    break;
            }
        }
        
        
        reverse(operations.begin(), operations.end());
        
        for (const string& op_str : operations) {
            cout << op_str;
        }
        cout << endl;
    }
    
    
    cout << dp[n][m] << endl;
    
    return 0;
}