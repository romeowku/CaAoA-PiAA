import math
from collections import deque

# функция для поиска пустой клетки на поле
def check_empty_place(matrix):
    for y in range(len(matrix)):
        for x in range(len(matrix[y])):
            if matrix[y][x] == 0:  # если клетка пустая
                return y, x
    return -1, -1  # если пустых клеток нет

# функция для нахождения максимального размера квадрата, который можно разместить в клетке (x, y)
def max_square_size(matrix, x, y):
    size = 1
    while x + size < len(matrix) and y + size < len(matrix) and matrix[y][x + size] == 0 and matrix[y + size][x] == 0:
        size += 1
    return size

# функция для проверки, является ли число простым
def is_prime(a):
    if a < 2:
        return False
    for i in range(2, int(math.sqrt(a)) + 1):
        if a % i == 0:
            return False
    return True

# функция для нахождения первого простого делителя числа
def prime_divisor(a):
    primes = [3, 5, 7, 11, 13, 17, 19, 23]
    for prime in primes:
        if a % prime == 0:
            return prime
    return a

# функция для начального разбиения поля на квадраты для заданного размера
def get_initial_square(size):
    squares = []
    half = size // 2
    if size % 2 == 0:
        for i in range(2):
            for j in range(2):
                squares.append((i * half, j * half, half))  # разделяем на 4 квадрата одинакового размера
    elif is_prime(size):
        squares.append((0, 0, half + 1))  # для простых чисел: разделение на части разного размера
        squares.append((half + 1, 0, half))
        squares.append((0, half + 1, half))
    else:
        p = prime_divisor(size)  # для составных чисел: разделение на части с учетом простых делителей
        squares.append((0, 0, size // p * 2))
        squares.append((size // p * 2, size // p, size // p))
        squares.append((size // p, size // p * 2, size // p))
    return squares

# итертивный алгоритм для поиска минимального числа квадратов
def start_algorithm(n):
    best = []
    min_squares = float('inf')
    matrix_zero = [[0] * n for _ in range(n)]  # начальная пустая матрица

    # получаем начальное разбиение (квадраты)
    initial_part = get_initial_square(n)
    # размещаем квадраты на поле
    for x, y, size in initial_part:
        for i in range(size):
            for j in range(size):
                matrix_zero[y + i][x + j] = size

    # используем стек для хранения состояний
    stack = deque([(matrix_zero, len(initial_part), initial_part)])

    # начинаем итерации по стеку
    while stack:
        matrix, count, squares = stack.pop()

        # если количество квадратов больше или равно минимальному, пропускаем это состояние
        if count >= min_squares:
            continue

        # находим пустое место на поле
        y, x = check_empty_place(matrix)
        if y == -1 and x == -1:
            # если нет пустых мест, обновляем минимальное количество квадратов, если нужно
            if count < min_squares:
                min_squares = count
                best = squares
            continue

        # находим максимальный размер квадрата для текущего места
        max_size = max_square_size(matrix, x, y)

        # пробуем разместить квадраты разных размеров
        for size in range(1, max_size + 1):
            new_matrix = [row[:] for row in matrix]  # копируем текущую матрицу
            for i in range(size):
                for j in range(size):
                    new_matrix[y + i][x + j] = size

            # добавляем квадрат в список и пушим новое состояние в стек
            new_squares = squares + [(x, y, size)]
            stack.append((new_matrix, count + 1, new_squares))

    return best

def print_result(best):
    print(len(best))
    for x, y, size in best:
        print(x + 1, y + 1, size)

def main():
    n = int(input())
    best = start_algorithm(n)
    print_result(best)

if __name__ == "__main__":
    main()
