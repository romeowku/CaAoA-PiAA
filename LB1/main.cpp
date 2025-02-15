// for launch use this command:
// g++ -o a main.cpp -lsfml-graphics -lsfml-window -lsfml-system && ./a

#include <iostream>
#include <vector>
#include <stack>
#include <limits>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

using namespace std;

//****************************************************************************************************
// функция для поиска пустой клетки на поле
pair<int, int> checkEmptyPlace(const vector<vector<int>>& matrix);

// функция для нахождения максимального размера квадрата, который можно разместить в клетке (x, y)
int maxSquareSize(const vector<vector<int>>& matrix, int x, int y);

// функция для проверки, является ли число простым
bool isPrime(int a);

// функция для нахождения первого простого делителя числа
int primeDivisor(int a);

// функция для вывода столешницы в форме (координата х, координата у, размер)
void printResult();

// функция для генерации случайного цвета для квадрата
sf::Color generateRandomColor();

// функция для назначения цветов квадратам 
void assignColors(); 

// функция для визуализации результата с использованием SFML
void visualize();
//****************************************************************************************************

//**************************************__ПЕРЕМЕННЫЕ/СТРУКТУРА__****************************************
struct State 
{
    vector<vector<int>> matrix;  // матрица, представляющая текущее состояние столешницы (0 - пустая клетка, >0 - квадрат)
    int count;  // количество квадратов в текущем разбиении
    vector<tuple<int, int, int>> squares;  // список квадратов, которые уже были добавлены (x, y, размер)
};

int n;  // размер поля (столешницы)
vector<tuple<int, int, int>> best;  // лучшая (минимальная) конфигурация квадратов
vector<sf::Color> square_colors;  // массив для хранения цветов квадратов для визуализации
//****************************************************************************************************

//*****************************************__ФУНКЦИИ__************************************************

// функция для начального разбиения поля на квадраты для заданного размера
vector<tuple<int, int, int>> getInitialSquare(int size) 
{
    vector<tuple<int, int, int>> squares;  // вектор для хранения координат и размеров квадратов
    int half = size / 2;  // для четных размеров и простого
    if (size % 2 == 0) 
    {
        for (int i = 0; i < 2; i++) 
        {
            for (int j = 0; j < 2; j++) 
            {
                squares.emplace_back(i * half, j * half, half);  // разделяем на 4 квадрата одинакового размера
            }
        }
    } else if (isPrime(size)) 
    {
        squares.emplace_back(0, 0, half + 1);  // для простых чисел: разделение на части разного размера
        squares.emplace_back(half + 1, 0, half);
        squares.emplace_back(0, half + 1, half);
    } else {
        int p = primeDivisor(size);  // для составных чисел: разделение на части с учетом простых делителей
        squares.emplace_back(0, 0, size / p * 2);
        squares.emplace_back(size / p * 2, size / p, size / p);
        squares.emplace_back(size / p, size / p * 2, size / p);
    }
    return squares;
}

// итеративный алгоритм для поиска минимального числа квадратов
void startAlgorithm(int n) 
{
    stack<State> s;  // стек для хранения состояний
    vector<vector<int>> matrix_zero(n, vector<int>(n, 0));  // начальная пустая матрица

    int min_squares = numeric_limits<int>::max();  // начальное минимальное количество квадратов

    // получаем начальное разбиение (квадраты)
    vector<tuple<int, int, int>> initial_part = getInitialSquare(n);
    // размещаем квадраты на поле
    for (auto& [x, y, size] : initial_part) 
    {
        for (int i = 0; i < size; i++) 
        {
            for (int j = 0; j < size; j++) 
            {
                matrix_zero[y + i][x + j] = size;  // заполняем клетки значением размера квадрата
            }
        }
    }

    // запушиваем начальное состояние в стек
    s.push({matrix_zero, (int)initial_part.size(), initial_part});

    // начинаем итерации по стеку
    while (!s.empty()) 
    {
        State state = s.top(); s.pop();  // берем текущее состояние из стека

        // если количество квадратов больше или равно минимальному, пропускаем это состояние
        if (state.count >= min_squares) { continue; }

        // находим пустое место на поле
        auto [y, x] = checkEmptyPlace(state.matrix);
        if (y == -1 && x == -1) 
        {
            // если нет пустых мест, обновляем минимальное количество квадратов, если нужно
            if (state.count < min_squares) 
            {
                min_squares = state.count;
                best = state.squares;  // сохраняем лучшее решение
            }
            continue;
        }

        // находим максимальный размер квадрата для текущего места
        int max_size = maxSquareSize(state.matrix, x, y);
        
        // пробуем разместить квадраты разных размеров
        for (int size = 1; size <= max_size; size++) 
        {
            vector<vector<int>> new_matrix = state.matrix;
            // заполняем клетки новым квадратом
            for (int i = 0; i < size; i++) 
            {
                for (int j = 0; j < size; j++) { new_matrix[y + i][x + j] = size; }
            }

            // добавляем квадрат в список и пушим новое состояние в стек
            auto new_squares = state.squares;
            new_squares.emplace_back(x, y, size);
            s.push({new_matrix, state.count + 1, new_squares});
        }
    }
}

void visualize() 
{
    const int cell_size = 50;  // размер клетки в пикселях
    sf::RenderWindow window(sf::VideoMode(n * cell_size, n * cell_size), "Square Partition Visualization");

    srand(time(0));  // инициализация генератора случайных чисел
    assignColors();  // генерация цветов для квадратов

    // ожидание и обработка событий окна
    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) { window.close(); }
        }

        window.clear(sf::Color::White);  // очистка экрана

        // рисуем квадраты с фиксированными цветами
        for (size_t i = 0; i < best.size(); i++) 
        {
            auto [x, y, size] = best[i];
            sf::RectangleShape rect(sf::Vector2f(size * cell_size, size * cell_size));  // создаем квадрат
            rect.setPosition(x * cell_size, y * cell_size);  // устанавливаем позицию
            rect.setFillColor(square_colors[i]);  // устанавливаем цвет квадрата
            window.draw(rect);  // рисуем квадрат
        }

        window.display();  // отображаем все нарисованные элементы на экране
    }
}

int main() 
{
    cin >> n;
    startAlgorithm(n);
    printResult();
    visualize();
    return 0;
}

//**********************************__ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ__***************************************

int maxSquareSize(const vector<vector<int>>& matrix, int x, int y) 
{
    int size = 1;  // начинаем с размера квадрата 1x1
    while (x + size < n && y + size < n && matrix[y][x + size] == 0 && matrix[y + size][x] == 0) 
    {
        // увеличиваем размер квадрата, пока он помещается в поле и не наталкивается на занятые клетки
        size++;
    }
    return size;  // возвращаем максимальный размер квадрата
}

int primeDivisor(int a) 
{
    vector<int> primes = {3, 5, 7, 11, 13, 17, 19, 23};  // набор простых чисел
    for (int prime : primes) 
    {
        if (a % prime == 0) { return prime; }  // если число делится на простое, возвращаем простое число
    }
    return a;  // если нет делителей из списка, возвращаем само число (оно простое)
}

bool isPrime(int a) 
{
    if (a < 2) { return false; }  // числа меньше 2 не простые
    for (int i = 2; i * i <= a; i++) {
        if (a % i == 0) { return false; }  // если число делится на i, оно не простое
    }
    return true;  // число простое
}

pair<int, int> checkEmptyPlace(const vector<vector<int>>& matrix) 
{
    for (int y = 0; y < n; y++) 
    {
        for (int x = 0; x < n; x++) 
        {
            if (matrix[y][x] == 0) { return {y, x}; } // если клетка пустая, возвращаем ее координаты
        }
    }
    return {-1, -1};  // если пустых клеток нет, возвращаем -1, -1
}

sf::Color generateRandomColor() 
{
    return sf::Color(rand() % 256, rand() % 256, rand() % 256);  // случайный цвет
}

void assignColors() 
{
    square_colors.clear();
    for (const auto& [x, y, size] : best) 
    {
        square_colors.push_back(generateRandomColor());  // генерация и сохранение цветов
    }
}

void printResult() 
{
    cout << best.size() << endl;
    for (const auto& [x, y, size] : best) 
    {
        cout << x + 1 << " " << y + 1 << " " << size << endl;
    }
}