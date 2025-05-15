from typing import List
import math

def display(vec: List[float]):
    for i, val in enumerate(vec):
        print(f"x{i+1} = {val:.3f}")

# Для каждого столбца j зануляем элементы под главной диагональю, вычитая строки
def forward_motion(coefficients: List[List[float]], known: List[float]):
    for j in range(3):
        p = coefficients[j][j]
        for i in range(j, 3):
            r = coefficients[i + 1][j]
            for k in range(j + 1, 4):
                q = coefficients[j][k]
                coefficients[i + 1][k] -= r * q / p
            known[i + 1] -= r * known[j] / p
            coefficients[i + 1][j] = 0.0

# Зануляет элементы над главной диагональю, тем самым получая диагональную матрицу
def back_motion(coefficients: List[List[float]], known: List[float]):
    for j in range(3, -1, -1):
        p = coefficients[j][j]
        for i in range(j - 1, -1, -1):
            r = coefficients[i][j]
            known[i] -= r / p * known[j]
            coefficients[i][j] = 0.0

# Для каждого столбца ищет строку с максимальным по модулю элементом в этом столбце и меняет её с текущей строкой
def main_elements(coefficients: List[List[float]], known: List[float]):
    n = len(coefficients)
    for i in range(n):
        max_ind = i
        for j in range(i+1, n):
            if abs(coefficients[j][i]) > abs(coefficients[max_ind][i]):
                max_ind = j
        if max_ind != i:
            coefficients[i], coefficients[max_ind] = coefficients[max_ind], coefficients[i]
            known[i], known[max_ind] = known[max_ind], known[i]

# Объединяет все шаги решения системы методом Гаусса
def gaus(coefficients: List[List[float]], known: List[float]) -> List[float]:
    main_elements(coefficients, known)
    forward_motion(coefficients, known)
    back_motion(coefficients, known)
    for i in range(4):
        known[i] /= coefficients[i][i]
    return known

# Метод простой итерации(переписывает систему уравнений в виде x = C*x + f)
def simple_it(coefficients: List[List[float]], known: List[float], e: float = 0.001) -> List[float]:
    main_elements(coefficients, known)
    
    n = 4
    C = [[0.0]*n for _ in range(n)]
    for i in range(n):
        for j in range(n):
            if i != j:
                C[i][j] = -coefficients[i][j] / coefficients[i][i]
    
    f = [known[i]/coefficients[i][i] for i in range(n)]
    
    x = [0.0]*n  # начальное приближение
    print(f"N  |\tx1\t|\tx2\t|\tx3\t|\tx4\t|\te\t|")
    K = 0
    while True:
        x_prev = x.copy()
        for i in range(n):
            x[i] = f[i]
            for j in range(n):
                if i != j:
                    x[i] += C[i][j] * x_prev[j]
        eps_count = sum(abs(x[i] - x_prev[i]) < e for i in range(n))
        print(f"{K}  |\t{ x[0]:.3f}\t|\t{ x[1]:.3f}\t|\t{ x[2]:.3f}\t|\t{ x[3]:.3f}\t|\t{e}\t|")
        if eps_count == n:
            return x
        K += 1

def main():
    coefficients = [
        [0.79, -0.04, 0.21, -18.0],
        [0.25, -1.23, 0.18, -0.09],
        [-0.21, 0.18, 0.8, -0.13],
        [0.15, -1.31, 0.06, -0.86]
    ]

    known = [-1.24, -0.86, 2.56, 0.79]

    known_gauss = gaus([row[:] for row in coefficients], known[:])  # копии, чтобы не менять исходные данные
    print("Gauss method:")
    display(known_gauss)

    known_simple_it = simple_it([row[:] for row in coefficients], known[:])
    print("Simple iteration method:")
    display(known_simple_it)

if __name__ == "__main__":
    main()
