import os
from typing import List, Tuple

# Запись в файл
def input_to_file(sender_file: str) -> None:
    with open(sender_file, 'w') as out:
        while True:
            str_line = input()
            if str_line == "EOF" or not str_line:
                break
            out.write(str_line + '\n')

# Чтение из файла в строку
def file_to_str(sender_file: str) -> str:
    with open(sender_file, 'r') as in_file:
        return in_file.read()

# Преобразование строки в массив байтов
def str_to_vec(input_str: str) -> List[int]:
    return [ord(c) for c in input_str]

# Разбиение сообщения на блоки по 16 байт
def message_to_blocks(message: List[int]) -> List[List[int]]:
    output = []
    for i in range((len(message) + 15) // 16):
        block = []
        for j in range(16):
            idx = 16 * i + j
            if idx < len(message):
                block.append(message[idx])
            else:
                block.append(ord(' '))  # Дополняем пробелами
        output.append(block)
    return output

# S-box таблица
s_box = [
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
]

# Обратная S-box таблица
inv_s_box = [
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
]

# Константы раундов
r_con = [
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00,
    0x1b, 0x00, 0x00, 0x00,
    0x36, 0x00, 0x00, 0x00
]

def display(vector: List[int]) -> None:
    print(' '.join([str(x) for x in vector]))

def display_hex(vector: List[int]) -> None:
    print(' '.join([hex(x)[2:].zfill(2) for x in vector]))

# Циклический сдвиг влево на 1 байт
def rot_word(input_word: List[int]) -> List[int]:
    return input_word[1:] + input_word[:1]

# Замена байтов с помощью S-box
def sub_word(input_word: List[int]) -> List[int]:
    return [s_box[b] for b in input_word]

# Операция XOR между двумя массивами байтов
def xor_func(vec1: List[int], vec2: List[int]) -> List[int]:
    return [a ^ b for a, b in zip(vec1, vec2)]

# Расширение ключа
def key_expansion(key: List[int], nk: int = 4, nb: int = 4, nr: int = 10) -> List[List[int]]:
    keys = []
    
    # Инициализация первых Nk слов
    for j in range(nk):
        keys.append([key[4*j], key[4*j+1], key[4*j+2], key[4*j+3]])
    
    # Генерация остальных слов
    for i in range(nk, nb * (nr + 1)):
        temp = keys[i-1].copy()
        
        if i % nk == 0:
            temp = rot_word(temp)
            temp = sub_word(temp)
            temp = [temp[j] ^ r_con[i//nk * 4 + j] for j in range(4)]
        elif nk > 6 and i % nk == 4:
            temp = sub_word(temp)
            
        next_word = xor_func(keys[i - nk], temp)
        keys.append(next_word)
    
    return keys

# Соединение ключей в один массив для каждого раунда
def connect_keys_part(keys: List[List[int]]) -> List[List[int]]:
    output = []
    for pos in range(0, len(keys), 4):
        one_key = []
        for i in range(4):
            one_key.extend(keys[pos + i])
        output.append(one_key)
    return output

# Преобразование MixColumns для одного столбца
def mix_single_column(r: List[int]) -> List[int]:
    a = r.copy()
    b = [0] * 4
    
    # Умножение на 2 в поле GF(2^8)
    for c in range(4):
        h = (r[c] >> 7) & 1
        b[c] = (r[c] << 1) & 0xFF
        b[c] ^= 0x1B * h
    
    # Умножение столбца на матрицу
    r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]
    r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]
    r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]
    r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]
    
    return r

# Преобразование MixColumns для всего состояния
def mix_column(state: List[int]) -> List[int]:
    temp = [0] * 4
    for i in range(4):
        # Извлечение столбца
        for j in range(4):
            temp[j] = state[i + j*4]
        
        # Обработка столбца
        mix_single_column(temp)
        
        # Возврат обработанного столбца
        for j in range(4):
            state[i + j*4] = temp[j]
    
    return state

# Обратная замена байтов
def inv_sub_word(input_word: List[int]) -> List[int]:
    return [inv_s_box[b] for b in input_word]

# Циклический сдвиг вправо на 1 байт
def inv_rot_word(input_word: List[int]) -> List[int]:
    return input_word[-1:] + input_word[:-1]

# Умножение в поле GF(2^8)
def mul_bytes(a: int, b: int) -> int:
    p = 0
    for _ in range(8):
        if b & 1:
            p ^= a
        high_bit = a & 0x80
        a <<= 1
        if high_bit:
            a ^= 0x1B
        b >>= 1
    return p & 0xFF

# Обратное преобразование MixColumns
def inv_mix_columns(state: List[int]) -> List[int]:
    s_copy = [0] * 16
    for i in range(4):
        # Извлечение столбца
        column = [state[4*j + i] for j in range(4)]
        
        # Обратное преобразование столбца
        new_column = [
            mul_bytes(0x0E, column[0]) ^ mul_bytes(0x0B, column[1]) ^ 
            mul_bytes(0x0D, column[2]) ^ mul_bytes(0x09, column[3]),
            mul_bytes(0x09, column[0]) ^ mul_bytes(0x0E, column[1]) ^ 
            mul_bytes(0x0B, column[2]) ^ mul_bytes(0x0D, column[3]),
            mul_bytes(0x0D, column[0]) ^ mul_bytes(0x09, column[1]) ^ 
            mul_bytes(0x0E, column[2]) ^ mul_bytes(0x0B, column[3]),
            mul_bytes(0x0B, column[0]) ^ mul_bytes(0x0D, column[1]) ^ 
            mul_bytes(0x09, column[2]) ^ mul_bytes(0x0E, column[3])
        ]
        
        # Возврат обработанного столбца
        for j in range(4):
            state[4*j + i] = new_column[j]
    
    return state

# Шифрование AES
def encrypt(keys: List[List[int]], state: List[int]) -> List[int]:
    nk = 4
    nr = nk + 6
    
    # Начальный раунд
    state = xor_func(keys[0], state)
    
    # Основные раунды
    for round_num in range(1, nr):
        state = sub_word(state)
        state = rot_word(state)
        state = mix_column(state)
        state = xor_func(keys[round_num], state)
    
    # Финальный раунд
    state = sub_word(state)
    state = rot_word(state)
    state = xor_func(keys[nr], state)
    
    return state

# Дешифрование AES
def decrypt(keys: List[List[int]], state: List[int]) -> List[int]:
    nk = 4
    nr = nk + 6
    
    # Финальный раунд в обратном порядке
    state = xor_func(keys[nr], state)
    state = inv_rot_word(state)
    state = inv_sub_word(state)
    
    # Основные раунды в обратном порядке
    for round_num in range(nr-1, 0, -1):
        state = xor_func(keys[round_num], state)
        state = inv_mix_columns(state)
        state = inv_rot_word(state)
        state = inv_sub_word(state)
    
    # Начальный раунд
    state = xor_func(keys[0], state)
    
    return state

def main():
    sender_file = "sender.txt"
    
    # Ввод данных и запись в файл
    input_to_file(sender_file)
    
    # Чтение данных из файла
    input_str = file_to_str(sender_file)
    
    # Преобразование в массив байтов
    input_vec = str_to_vec(input_str)
    
    # Разбиение на блоки по 16 байт
    message = message_to_blocks(input_vec)
    
    # Ключ (16 байт)
    key = [ord(c) for c in ['1','2','3','4','5','6','7','8','9','0','a','b','c','d','e','f']]
    
    # Генерация раундовых ключей
    keys = key_expansion(key)
    keys = connect_keys_part(keys)
    
    print("Generated keys:")
    print("------------------------------------")
    for one_key in keys:
        display_hex(one_key)
    
    # Шифрование с режимом CFB
    encrypted_messages = [[0] * 16]  # IV
    
    print("------------------------------------")
    print("Message:")
    for state in message:
        display(state)
        
        # Шифрование блока
        encrypted_state = encrypt(keys, state.copy())
        
        # CFB режим: XOR с предыдущим зашифрованным блоком
        encrypted_block = xor_func(encrypted_messages[-1], encrypted_state)
        encrypted_messages.append(encrypted_block)
    
    print("------------------------------------")
    print("Encrypted message:")
    print("InVec - ", end="")
    for i in encrypted_messages:
        display_hex(i)
    
    # Дешифрование
    decrypted_messages = []
    
    for i in range(len(encrypted_messages)-1, 0, -1):
        # CFB режим: XOR с предыдущим зашифрованным блоком
        state = xor_func(encrypted_messages[i], encrypted_messages[i-1])
        
        # Дешифрование блока
        decrypted_state = decrypt(keys, state)
        decrypted_messages.append(decrypted_state)
    
    print("------------------------------------")
    print("Decrypted message:")
    for i in reversed(decrypted_messages):
        display(i)
    print("------------------------------------")

if __name__ == "__main__":
    main()