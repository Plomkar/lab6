package main

import (
	"fmt"
)

func copyBoard(board [][]int) [][]int {
	rows := len(board)
	cols := len(board[0])
	newBoard := make([][]int, rows)
	for i := 0; i < rows; i++ {
		newBoard[i] = make([]int, cols)
		copy(newBoard[i], board[i])
	}
	return newBoard
}

func boardToString(board [][]int) string {
	s := ""
	for _, row := range board {
		for _, cell := range row {
			s += fmt.Sprintf("%d", cell)
		}
	}
	return s
}

func flip(board [][]int, x1, y1, x2, y2 int) {
	board[x1][y1] ^= 1
	board[x2][y2] ^= 1
}

func isTarget(board [][]int, targetTop, targetBottom int) bool {
	rows := len(board)
	cols := len(board[0])
	half := rows / 2

	for i := 0; i < rows; i++ {
		target := targetTop
		if i >= half {
			target = targetBottom
		}
		for j := 0; j < cols; j++ {
			if board[i][j] != target {
				return false
			}
		}
	}
	return true
}

func bfs(start [][]int, targetTop, targetBottom int) int {
	type state struct {
		board [][]int
		steps int
	}

	queue := []state{{start, 0}}
	visited := make(map[string]bool)
	visited[boardToString(start)] = true

	rows := len(start)
	cols := len(start[0])

	for len(queue) > 0 {
		curr := queue[0]
		queue = queue[1:]

		if isTarget(curr.board, targetTop, targetBottom) {
			return curr.steps
		}

		for i := 0; i < rows; i++ {
			for j := 0; j < cols; j++ {
				// Горизонтальная пара
				if j+1 < cols {
					newBoard := copyBoard(curr.board)
					flip(newBoard, i, j, i, j+1)
					key := boardToString(newBoard)
					if !visited[key] {
						visited[key] = true
						queue = append(queue, state{newBoard, curr.steps + 1})
					}
				}
				// Вертикальная пара
				if i+1 < rows {
					newBoard := copyBoard(curr.board)
					flip(newBoard, i, j, i+1, j)
					key := boardToString(newBoard)
					if !visited[key] {
						visited[key] = true
						queue = append(queue, state{newBoard, curr.steps + 1})
					}
				}
			}
		}
	}
	return -1
}

func main() {
	// Пример из задания (вариант 3, пример 4)
	board := [][]int{
		{1, 0, 1},
		{0, 1, 0},
		{1, 1, 0},
		{0, 0, 1},
	}

	fmt.Println("Исходная доска (4x3):")
	for _, row := range board {
		for _, cell := range row {
			fmt.Printf("%d ", cell)
		}
		fmt.Println()
	}
	fmt.Println()

	// Целевое состояние: верх = 0, низ = 1
	result := bfs(board, 0, 1)

	fmt.Println("Целевое состояние: верхняя половина = 0, нижняя половина = 1")
	fmt.Printf("Минимальное количество ходов: %d\n", result)
}
