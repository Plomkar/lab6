#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>

using namespace std;

// Функция для вывода матрицы
void printMatrix(int** matrix, int M, int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

// Функция для подсчёта суммы цифр числа
int sumOfDigits(int num) {
    int sum = 0;
    num = abs(num);
    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

int main() {
    srand(time(0));

    // ЧАСТЬ 1
    cout << "===== ЧАСТЬ 1 =====" << endl;
    int M, N;
    cout << "Введите M и N (M>0, N>0): ";
    cin >> M >> N;

    // Выделение памяти и заполнение матрицы случайными числами [10, 50]
    int** matrix = new int* [M];
    for (int i = 0; i < M; i++) {
        matrix[i] = new int[N];
        for (int j = 0; j < N; j++) {
            matrix[i][j] = 10 + rand() % 41; // [10, 50]
        }
    }

    cout << "\nИсходная матрица:" << endl;
    printMatrix(matrix, M, N);

    // Массив разностей max-min по строкам
    int* diffArray = new int[M];
    int maxDiffRow = 0;

    for (int i = 0; i < M; i++) {
        int minVal = matrix[i][0], maxVal = matrix[i][0];
        int minIndex = 0, maxIndex = 0;

        // Поиск первого минимального и первого максимального
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
                minIndex = j;
            }
            if (matrix[i][j] > maxVal) {
                maxVal = matrix[i][j];
                maxIndex = j;
            }
        }

        // Меняем местами
        swap(matrix[i][minIndex], matrix[i][maxIndex]);

        // Разность после перестановки
        diffArray[i] = maxVal - minVal;
        if (diffArray[i] > diffArray[maxDiffRow]) {
            maxDiffRow = i;
        }
    }

    cout << "\nМатрица после замены min <-> max в каждой строке:" << endl;
    printMatrix(matrix, M, N);

    cout << "\nРазности (max-min) по строкам:" << endl;
    for (int i = 0; i < M; i++) {
        cout << "Строка " << i << ": " << diffArray[i] << endl;
    }
    cout << "Строка с максимальной разностью: " << maxDiffRow << endl;

    // ЧАСТЬ 2
    cout << "\n===== ЧАСТЬ 2 =====" << endl;
    int M2, N2;
    cout << "Введите M2 и N2 (M2>0, N2>0): ";
    cin >> M2 >> N2;

    int** matrix2 = new int* [M2];
    for (int i = 0; i < M2; i++) {
        matrix2[i] = new int[N2];
        for (int j = 0; j < N2; j++) {
            matrix2[i][j] = 100 + rand() % 101; // [100, 200]
        }
    }

    cout << "\nИсходная матрица (100-200):" << endl;
    printMatrix(matrix2, M2, N2);

    int maxSumRow = 0;
    int maxSum = 0;

    for (int i = 0; i < M2; i++) {
        int rowSumDigits = 0;
        for (int j = 0; j < N2; j++) {
            rowSumDigits += sumOfDigits(matrix2[i][j]);
        }
        cout << "Строка " << i << ", сумма цифр: " << rowSumDigits << endl;
        if (rowSumDigits > maxSum) {
            maxSum = rowSumDigits;
            maxSumRow = i;
        }
    }

    cout << "\nСтрока с наибольшей суммой цифр: " << maxSumRow
         << " (сумма = " << maxSum << ")" << endl;

    // Очистка памяти
    for (int i = 0; i < M; i++) delete[] matrix[i];
    delete[] matrix;
    delete[] diffArray;

    for (int i = 0; i < M2; i++) delete[] matrix2[i];
    delete[] matrix2;

    return 0;
}