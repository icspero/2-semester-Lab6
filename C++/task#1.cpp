#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <map>

using namespace std;

////////////////////////////Task#1//////////////////////////////////////////////
void displayMatrix(vector<vector<int>> matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        cout << "Матрица пуста!" << endl;
        return;
    }

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int inizialMatrix1(vector<vector<int>>& matrix, int por, mt19937& gen) {
    if (por % 2 == 1) {
        cout << "Матрица должна быть чётной, введите другое значение!" << endl;
        return 0;
    }

    matrix.clear();

    uniform_int_distribution<> dist(-50, 50);

    for (int row = 0; row < por; row++) {
        vector<int> str;
        for (int col = 0; col < por; col++) {
            int elem;
            if (col % 2 == 1) {
                do {
                    elem = dist(gen);
                } while (elem >= -10 && elem <= 10);
            } else {
                elem = dist(gen);
            }
            str.push_back(elem);
        }
        matrix.push_back(str);
    }

    return 1;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////Task#2//////////////////////////////////////////////
int inizialMatrix2(vector<vector<int>>& matrix, int str, int col, mt19937& gen) {
    uniform_int_distribution<> dist(-100, 100);

    matrix.clear(); // на случай, если матрица уже была заполнена

    for (int i = 0; i < str; i++) {
        vector<int> row;
        for (int j = 0; j < col; j++) {
            int elem = dist(gen);
            row.push_back(elem);
        }
        matrix.push_back(row);
    }

    return 1;
}

int modifiedMatrix(vector<vector<int>>& matrix) {
    vector<pair<int, int>> pos_neg;
    for (const auto& row : matrix) {
        int pos = 0, neg = 0;
        for (int val : row) {
            if (val > 0) pos++;
            else if (val < 0) neg++;
        }
        pos_neg.push_back({pos, neg});
    }

    int k = 0;
    for (const auto& pair : pos_neg) {
        if (pair.first > pair.second) {
            auto middle = matrix[k].begin() + matrix[k].size()/2;
            sort(middle, matrix[k].end());
            reverse(middle, matrix[k].end());
            k += 1;
        }
        if (pair.first < pair.second) {
            for (int i = 0; i < matrix[k].size(); i++) {
                matrix[k][i] = matrix[k][i] * matrix[k][i];
            }
            k += 1;
        }
        if (pair.first == pair.second) {
            for (int i = 0; i < matrix[k].size(); i++) {
                matrix[k][i] = 0;
            }
            k += 1;
        }
    }

    return 1;
}

void displayMatrix2(vector<vector<int>> matrix, int col, int row) {
    if (matrix.empty() || matrix[0].empty()) {
        cout << "Матрица пуста!" << endl;
        return;
    }

    string dlina = "------";
    string resstr = "";
    for (int i = 0; i < row; i++) {
        resstr += dlina;
    }

    for (int j = col - 1; j > -1; j--) {
        for (int i = 0; i < row; i++) {
            cout << matrix[i][j];
            if (i != row - 1) {
                cout << " | ";
            }
        }
        if (j != 0) {
            cout << endl << resstr << endl;
        } 
    }
    cout << endl;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////Task#3//////////////////////////////////////////////
map<int, int> subMatrix(vector<vector<int>>& matrix, int str, int col, mt19937& gen) {
    map<int, int> result;

    int maxSize = min(str, col);
    for (int k = 1; k <= maxSize; ++k) {
        int count = 0;
        // Перебираем все возможные позиции верхнего левого угла подматрицы k×k
        for (int i = 0; i <= str - k; ++i) {
            for (int j = 0; j <= col - k; ++j) {
                bool allOne = true;
                for (int x = 0; x < k && allOne; ++x) {
                    for (int y = 0; y < k; ++y) {
                        if (matrix[i + x][j + y] != 1) {
                            allOne = false;
                            break;
                        }
                    }
                }
                if (allOne) {
                    ++count;
                }
            }
        }
        result[k] = count;
    }

    return result;
}
////////////////////////////////////////////////////////////////////////////////

int main() {
    random_device rd;
    mt19937 gen(rd());

    while (true) {
        cout << "Введите номер задания: ";
        string task;
        cin >> task;
        if (task == "1") {
            vector<vector<int>> matrix;
            while(true) {
                cout << "Напишите команду: ";
                string str;
                cin >> str;
                if (str == "create") {
                    int n;
                    while (true) {
                        cout << "Введите порядок матрицы: ";
                        cin >> n;
                        int res = inizialMatrix1(matrix, n, gen);
                        if (res == 1) {
                            break;
                        }
                    }
                    cout << "Матрица заполнена случайными числами от -50 до 50(за исключением чётных столбцов(в них нет чисел из диапазона [-10, 10]))\n";
                } 
                if (str == "print") {
                    displayMatrix(matrix);
                }
                if (str == "exit") {
                    break;
                }
            }
        }
        if (task == "2") {
            vector<vector<int>> matrix;
            int row, col;
            cout << "Введите длину матрицы(M): ";
            cin >> row;
            cout << "Введите ширину матрицы(N): ";
            cin >> col;
            int res = inizialMatrix2(matrix, row, col, gen);
            cout << "Сгенерированная матрица(MxN):\n";
            displayMatrix2(matrix, col, row);
            cout << endl;

            cout << "Модифицированная матрица:\n";
            res = modifiedMatrix(matrix);
            displayMatrix2(matrix, col, row);
            cout << endl;
        }
        if (task == "3") {
            vector<vector<int>> matrix = {
                {1, 1, 1, 0},
                {1, 1, 1, 1},
                {1, 1, 1, 1},
                {0, 1, 1, 0}
            };
            
            map<int, int> res = subMatrix(matrix, 4, 4, gen);
            cout << "Матрица:" << endl;
            displayMatrix(matrix);
            cout << endl;

            int k = 0;
            for (const auto& i : res) {
                k += i.second;
            }

            cout << "Всего найдено " << k << " подматриц:" << endl;

            int i = 0;
            for (const auto& i : res) {
                if (i.second != 0) {
                    cout << i.second << " подматри(ц/цы/а) со стороной " << i.first << endl;
                }
            }
            cout << endl;
        }
        if (task == "exit") {
            break;
        }
    }
}