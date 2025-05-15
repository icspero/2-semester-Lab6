#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Запись в файл
void inputToFile(string &senderFile){
    ofstream out; // поток записи в файл
    out.open(senderFile);
    if (out.is_open()){ // если файл существует и открывается
        string str;
        while (getline(cin, str)){
            if (str == "EOF" || str.empty()) break; //пока не введём в консоль EOF или пустая строка
            out << str << '\n';
        }
    }
    out.close();
}

// Запись из файла в строку
string fileToStr(string &str, string &senderFile){
    ifstream in;
    in.open(senderFile);
    if(in.is_open()){
        char ch;
        while(in.get(ch)){
            str.push_back(ch);
        }
    }
    in.close();
    return str;
}

// Преобразование в массив байтов
vector<unsigned char> strToVec(string &input){
    vector<unsigned char> output;
    output.reserve(input.size());
    for (auto i : input){
        output.push_back(static_cast<unsigned char>(i));
    }
    return output;
}

// Разбиение сообщения на блоки по 16 байт
vector<vector<unsigned char>> messageToBlocks(const vector<unsigned char> &message){
    vector<vector<unsigned char>> output;
    vector<unsigned char> tmp;
    for (int i = 0; 16 * i < message.size(); i++){
        for (int j = 0; j < 16; j++){
            if (16*i + j < message.size()){ // если данные есть, берем байт из сообщения
                tmp.push_back(message[j + 16 * i]);
            }
            else{
                tmp.push_back(' '); // если данные кончились, дополняем ' '
            }
        }
        output.push_back(tmp);
        tmp.clear();
    }
    return output; // возвращает вектор блоков
}

// Каждый байт входного блока заменяется на другой байт с помощью таблицы(256 значений)
const vector<unsigned char> sBox{
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
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

// Используется при дешифровании, обратна sBox
const vector<unsigned char> invSBox{
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
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

// Константы раундов, используемые в процессе расширения ключей
const vector<unsigned char> Rcon = {
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
};

// Вывод как значений
void display(const vector<unsigned char> &vector1){
    for (const auto &i : vector1){
        cout << i << " ";
    }
    cout << endl;
}

// Вывод в 16-ом виде
void displayHEX(const vector<unsigned char> &vector1){
    for (const auto &i : vector1){
        cout << hex << static_cast<int>(i) << " ";
    }
    cout << endl;
}

// Сдвигает байты на 1 влево
vector<unsigned char> rotWord(vector<unsigned char> &input){
    unsigned char tmpCell = input[0];
    input[0] = input[1];
    input[1] = input[2];
    input[2] = input[3];
    input[3] = tmpCell;
    return input;
}

// Замена на i-тый из sBox
vector<unsigned char> subWord(vector<unsigned char> &input){
    vector<unsigned char> output;
    output.reserve(input.size());
    for (auto i : input) {
        output.push_back(sBox[i]);
    }
    return output;
}

// Побитово складывает (XOR) два вектора байт одинаковой длины.
vector<unsigned char> xorFunc(const vector<unsigned char> &vec1,const vector<unsigned char> &vec2){
    vector<unsigned char> outVec;
    outVec.reserve(vec1.size());
    for (auto k = 0; k < vec1.size(); k++){
        outVec.push_back(vec1[k] ^ vec2[k]);
    }
    return outVec;
}

// Функция выполняет расширение ключа(нужно расширить до набора раундовых ключей)(разные ключи на каждом раунде)
vector<vector<unsigned char>> keyExpansion(vector<unsigned char> &key, vector<vector<unsigned char>> &keys, int Nk = 4, int Nb = 4, int Nr = 10){
    vector<unsigned char> tmp;
    for (int j = 0; j < Nk; j++){
        keys.push_back({key[4 * j], key[4 * j + 1], key[4 * j + 2], key[4 * j + 3]}); // исходный ключ разбивает на Nk слов по 4 байта
    }
    keys.reserve(Nb * (Nr + 1));
    for (auto i = Nk; i < Nb * (Nr + 1); i++){
        tmp = keys[i - 1]; // каждый новый ключевой блок строится из предыдущего
        if (i % Nk == 0){
            tmp = rotWord(tmp); // циклический сдвиг на 1 байт влево
            tmp = subWord(tmp); // замена байтов через sBox
            for (int j = 0; j < 4; ++j){
                tmp[j] = tmp[j] ^ Rcon[i/Nk]; // XOR с константами Rcon
            }
        } else if (Nk > 6 && i % Nk == 4) {
            tmp = subWord(tmp);
        }
        keys.push_back(xorFunc(keys[i - Nk], tmp)); // получаем следующее слово ключа с помощью XOR
    }
    return keys; // keys будет содержать Nb * (Nr + 1) блоков по 4 байта, т.е. все раундовые ключи для AES
}

// Собирает раундовые ключи из построчного формата(4-векторные колонки) в один вектор на каждый раунд
vector<vector<unsigned char>> connectKeysPart (const vector<vector<unsigned char>> &keys){
    vector<vector<unsigned char>> output;
    output.reserve(keys.size()/4);
    vector<unsigned char> oneKey;
    for (int pos = 0; pos < keys.size(); pos += 4) { // обработка по 4 колонки за раз, каждые 4 - это 1 полный ключ для раунда
        for(int i = 0; i < 4; i++){
            oneKey.push_back(keys[pos + i][0]);
            oneKey.push_back(keys[pos + i][1]);
            oneKey.push_back(keys[pos + i][2]);
            oneKey.push_back(keys[pos + i][3]);
        }
        output.push_back(oneKey);
        oneKey.clear();
    }
    return output; // возвращает массив, в котором каждый элемент - это один 16-байтовый ключ для соответствующего раунда
}

// Перемешивает байты в одном столбце
void mixSingleColumn(vector<unsigned char> &r) { // принимает один столбец блока AES
    vector<unsigned char> a = r;
    vector<unsigned char> b(4);
    int c, h;

    // умножает каждый байт на 2 в GF(2^8)
    for (c = 0; c < 4; c++){
        h = (unsigned char)((signed char)r[c] >> 7);
        b[c] = r[c] << 1; // умножение на 2
        b[c] ^= 0x1B & h; // приводим к допустимому диапазону поля(редукция)
    }
    // собираем новую колонку(умножение столбца на фикс. матрицу)
    r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
    r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
    r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3];
    r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];

}

// Перемешиваем байты в столбцах
vector<unsigned char> mixColumn(vector<unsigned char> &input){
    vector<unsigned char> temp(4);
    for(int i = 0; i < 4; i++){ //цикл для прохода по колонам
        for (int j = 0; j < 4; j++){//для взятия каждого в колонне
            temp[j] = input[i + j * 4];
        }
        mixSingleColumn(temp);
        for (int j = 0; j < 4; j++){//возврат перемешанного
            input[i + j * 4] = temp[j];
        }
    }
    return input;
}

// Выполняет обратное преобразование замены байтов
vector<unsigned char> invSubWord(vector<unsigned char> &input){
    vector<unsigned char> output;
    output.reserve(input.size());
    for (auto i : input) {
        output.push_back(invSBox[i]);
    }
    return output;
}

// Поворачивает байты вправо(обратна функции rotWord)
vector<unsigned char> invRotWord(vector<unsigned char> &input){
    unsigned char tmpCell = input[3];
    input[3] = input[2];
    input[2] = input[1];
    input[1] = input[0];
    input[0] = tmpCell;
    return input;
}

// Умножение двух байт a и b в поле Галуа GF(2^8) с редукцией по модулю 0x1B(необходимо для операций MixColumns, 
// где нужно перемножать байты с определёнными коэффициентами)
unsigned char mulBytes(unsigned char a, unsigned char b){
    unsigned char p = 0, i, modulo = 0x1B;
    unsigned char high_bit_mask = 0x80;
    unsigned char high_bit;
    for (i = 0; i < 8; i++)
    {
        if (b & 1) p ^= a;
        high_bit = a & high_bit_mask;
        a <<= 1;
        if (high_bit) a ^= modulo;
        b >>= 1;
    }
    return p;
}

// Реализует обратное преобразование MixColumns
vector<unsigned char> invMixColumns(vector<unsigned char>& state){
    vector<unsigned char> sCopy;
    vector<unsigned char> singleColumn;
    for(int i = 0; i < 4; i++){
        singleColumn.clear();
        for(int k = 0; k < 4; k++){
            singleColumn.push_back(state[4*k+i]);
        }
        sCopy = singleColumn;
        singleColumn[0] = mulBytes(0xE, sCopy[0]) ^ mulBytes(0xB, sCopy[1]) ^ mulBytes(0xD, sCopy[2]) ^ mulBytes(0x9, sCopy[3]);
        singleColumn[1] = mulBytes(0x9, sCopy[0]) ^ mulBytes(0xE, sCopy[1]) ^ mulBytes(0xB, sCopy[2]) ^ mulBytes(0xD, sCopy[3]);
        singleColumn[2] = mulBytes(0xD, sCopy[0]) ^ mulBytes(0x9, sCopy[1]) ^ mulBytes(0xE, sCopy[2]) ^ mulBytes(0xB, sCopy[3]);
        singleColumn[3] = mulBytes(0xB, sCopy[0]) ^ mulBytes(0xD, sCopy[1]) ^ mulBytes(0x9, sCopy[2]) ^ mulBytes(0xE, sCopy[3]);
        for(int k = 0; k < 4; k++){
            state[4*k+i] = singleColumn[k];
        }

    }
    return state;
}

// Шифрование 
vector<unsigned char> encrypt(vector<vector<unsigned char>> &keys, vector<unsigned char> &state){ // принимает вектор раундовых ключей и текущий блок данных
    const int Nk = 4;
    const int Nr = Nk + 6;
    state = xorFunc(keys[0], state);
    for(int round = 1; round <= Nr - 1; round++){
        state = subWord(state);
        state = rotWord(state);
        state = mixColumn(state);
        state = xorFunc(keys[round], state);
    }
    state = subWord(state);
    state = rotWord(state);
    state = xorFunc(keys[10], state);
    return state; // возвращает зашифрованный блок
}

// Дешифрование
vector<unsigned char> decrypt(vector<vector<unsigned char>> &keys, vector<unsigned char> &state){ // принимает раундовые ключи и зашифрованный блок
    const int Nk = 4;
    const int Nr = Nk + 6;
    state = xorFunc(keys[10], state);
    state = invRotWord(state);
    state = invSubWord(state);
    for(int round = Nr - 1; round >= 1; round--){
        state = xorFunc(keys[round], state);
        state = invMixColumns(state);
        state = invRotWord(state);
        state = invSubWord(state);
    }
    state = xorFunc(keys[0], state);
    return state; // возвращает расшифрованный блок
}

int main() {
    string senderFile = "sender.txt";
    inputToFile(senderFile);
    string input;
    input = fileToStr(input, senderFile);
    vector<unsigned char> inputVec;
    inputVec = strToVec(input);
    vector<vector<unsigned char>> message;
    message = messageToBlocks(inputVec);
    vector<unsigned char> key = {'1','2','3','4',
                                 '5','6','7','8',
                                 '9','0','a','b',
                                 'c','d','e','f'}; // 16 байт ключа
    vector<vector<unsigned char>> keys;
    keys = keyExpansion(key, keys); // генерация ключей для будущих кругов шифрования.
    keys = connectKeysPart(keys); // соединение ключа из 4 частей по 4 байта в 1 из 16 байт

    cout << "Generated keys: " << endl << "------------------------------------" << endl;
    for(const auto &oneKey : keys){
        displayHEX(oneKey);
    }
    vector<vector<unsigned char>> encryptedMessages;
    encryptedMessages.push_back({'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'});//IV
    cout << "------------------------------------" << endl;
    cout << "Message: " << endl;
    for(auto state : message){
        display(state);
        state = encrypt(keys, state); // шифруем
        encryptedMessages.push_back(xorFunc(encryptedMessages[encryptedMessages.size()-1], state));//CFB часть AES
    }
    cout << "------------------------------------" << endl;
    cout << "Encrypted message: " << endl << "InVec - ";
    for (const auto &i : encryptedMessages){
        displayHEX(i);
    }
    vector<vector<unsigned char>> decryptedMessages;
    vector<unsigned char> state; // так, потому, что выводить нужно в обратном порядке
    for (auto i = encryptedMessages.size() - 1; i > 0; i--){
        state = xorFunc(encryptedMessages[i],encryptedMessages[i - 1]); // CFB
        decryptedMessages.push_back(decrypt(keys, state)); // дешифруем
    }
    cout << "------------------------------------" << endl;
    cout << "Decrypted message: " << endl;
    for (int i = static_cast<int>(decryptedMessages.size()) - 1; i >= 0; i--){
        display(decryptedMessages[i]);
    }
    cout << "------------------------------------" << endl;
}
