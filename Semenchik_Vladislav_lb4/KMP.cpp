#include <iostream>
#include <string>
#include <vector>

#define LOG false
#define SHIFT true

using namespace std;


vector<int> buildPrefixFunction(const string& P) {
    int m = P.length();
    vector<int> pi(m, 0);
    
    if (LOG) cout << "Построение префикс-функции для шаблона: " << P << endl;
    
    int j = 0;
    for (int i = 1; i < m; i++) {
        if (LOG) cout << "  i=" << i << ", j=" << j << ", P[i]=" << P[i] << ", P[j]=" << P[j] << endl;
        
        while (j > 0 && P[i] != P[j]) {
            if (LOG) cout << "    несовпадение, j=" << j << " -> pi[" << j-1 << "]=" << pi[j-1] << endl;
            j = pi[j - 1];
        }
        
        if (P[i] == P[j]) {
            j++;
            if (LOG) cout << "    совпадение, j++ -> j=" << j << endl;
        }
        
        pi[i] = j;
        if (LOG) cout << "    pi[" << i << "]=" << j << endl;
    }
    
    if (LOG) {
        cout << "Готовая префикс-функция: ";
        for (int i = 0; i < m; i++) cout << pi[i] << " ";
        cout << endl;
    }
    
    return pi;
}

vector<int> KMPSearch(const string& P, const string& T) {
    int n = T.length();
    int m = P.length();
    
    if (m == 0) {
        if (LOG) cout << "Образец пустой, возврат пустого списка" << endl;
        return {};
    }
    
    if (LOG) cout << "Поиск образца \"" << P << "\" в тексте \"" << T << "\"" << endl;
    
    vector<int> pi = buildPrefixFunction(P);
    vector<int> occurrences;
    
    int j = 0;
    
    for (int i = 0; i < n; i++) {
        if (LOG) cout << "i=" << i << ", T[i]=" << T[i] << ", j=" << j << ", P[j]=" << (j < m ? string(1, P[j]) : "?") << endl;
        
        while (j > 0 && T[i] != P[j]) {
            if (LOG) cout << "  несовпадение, откат j=" << j << " -> pi[" << j-1 << "]=" << pi[j-1] << endl;
            j = pi[j - 1];
        }
        
        if (T[i] == P[j]) {
            j++;
            if (LOG) cout << "  совпадение, j++ -> j=" << j << endl;
        }
        
        if (j == m) {
            int pos = i - m + 1;
            occurrences.push_back(pos);
            if (LOG) cout << "  найдено вхождение на позиции " << pos << endl;
            j = pi[j - 1];
            if (LOG) cout << "  после нахождения, j=" << j << " (откат по pi[" << m-1 << "])" << endl;
            if (SHIFT) {
                if (LOG) cout << "  SHIFT режим, выход после первого вхождения" << endl;
                return occurrences;
            }
        }
    }
    
    if (LOG) cout << "Поиск завершён, найдено вхождений: " << occurrences.size() << endl;
    
    return occurrences;
}

int main() {
    string P, T;
    
    cin >> P;
    cin >> T;
    
    if (LOG) cout << "Входные данные: образец=\"" << P << "\", текст=\"" << T << "\", SHIFT=" << SHIFT << endl;

    vector<int> occurrences;

    if (SHIFT) {
        if (LOG) cout << "Режим выявления цикличсекого сдвига" << endl;
        if (P.length() != T.length()) {
            if (LOG) cout << "Длины образца и текста не совпадают" << endl;
            cout << -1 << endl;
            return 0;
        }
        if (P.empty()) {
            if (LOG) cout << "Пустой образец" << endl;
            cout << 0 << endl;
            return 0;
        }
        string doubleP = P + P;
        if (LOG) cout << "Склеенный образец: " << doubleP << endl;
        occurrences = KMPSearch(T, doubleP);
    } else {
        if (LOG) cout << "Режим обычного поиска" << endl;
        occurrences = KMPSearch(P, T);
    }
    
    if (occurrences.empty()) {
        if (LOG) cout << "Результат: вхождений нет" << endl;
        cout << -1 << endl;
    } else {
        if (LOG) cout << "Результат: ";
        for (size_t i = 0; i < occurrences.size(); i++) {
            if (i > 0) cout << ",";
            cout << occurrences[i];
        }
        cout << endl;
    }

    return 0;
}