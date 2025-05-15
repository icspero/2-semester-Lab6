#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

void display(const vector<float>& vec1) {
    for (int i = 0; i < vec1.size(); i++) {
        cout <<fixed << setprecision(3)<< "x" << i+1 << " = " << vec1[i] << endl;
    }
}

void forwardMotion(vector<vector<float>>& coefficients, vector<float>& known) {
    float p, q, r;
    for (int j = 0; j < 3; j++) {
        p = coefficients[j][j];
        for (int i = j; i < 3; i++) {
            r = coefficients[i + 1][j];
            for (int k = j + 1; k < 4; k++) {
                q = coefficients[j][k];
                coefficients[i + 1][k] -= r * q / p;
            }
            known[i + 1] -= r * known[j] / p;
            coefficients[i + 1][j] = 0;
        }
    }
}

void backMotion(vector<vector<float>>& coefficients, vector<float>& known) {
    float p, r;
    for (int j = 3; j >= 0; --j) {
        p = coefficients[j][j];
        for (int i = j - 1; i >= 0; --i) {
            r = coefficients[i][j];
            known[i] -= r / p * known[j];
            coefficients[i][j] = 0;
        }
    }
}

void mainElements(vector<vector<float>>& coefficients, vector<float>& known) {
    int maxInd = 0;
    for (int i = 0; i < coefficients.size(); i++) {
        for (int j = 0; j < coefficients.size(); j++) {
            if (abs(coefficients[j][i]) > abs(coefficients[maxInd][i])) maxInd = j;
        }
        swap(coefficients[i], coefficients[maxInd]);
        swap(known[i], known[maxInd]);
    }
}

vector<float> gaus(vector<vector<float>> coefficients, vector<float> known) {
    mainElements(coefficients, known);
    forwardMotion(coefficients, known);
    backMotion(coefficients, known);
    for (int i = 0; i < 4; i++) {
        known[i] /= coefficients[i][i];
    }
    return known;
}

vector<float> simpleIt(vector<vector<float>> coefficients, vector<float> known, float e = 0.001) {
    mainElements(coefficients, known);
    vector<vector<float>> C(4, vector<float>(4, 0));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i != j) {
                C[i][j] = -(coefficients[i][j] / coefficients[i][i]);
            }
        }
    }
    vector<float> f;
    f.reserve(known.size());
    for (int i = 0; i < known.size(); i++) {
        f.push_back(known[i] / coefficients[i][i]);
    }
    int epsCount;
    vector<float> x = { 0,0,0,0 };
    cout<<"N"<<"|\t"<<"x1"<<"\t|\t\t"<<"x2"<<"\t|\t\t"<<"x3"<<"\t|\t\t"<<"x4"<<"\t|\t\t"<<"e"<<"\t|\t"<<endl;
    int K=0;
    while (true) {
        vector<float> x_prev = x; // Сохраняем предыдущие значения x
        for (int i = 0; i < 4; i++) {
            x[i] = f[i]; // Сначала устанавливаем значения f
            for (int j = 0; j < 4; j++) {
                if (i != j) {
                    x[i] += C[i][j] * x_prev[j]; // Добавляем элементы матрицы C, умноженные на предыдущие значения x
                }
            }
        }
        epsCount = 0;
        for (int i = 0; i < 4; i++) {
            if (abs(x[i] - x_prev[i]) < e) {
                epsCount++; // Проверяем сходимость итераций
            }
        }
        for(int i=0;i<1;i++){
         cout<<K<<fixed << setprecision(3) <<"|\t"<<x[0]<<"\t|\t\t"<<x[1]<<"\t|\t\t"<<x[2]<<"\t|\t\t"<<x[3]<<"\t|\t\t"<<e<<"\t|\t"<<endl;
         }
        if (epsCount == 4) {
          
            return x;
        }
        K++;
    }
}

int main() {
    vector<vector<float>> coefficients = {
            {0.79, -0.04, 0.21, -18.0},
            {0.25, -1.23, 0.18, -0.09},
            {-0.21, 0.18, 0.8, -0.13},
            {0.15, -1.31, 0.06, -0.86} };

    vector<float> known = { -1.24,
                           -0.86,
                           2.56,
                           0.79 };

    vector<float> knownGauss = gaus(coefficients, known);
    cout << "Gauss method:" << endl;
    display(knownGauss);

    vector<float> knownSimpleIt = simpleIt(coefficients, known);
    cout << "Simple iteration method:" << endl;
    display(knownSimpleIt);
}