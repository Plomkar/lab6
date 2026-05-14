#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Прямой метод Гаусса с выбором главного элемента по столбцу
vector<double> gauss(vector<vector<double>> A, vector<double> b) {
    int n = A.size();
    
    // Прямой ход
    for (int i = 0; i < n; i++) {
        // Поиск главного элемента в столбце i
        int maxRow = i;
        double maxVal = fabs(A[i][i]);
        for (int k = i + 1; k < n; k++) {
            if (fabs(A[k][i]) > maxVal) {
                maxVal = fabs(A[k][i]);
                maxRow = k;
            }
        }
        // Перестановка строк
        if (maxRow != i) {
            swap(A[i], A[maxRow]);
            swap(b[i], b[maxRow]);
        }
        
        // Нормализация строки i
        double div = A[i][i];
        for (int j = i; j < n; j++) {
            A[i][j] /= div;
        }
        b[i] /= div;
        
        // Вычитание из нижних строк
        for (int k = i + 1; k < n; k++) {
            double factor = A[k][i];
            for (int j = i; j < n; j++) {
                A[k][j] -= factor * A[i][j];
            }
            b[k] -= factor * b[i];
        }
        
        // Вывод промежуточных результатов
        cout << "\nПосле шага " << i + 1 << ":\n";
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                cout << fixed << setprecision(4) << A[r][c] << "\t";
            }
            cout << " | " << b[r] << endl;
        }
    }
    
    // Обратный ход
    vector<double> x(n);
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= A[i][j] * x[j];
        }
    }
    return x;
}

// Приведение к виду x = Cx + f
void toIterationForm(vector<vector<double>> A, vector<double> b, 
                     vector<vector<double>>& C, vector<double>& f) {
    int n = A.size();
    C.assign(n, vector<double>(n, 0.0));
    f.assign(n, 0.0);
    
    for (int i = 0; i < n; i++) {
        double diag = A[i][i];
        f[i] = b[i] / diag;
        for (int j = 0; j < n; j++) {
            if (j != i) {
                C[i][j] = -A[i][j] / diag;
            }
        }
    }
}

// Метод простых итераций
vector<double> simpleIteration(vector<vector<double>> C, vector<double> f, 
                                double eps, int& iterations) {
    int n = C.size();
    vector<double> x(n, 0.0); // начальное приближение
    vector<double> x_new(n);
    iterations = 0;
    
    cout << "\nТаблица итераций:\n";
    cout << "N\tx1\t\tx2\t\tx3\t\tx4\n";
    
    while (true) {
        // Вычисление нового приближения
        for (int i = 0; i < n; i++) {
            x_new[i] = f[i];
            for (int j = 0; j < n; j++) {
                x_new[i] += C[i][j] * x[j];
            }
        }
        
        iterations++;
        
        // Вывод текущей итерации
        cout << iterations << "\t";
        for (int i = 0; i < n; i++) {
            cout << fixed << setprecision(6) << x_new[i] << "\t";
        }
        cout << endl;
        
        // Проверка сходимости
        double maxDiff = 0.0;
        for (int i = 0; i < n; i++) {
            maxDiff = max(maxDiff, fabs(x_new[i] - x[i]));
        }
        
        x = x_new;
        
        if (maxDiff < eps) {
            break;
        }
        
        if (iterations > 1000) {
            cout << "Превышено максимальное число итераций!\n";
            break;
        }
    }
    
    return x;
}

int main() {
    // Матрица A и вектор b из варианта 2
    vector<vector<double>> A = {
        {0.89, -0.04, 0.21, -1.16},
        {0.25, -1.23, 0.12, -0.09},
        {-0.21, 0.12, 0.8, -0.13},
        {0.15, -1.31, 0.06, -1.15}
    };
    vector<double> b = {-1.24, -1.15, 2.56, 0.89};
    
    cout << "==========================================\n";
    cout << "Метод Гаусса с выбором главного элемента\n";
    cout << "==========================================\n";
    
    vector<double> x_gauss = gauss(A, b);
    
    cout << "\nРешение системы (метод Гаусса):\n";
    for (int i = 0; i < x_gauss.size(); i++) {
        cout << "x" << i + 1 << " = " << fixed << setprecision(6) << x_gauss[i] << endl;
    }
    
    cout << "\n==========================================\n";
    cout << "Метод простых итераций\n";
    cout << "==========================================\n";
    
    vector<vector<double>> C;
    vector<double> f;
    toIterationForm(A, b, C, f);
    
    // Проверка условия сходимости (норма C < 1)
    double normC = 0.0;
    for (int i = 0; i < C.size(); i++) {
        double rowSum = 0.0;
        for (int j = 0; j < C.size(); j++) {
            rowSum += fabs(C[i][j]);
        }
        normC = max(normC, rowSum);
    }
    cout << "Норма матрицы C: " << normC << endl;
    if (normC < 1) {
        cout << "Условие сходимости выполняется.\n";
    } else {
        cout << "Условие сходимости не гарантировано.\n";
    }
    
    int iterations;
    double eps = 1e-3;
    vector<double> x_iter = simpleIteration(C, f, eps, iterations);
    
    cout << "\nРешение системы (простые итерации, eps = " << eps << "):\n";
    for (int i = 0; i < x_iter.size(); i++) {
        cout << "x" << i + 1 << " = " << fixed << setprecision(6) << x_iter[i] << endl;
    }
    cout << "Количество итераций: " << iterations << endl;
    
    return 0;
}