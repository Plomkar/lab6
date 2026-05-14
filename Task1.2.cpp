#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // для usleep

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 20;

void copyField(bool source[HEIGHT][WIDTH], bool dest[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            dest[i][j] = source[i][j];
}

int countNeighbors(bool field[HEIGHT][WIDTH], int x, int y) {
    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < HEIGHT && ny >= 0 && ny < WIDTH && field[nx][ny])
                count++;
        }
    }
    return count;
}

void updateField(bool field[HEIGHT][WIDTH]) {
    bool newField[HEIGHT][WIDTH];
    copyField(field, newField);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int neighbors = countNeighbors(field, i, j);
            if (field[i][j]) {
                if (neighbors < 2 || neighbors > 3)
                    newField[i][j] = false;
            } else {
                if (neighbors == 3)
                    newField[i][j] = true;
            }
        }
    }
    copyField(newField, field);
}

void printField(bool field[HEIGHT][WIDTH]) {
    cout << "\033[2J\033[1;1H"; // очистка и перемещение курсора (Linux)
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << (field[i][j] ? "#" : ".");
        }
        cout << endl;
    }
    cout << "=== Game of Life (Ctrl+C для выхода) ===" << endl;
}

void setBlock(bool field[HEIGHT][WIDTH], int x, int y) {
    field[x][y] = field[x][y+1] = field[x+1][y] = field[x+1][y+1] = true;
}

void setBeehive(bool field[HEIGHT][WIDTH], int x, int y) {
    field[x][y+1] = field[x][y+2] = true;
    field[x+1][y] = field[x+1][y+3] = true;
    field[x+2][y+1] = field[x+2][y+2] = true;
}

void setLoaf(bool field[HEIGHT][WIDTH], int x, int y) {
    field[x][y+1] = field[x][y+2] = true;
    field[x+1][y] = field[x+1][y+3] = true;
    field[x+2][y+1] = field[x+2][y+3] = true;
    field[x+3][y+2] = true;
}

void setBoat(bool field[HEIGHT][WIDTH], int x, int y) {
    field[x][y] = field[x][y+1] = true;
    field[x+1][y] = field[x+1][y+2] = true;
    field[x+2][y+1] = true;
}

int main() {
    srand(time(0));
    bool field[HEIGHT][WIDTH] = { false };

    cout << "Выберите демонстрацию:\n1 - Block\n2 - Beehive\n3 - Loaf\n4 - Boat\n5 - Случайная колония\nВаш выбор: ";
    int choice;
    cin >> choice;

    switch (choice) {
        case 1: setBlock(field, HEIGHT/2-1, WIDTH/2-1); break;
        case 2: setBeehive(field, HEIGHT/2-2, WIDTH/2-3); break;
        case 3: setLoaf(field, HEIGHT/2-3, WIDTH/2-4); break;
        case 4: setBoat(field, HEIGHT/2-2, WIDTH/2-2); break;
        case 5:
            for (int i = 0; i < HEIGHT; i++)
                for (int j = 0; j < WIDTH; j++)
                    field[i][j] = (rand() % 4 == 0);
            break;
        default: return 0;
    }

    int generations = 0;
    while (true) {
        printField(field);
        cout << "Поколение: " << generations++ << endl;
        updateField(field);
        usleep(250000); // 250 мс (в микросекундах)
    }

    return 0;
}