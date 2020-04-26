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

/*
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

/*
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

/*
Функция делает, следующие : из каждого элемента каждой строки вычитается минимальный элемент данной строки.
При этом минимальный элемент строки прибавляется к нижней границе
Функция возвращает нижнюю границу
*/

vector<int> BottomLine(vector<vector<vector<int>>> matrix, int n, vector<int> answer){
    if(n > 2){
    int min_elemen = 10000;
    vector<int> min_elements_line;
    vector<int> min_elements_column;
    vector<vector<vector<int>>> copy_matrix = matrix;
    /*
    В каждой строке матрицы стоимости найдем минимальный элемент
    и вычтем его из всех элементов строки. Сделаем это и для столбцов,
    не содержащих нуля.
    Получим матрицу стоимости, каждая строка и каждый столбец
    которой содержат хотя бы один нулевой элемент
    */
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(i != j){
                if(matrix[i][j][0] < min_elemen){
                    min_elemen = matrix[i][j][0];
                }
            }
        }
        min_elements_line.push_back(min_elemen);
        min_elemen = 10000;
    }
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
                if(i != j){
                matrix[i][j][0] = matrix[i][j][0] - min_elements_line[i];
                }
            }
        }
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(i != j){
                if(matrix[j][i][0] < min_elemen){
                    min_elemen = matrix[j][i][0];
                }
            }
        }
        min_elements_column.push_back(min_elemen);
        min_elemen = 10000;
    }
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
                if(i != j){
                    matrix[j][i][0] = matrix[j][i][0] - min_elements_column[i];
                }
            }
        }
    min_elements_column.clear();
    min_elements_line.clear();
    /*
    Для каждого нулевого элемента матрицы, рассчитываем коэффициент k, который
    равен сумме минимальных элементов столбца и строки этого нуля.
    Выбираем нуль с максимальным коэффициентом
    */
    vector<vector<vector<int>>> matrix_coefficient;
    vector<int> v;
    vector<vector<int>> vv;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            v.push_back(0);
            v.push_back(0);
            v.push_back(0);
            vv.push_back(v);
            v.clear();
        }
        matrix_coefficient.push_back(vv);
        vv.clear();
    }
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(i != j && matrix[i][j][0] == 0){
            int min_column = 1000000;
            int min_line = 100000;
            int coefficient = 0;
                for(int k = 0; k < n; ++k){
                    if((matrix[i][k][0] < min_line) && (k != j)){
                        min_line = matrix[i][k][0];
                    }
                }
                for(int k = 0; k < n; ++k){
                    if((matrix[k][j][0] < min_column) && (k != i)){
                        min_column = matrix[k][j][0];
                    }
                }
                coefficient = min_column + min_line;
                matrix_coefficient[i][j][0] = coefficient;
                matrix_coefficient[i][j][1] = i;
                matrix_coefficient[i][j][2] = j;
            }
        }
    }
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            cout << matrix_coefficient[i][j][0] << " ";
        }
        cout << endl;
    }
    int max_elemen = 0;
    int max_index_line = 0;
    int max_index_column = 0;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(i != j && (matrix[i][j][0] > max_elemen)){
                max_elemen = matrix[i][j][0];
                max_index_line = matrix[i][j][1];
                max_index_column = matrix[i][j][2];
            }
        }
    }
    answer.push_back((max_index_line + 1));
    answer.push_back((max_index_column + 1));
    cout << " k";
    /*
    Удаляем строку и столбец, на пересечении которого выбранный нами нуль.
    */
    vector<vector<vector<int>>> deleted_item;
    vector<vector<int>> v2;
    vector<int> v3;
    for(int i = 0; i < n; ++i){
        if(i != max_index_line){
        for(int j = 0; j < n; ++j){
            if(j != max_index_column){
                v3.push_back(copy_matrix[i][j][0]);
                v3.push_back(i);
                v3.push_back(j);
                v2.push_back(v3);
                v3.clear();
                cout << " n ";
            }
        }
        deleted_item.push_back(v2);
        cout << " nn ";
        v2.clear();
        }
    }
    cout << " deleted ";
    for(int i = 0; i < (n - 1); ++i){
        for(int j = 0; j < (n - 1); ++j){
            cout << deleted_item[i][j][0] << " ";
        }
        cout << endl;
    }
    cout << " answer" << endl;
    for(auto x : answer){
        cout << x << " " << endl;
    }
    return answer = BottomLine(deleted_item, n - 1, answer);
    } else {
        return answer;
    }
}



int main() {
    vector<string> substrings; // вектор исходных строк
    int n; // кол- во исходных строк
    cin >> n;
    vector<int> answer;
    vector<vector<vector<int>>> matrix;
    vector<int> v;
    vector<vector<int>> vv;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            v.push_back(0);
            v.push_back(0);
            v.push_back(0);
            vv.push_back(v);
            v.clear();
        }
        matrix.push_back(vv);
        vv.clear();
    }
    /*
    for(int i = 0; i < n; ++i){
        string s;
        cin >> s;
        substrings.push_back(s);
    }
    vector<vector<int>> matrix = OverlapMatrix(substrings, n);
    int current_bottom_sum;
    // Матрица Overlap
    */
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            cin >> matrix[i][j][0];
            matrix[i][j][1] = i;
            matrix[i][j][2] = j;

        }
    }
    answer = BottomLine(matrix, n, answer);
    cout << "matrix" << endl;
    for(auto x : answer){
        cout << x << " ";
    }
    return 0;
}
