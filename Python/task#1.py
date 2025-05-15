import random

############################ Task#1 ##############################
def display_matrix(matrix):
    if not matrix or not matrix[0]:
        print("Матрица пуста!")
        return

    for row in matrix:
        print(" ".join(map(str, row)))

def inizial_matrix_1(por):
    if por % 2 == 1:
        print("Матрица должна быть чётной, введите другое значение!")
        return None

    matrix = []
    for row in range(por):
        current_row = []
        for col in range(por):
            if col % 2 == 1:
                # выбираем случайное число вне диапазона [-10,10]
                while True:
                    elem = random.randint(-50, 50)
                    if elem < -10 or elem > 10:
                        break
            else:
                elem = random.randint(-50, 50)
            current_row.append(elem)
        matrix.append(current_row)

    return matrix
##################################################################

############################ Task#2 ##############################
def inizial_matrix_2(rows, cols):
    matrix = []
    for _ in range(rows):
        row = [random.randint(-100, 100) for _ in range(cols)]
        matrix.append(row)
    return matrix

def modified_matrix(matrix):
    for i, row in enumerate(matrix):
        pos = sum(1 for val in row if val > 0)
        neg = sum(1 for val in row if val < 0)

        if pos > neg:
            middle = len(row) // 2
            # сортируем правую половину по убыванию
            right_part = sorted(row[middle:], reverse=True)
            matrix[i] = row[:middle] + right_part

        elif pos < neg:
            # возводим элементы строки в квадрат
            matrix[i] = [val * val for val in row]

        else:  # pos == neg
            matrix[i] = [0] * len(row)

def display_matrix_2(matrix, cols, rows):
    if not matrix or not matrix[0]:
        print("Матрица пуста!")
        return

    dlina = "------"
    resstr = dlina * rows

    for j in reversed(range(cols)):
        for i in range(rows):
            print(matrix[i][j], end="")
            if i != rows - 1:
                print(" | ", end="")
        if j != 0:
            print("\n" + resstr)
    print()
##################################################################

############################ Task#3 ##############################
def sub_matrix(matrix, rows, cols):
    result = {}
    max_size = min(rows, cols)

    for k in range(1, max_size + 1):
        count = 0
        for i in range(rows - k + 1):
            for j in range(cols - k + 1):
                all_one = True
                for x in range(k):
                    for y in range(k):
                        if matrix[i + x][j + y] != 1:
                            all_one = False
                            break
                    if not all_one:
                        break
                if all_one:
                    count += 1
        result[k] = count
    return result
##################################################################

def main():
    while True:
        task = input("Введите номер задания: ")
        if task == "1":
            matrix = None
            while True:
                command = input("Напишите команду: ")
                if command == "create":
                    while True:
                        n = int(input("Введите порядок матрицы: "))
                        matrix = inizial_matrix_1(n)
                        if matrix is not None:
                            break
                    print("Матрица заполнена случайными числами от -50 до 50(за исключением чётных столбцов(в них нет чисел из диапазона [-10, 10]))")
                elif command == "print":
                    if matrix:
                        display_matrix(matrix)
                    else:
                        print("Матрица не создана.")
                elif command == "exit":
                    break

        elif task == "2":
            row = int(input("Введите длину матрицы(M): "))
            col = int(input("Введите ширину матрицы(N): "))
            matrix = inizial_matrix_2(row, col)
            print("Сгенерированная матрица(MxN):")
            display_matrix_2(matrix, col, row)
            print()

            modified_matrix(matrix)
            print("Модифицированная матрица:")
            display_matrix_2(matrix, col, row)
            print()

        elif task == "3":
            matrix = [
                [1, 1, 1, 0],
                [1, 1, 1, 1],
                [1, 1, 1, 1],
                [0, 1, 1, 0]
            ]
            res = sub_matrix(matrix, 4, 4)
            print("Матрица:")
            display_matrix(matrix)
            print()

            total = sum(res.values())
            print(f"Всего найдено {total} подматриц:")

            for k, count in res.items():
                if count != 0:
                    print(f"{count} подматри(ц/цы/а) со стороной {k}")
            print()

        elif task == "exit":
            break

if __name__ == "__main__":
    main()
