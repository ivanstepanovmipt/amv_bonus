/* 4-приближённый алгоритм Блюма-Янга-Ли-Тромпа-Яннакакиса
** ввод: S1, S2, …, Sn — множество строк конечного алфавита E*
** вывод: X — строка алфавита E* содержащая каждую строку S1..n
в качестве подстроки, где длина |X| минимизирована
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/*
Функция вычисляет максимальную длину суффикса строки s1
совпадающего с префиксом строки s2.
*/

int Overlap(string s1, string s2){
    int l1 = s1.length();
    int l2 = s2.length();
    int x = min(l1, l2);
    s1 = s1.substr(l1 - x);
    s2 = s2.substr(0, x);
    l2 = s2.length();
    while((s1 != s2) && (x != 0)){
        s1 = s1.substr(1);
        s2 = s2.substr(0,l2 - 1);
        l2 = l2 - 1;
        x = x - 1;
    }
    return x;
}

/*
Функция вычисляет матрицу overlap'ов для всех
упорядоченных пар (Si, Sj).
*/

vector<vector<int>> OverlapMatrix(vector<string> substrings, int n){
    vector<vector<int>> matrix(n, vector<int> (n));
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(i == j){
                matrix[i][j] = 0;
            } else {
                matrix[i][j] = Overlap(substrings[i], substrings[j]);
            }
        }
    }
    return matrix;
}

/*
Функция, вычисляющая полное назначение максимального
веса жажным методом.
*/

vector<int> GreedyAppointment(vector<vector<int>> matrix, int n){
    vector<int> matrix_greedy_appointment(n);
    int max_elemen = matrix[0][0];
    int max_line_number = 0;
    int max_column_number = 0;
    vector<vector<int>> matrix_help(n, vector<int> (n)); // вспомогательная матрица для обозначения доступных вершин.
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            matrix_help[i][j] = 1;
        }
    }
    for(int k = 0; k < n; ++k){
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < n; ++j){
                if((matrix[i][j] > max_elemen) && (matrix_help[i][j] == 1)){
                    max_elemen = matrix[i][j];
                    max_column_number = j;
                    max_line_number = i;
                }
            }
        }
        matrix_greedy_appointment[max_line_number] = max_column_number;
        for(int i = 0; i < n; ++i){
            matrix_help[max_line_number][i] = 0;
        }
        for(int i = 0; i < n; ++i){
            matrix_help[i][max_column_number] = 0;
        }
        max_elemen = -1;
        max_column_number = 0;
        max_line_number = 0;
    }
    return matrix_greedy_appointment;
}

/*
Функция нахождение покрытия циклами минимальной полной длины.
*/

vector<vector<int>> CycleCoverage(vector<int> matrix_greedy_appointment, int n){
    vector<vector<int>> matrix_cycle_coverage;
    vector<int> v;      //вспомагательные векторы
    vector<int> v_help;
    for(int i = 0; i < n; ++i){
        if(i == 0)
            v_help.push_back(1);
        else
            v_help.push_back(0);
    }
    int start_of_cycle = 0;
    int current_numbers = matrix_greedy_appointment[0];
    int previous_number = 0;
    int x = 0; // вспомогательная переменная
    for(int i = 0; i < n; ++i){
        if(start_of_cycle == current_numbers){
            v_help[current_numbers] = 1;
            v.push_back(current_numbers);
            matrix_cycle_coverage.push_back(v);
            v.clear();
            while(v_help[x] != 0){
                x = x + 1;
            }
            start_of_cycle = x;
            previous_number = x;
            current_numbers = matrix_greedy_appointment[x];
            x = 0;
        } else {
            v_help[current_numbers] = 1;
            previous_number = matrix_greedy_appointment[previous_number];
            current_numbers = matrix_greedy_appointment[current_numbers];
            v.push_back(previous_number);
        }
    }
    return matrix_cycle_coverage;
}

/*
Функция возвращает строку s, обрезанную справа
на n символов.
*/

string Prefix(string s, int n){
    string s1 = s.substr(0, (s.length() - n));
    return s1;
}

/*
Функция осуществляющая циклический сдвиг каждого цикла в покрытии такой, чтобы
минимизировать overlap первой и последней строки в цикле.
А также конструироет надстроки для каждого цикла.
*/

string Assembly(vector<vector<int>> cycle, vector<vector<int>> matrix, vector<string> substrings){
    string min_string(100, 'a');
    string current_string = "";
    string final_answer = "";
    for(int i = 0; i < cycle.size(); ++i){
        for(int j = 0; j < cycle[i].size(); ++j){
            for(int k = 0; k < cycle[i].size(); ++k){
                if(k == (cycle[i].size() - 1)){
                    int l = cycle[i][k];
                    current_string = current_string + substrings[l];
                } else {
                    int l_1 = cycle[i][k];
                    current_string = current_string + Prefix(substrings[l_1], matrix[cycle[i][k]][cycle[i][k + 1]]);
                }
            }
            rotate(cycle[i].begin(), cycle[i].begin() + 1, cycle[i].end());
            if(min_string.length() > current_string.length()){
                min_string = current_string;
                }
            current_string = "";
        }
        final_answer = final_answer + min_string;
        current_string = "";
        string new_min_string(100, 'a');
        min_string = new_min_string;
    }
    return final_answer;
}

int main() {
    vector<string> substrings; // вектор исходных строк
    int n; // кол- во исходных строк
    cin >> n;
    for(int i = 0; i < n; ++i){
        string s;
        cin >> s;
        substrings.push_back(s);
    }
    vector<vector<int>> matrix = OverlapMatrix(substrings, n);
    // Матрица Overlap
    vector<int> matrix_greedy_appointment = GreedyAppointment(matrix, n);
    // Полное назначение
    vector<vector<int>> cycle = CycleCoverage(matrix_greedy_appointment, n);
    // Нахождение циклов
    string answer = Assembly(cycle,  matrix, substrings);
    // Итоговый ответ(строка, содержащая каждую строку S1..n в качестве подстроки, где длина |X| минимизирована.)
    cout << answer << endl;

    /*
    cout << "matrix" << endl;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Greedy" << endl;
    for(auto x : matrix_greedy_appo
            intment){
        cout << x << " ";
    }
        cout << "cycle " << endl;
    for (auto i:cycle){
        for(auto j:i)
            cout << j << " ";
        cout << "\n";
    }
    */
}
