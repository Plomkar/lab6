package main

import (
	"fmt"
	"math"
)

// Прямой метод Гаусса с выбором главного элемента
func gauss(A [][]float64, b []float64) []float64 {
	n := len(A)
	// Копируем матрицу и вектор
	AA := make([][]float64, n)
	for i := 0; i < n; i++ {
		AA[i] = make([]float64, n)
		copy(AA[i], A[i])
	}
	bb := make([]float64, n)
	copy(bb, b)

	// Прямой ход
	for i := 0; i < n; i++ {
		// Поиск главного элемента
		maxRow := i
		maxVal := math.Abs(AA[i][i])
		for k := i + 1; k < n; k++ {
			if math.Abs(AA[k][i]) > maxVal {
				maxVal = math.Abs(AA[k][i])
				maxRow = k
			}
		}
		// Перестановка строк
		if maxRow != i {
			AA[i], AA[maxRow] = AA[maxRow], AA[i]
			bb[i], bb[maxRow] = bb[maxRow], bb[i]
		}

		// Нормализация
		div := AA[i][i]
		for j := i; j < n; j++ {
			AA[i][j] /= div
		}
		bb[i] /= div

		// Вычитание
		for k := i + 1; k < n; k++ {
			factor := AA[k][i]
			for j := i; j < n; j++ {
				AA[k][j] -= factor * AA[i][j]
			}
			bb[k] -= factor * bb[i]
		}

		// Вывод промежуточных результатов
		fmt.Printf("\nПосле шага %d:\n", i+1)
		for r := 0; r < n; r++ {
			for c := 0; c < n; c++ {
				fmt.Printf("%8.4f ", AA[r][c])
			}
			fmt.Printf(" | %8.4f\n", bb[r])
		}
	}

	// Обратный ход
	x := make([]float64, n)
	for i := n - 1; i >= 0; i-- {
		x[i] = bb[i]
		for j := i + 1; j < n; j++ {
			x[i] -= AA[i][j] * x[j]
		}
	}
	return x
}

// Приведение к виду x = Cx + f
func toIterationForm(A [][]float64, b []float64) (C [][]float64, f []float64) {
	n := len(A)
	C = make([][]float64, n)
	f = make([]float64, n)
	for i := 0; i < n; i++ {
		C[i] = make([]float64, n)
		diag := A[i][i]
		f[i] = b[i] / diag
		for j := 0; j < n; j++ {
			if j != i {
				C[i][j] = -A[i][j] / diag
			}
		}
	}
	return
}

// Метод простых итераций
func simpleIteration(C [][]float64, f []float64, eps float64) ([]float64, int) {
	n := len(C)
	x := make([]float64, n) // начальное приближение
	xNew := make([]float64, n)
	iter := 0

	fmt.Println("\nТаблица итераций:")
	fmt.Println("N\tx1\t\tx2\t\tx3\t\tx4")

	for {
		for i := 0; i < n; i++ {
			xNew[i] = f[i]
			for j := 0; j < n; j++ {
				xNew[i] += C[i][j] * x[j]
			}
		}
		iter++

		// Вывод
		fmt.Printf("%d\t", iter)
		for i := 0; i < n; i++ {
			fmt.Printf("%.6f\t", xNew[i])
		}
		fmt.Println()

		// Проверка сходимости
		maxDiff := 0.0
		for i := 0; i < n; i++ {
			diff := math.Abs(xNew[i] - x[i])
			if diff > maxDiff {
				maxDiff = diff
			}
		}

		x = xNew

		if maxDiff < eps {
			break
		}
		if iter > 1000 {
			fmt.Println("Превышено максимальное число итераций!")
			break
		}
	}
	return x, iter
}

func main() {
	// Матрица A и вектор b из варианта 2
	A := [][]float64{
		{0.89, -0.04, 0.21, -1.16},
		{0.25, -1.23, 0.12, -0.09},
		{-0.21, 0.12, 0.8, -0.13},
		{0.15, -1.31, 0.06, -1.15},
	}
	b := []float64{-1.24, -1.15, 2.56, 0.89}

	fmt.Println("==========================================")
	fmt.Println("Метод Гаусса с выбором главного элемента")
	fmt.Println("==========================================")

	xGauss := gauss(A, b)

	fmt.Println("\nРешение системы (метод Гаусса):")
	for i := 0; i < len(xGauss); i++ {
		fmt.Printf("x%d = %.6f\n", i+1, xGauss[i])
	}

	fmt.Println("\n==========================================")
	fmt.Println("Метод простых итераций")
	fmt.Println("==========================================")

	C, f := toIterationForm(A, b)

	// Проверка нормы
	normC := 0.0
	for i := 0; i < len(C); i++ {
		rowSum := 0.0
		for j := 0; j < len(C); j++ {
			rowSum += math.Abs(C[i][j])
		}
		if rowSum > normC {
			normC = rowSum
		}
	}
	fmt.Printf("Норма матрицы C: %.6f\n", normC)
	if normC < 1 {
		fmt.Println("Условие сходимости выполняется.")
	} else {
		fmt.Println("Условие сходимости не гарантировано.")
	}

	eps := 1e-3
	xIter, iter := simpleIteration(C, f, eps)

	fmt.Printf("\nРешение системы (простые итерации, eps = %.0e):\n", eps)
	for i := 0; i < len(xIter); i++ {
		fmt.Printf("x%d = %.6f\n", i+1, xIter[i])
	}
	fmt.Printf("Количество итераций: %d\n", iter)
}
