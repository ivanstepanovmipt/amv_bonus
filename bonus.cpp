#include <iostream>
#include <string>
#include <vector>

using namespace std;

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

vector<vector<int>> AdjacencyMatrix(vector<string> substrings, int n){
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


vector<int> GreedyAppointment(vector<vector<int>> matrix, int n){
    vector<int> matrix_greedy_appointment(n);
    int max_elemen = matrix[0][0];
    int max_line_number = 0;
    int max_column_number = 0;
    vector<vector<int>> matrix_help(n, vector<int> (n));
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

vector<vector<int>> CycleCoverage(vector<int> matrix_greedy_appointment, int n){
    vector<vector<int>> matrix_cycle_coverage;
    vector<int> v;
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
    int x = 0;
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

string Assembly(vector<vector<int>> cycle){
    string min_string ;
    vector<string> final_answer;
    for(auto s_string:cycle){
        for(auto j:s_string){
            for(int i = 0; i < s_string.length(); ++i){
                min_string = min_string + s_string[i];
            }
            min_string = min_string + j;

        }
    }


string Prefix(string s, int n){
    string s1 = s.substr(0, (s.length() - n));
}

int main() {
    vector<string> substrings;
    int n;
    cin >> n;
    for(int i = 0; i < n; ++i){
        string s;
        cin >> s;
        substrings.push_back(s);
    }
    vector<vector<int>> matrix = AdjacencyMatrix(substrings, n);
    /*
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    */
    vector<int> matrix_greedy_appointment = GreedyAppointment(matrix, n);
    /*
    for(auto x : matrix_greedy_appointment){
        cout << x << " ";
    }
    */
    vector<vector<int>> cycle = CycleCoverage(matrix_greedy_appointment, n);
    /*
    for (auto i:v){
        for(auto j:i)
            cout << j << " ";
        cout << "\n";
    }
    */
}