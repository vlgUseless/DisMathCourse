#include "Table.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <map>

using namespace std;

char order[] = "abehcdfg";
map<char, int> symbols{ {'a', 0},{'b', 1},{'e', 2},{'h', 3},{'c', 4},{'d', 5},{'f', 6}, {'g', 7}};
map<char, int> symbolsReverse{ {0, 'a'},{1, 'b'},{2, 'e'},{3, 'h'},{4, 'c'},{5, 'd'},{6, 'f'}, {7, 'g'}};

char TableAdd[TAB_SIZE][TAB_SIZE];
char TableAddShift[TAB_SIZE][TAB_SIZE];
char TableMult[TAB_SIZE][TAB_SIZE];
char TableMultShift[TAB_SIZE][TAB_SIZE];

// Функция для чтения данных из файла в таблицу
void ReadingTable(char Table[TAB_SIZE][TAB_SIZE], string filename) {
    ifstream in(filename);

    int i = 0, j = 0;
    string line;

    if (in.is_open()) {
        while (getline(in, line) && i < TAB_SIZE) {
            istringstream iss(line);
            char elem;

            while (iss >> elem && j < TAB_SIZE) {
                Table[i][j] = elem;
                j++;
            }

            j = 0;
            i++;
        }
    }
    else {
        cout << "Unable to open file";
    }

    in.close();
}

// Функция для дополнения операнда до длины 8
void PadOperand(string& operand, const char paddingChar, const int size) {
    while (operand.length() < size) {
        operand = paddingChar + operand;
    }
}

// Функция для удаления ведущих нулей в числе
void RemoveLeadingZeros(string& number) {
    string num = number;
    bool sign = false;
    if (num[0] == '-') {
        sign = true;
        num = num.substr(1, num.length());
    }
    // Находим первую ненулевую цифру
    size_t firstNonZero = num.find_first_not_of('a');

    // Если все цифры нули, то результат будет "a"
    if (firstNonZero > 8) {
        num = "a";
    }
    
    
    else {
        // Удаляем ведущие нули
        num = num.substr(firstNonZero);
    }
    number = num;
    if (sign) {
        number = '-' + number;
    }
}

// Функция для обмена значений операндов
void SwapOperands(string& operand1, string& operand2) {
    string temp = operand1;
    operand1 = operand2;
    operand2 = temp;
}

bool isGreaterThan(string operand1,string operand2) {
    RemoveLeadingZeros(operand1);
    RemoveLeadingZeros(operand2);

    int value1 = 0, value2 = 0;
    string op1 = operand1;
    string op2 = operand2;

    if (operand1.length() > operand2.length()) {
        return true;
    }
    

    if (op1[0] == '-') {
        op1 = op1.substr(1, op1.length());
    }
    if (op2[0] == '-') {
        op2 = op2.substr(1, op2.length());
    }
    for (char c : operand1) {
        value1 = value1 * 8 + symbols[c];
    }

    for (char c : operand2) {
        value2 = value2 * 8 + symbols[c];
    }

    return value1 > value2;
}

// Функция для проверки валидности ввода
bool IsOperandValid(const string& operand) {
    if ((operand.length() > 8 and operand[0] != '-') or 
        operand.length() > 9) {
        return false;
    }
    
    // Обрабатываем элементы операнда на валидность
    for (char ch : operand) {
        // Пропускаем минус
        if (ch == '-') {
            continue;
        }

        if (!isalpha(ch) || (ch < 'a' || ch > 'h')) {
            return false; // Если найден недопустимый символ, вернуть false
        }
    }
    return true; // Все символы допустимы
}

// Функция для сложения двух символов с учетом переноса из глобальной таблицы
char AddCharacters(char a, char b, char& carry) {
    // Получаем остаток из соответствующей ячейки глобальной таблицы
    char result = TableAdd[a - 'a'][b - 'a'];

    // Учитываем предыдущий перенос
    result = AddCharacters(result, carry);

    // Обновляем перенос
    carry = max(TableAddShift[a - 'a'][b - 'a'], carry);

    return result;
}

char AddCharacters(char a, char& carry) {
    // Получаем остаток и перенос из соответствующей ячейки глобальной таблицы
    char result = TableAdd[a - 'a'][carry - 'a'];
    carry = TableAddShift[a - 'a'][carry - 'a'];

    return result;
}


// Функция для сложения двух операндов
void AddOperands(string& operand1, string& operand2, string& result) {
    char carry = 'a';
    string tempResult;

    int size = 0;
    if (operand1.length() < operand2.length()) {
        size = operand2.length();
        PadOperand(operand1, 'a', size);
    }
    else {
        size = operand1.length();
        PadOperand(operand2, 'a', size);
    }

    for (int i = size - 1; i >= 0; --i) {
        char sum = AddCharacters(operand1[i], operand2[i], carry);
        tempResult = sum + tempResult;
    }

    if (carry != 'a') {
        tempResult = carry + tempResult;
    }

    PadOperand(tempResult, 'a', TAB_SIZE);
    
    result = tempResult;
}



// Функция для вычитания двух символов с учетом заёма из глобальной таблицы
char SubtractCharacters(char a, char b, char& borrow) {
    // Находим строку, соответствующую символу b в таблице сложения
    int rowIndex = b - 'a';

    // Находим столбец, соответствующий символу a в таблице сложения
    int colIndex = 0;
    for (; colIndex < TAB_SIZE; ++colIndex) {
        if (TableAdd[rowIndex][colIndex] == a) {
            break;
        }
    }

    // Результат вычитания и обновление заёма
    char result = colIndex + 'a';
    if (borrow != 'a') {
        if (result != 'a') {
            char newborrow = 'a';
            result = SubtractCharacters(result, borrow, newborrow);
        }
        else {
            return 'g';
        }
    }

    // Обновляем заём для следующего разряда
    borrow = TableAddShift[rowIndex][colIndex];

    return result;
}

// Функция для вычитания двух операндов
void SubtractOperands(string& operand1, string& operand2, string& result) {
    char borrow = 'a';  // начальный заём - аддитивная единица
    bool sign = false;
    string tempResult;  // временное хранилище результата

    int size = 0;
    
    if (isGreaterThan(operand2, operand1)){
        // Меняем числа местами и дописываем минус
        sign = true;
        size = operand2.length();
        SwapOperands(operand1, operand2);
    }
    else {
        size = operand1.length();
    }
    PadOperand(operand2, 'a', size); // Дополняем операнд2 ведущими нулями без учёта знака


    for (int i = size - 1; i >= 0; --i) {
        char diff = SubtractCharacters(operand1[i], operand2[i], borrow);
        tempResult = diff + tempResult;
    }

    // Добавляем знак минуса, если есть заём
    PadOperand(tempResult, 'a', TAB_SIZE);

    if ((borrow != 'a' or sign) and tempResult != "aaaaaaaa") {
        tempResult = '-' + tempResult;
    }

    result = tempResult;
}



// Функция для умножения двух символов с учетом переноса из глобальной таблицы
char MultiplyCharacters(char a, char b, char& carry) {
    // Результат умножения и обновление переноса
    char result = TableMult[a - 'a'][b - 'a'];

    // Добавляем текущий перенос
    result = AddCharacters(result, carry);

    // Обновляем текущий перенос
    carry = AddCharacters(TableMultShift[a - 'a'][b - 'a'], carry);

    return result;
}


// Функция для умножения двух операндов
void MultiplyOperands(string& operand1, string& operand2, string& result) {
    // Инициализируем результат нулями
    string tempResult;

    char carry = 'a';  // Начальный перенос для текущего разряда
    
    if (operand1 == "a" or operand2 == "a") {
        result = "a";
        return;
    }

    
    // Умножение
    for (int i = operand2.length() - 1; i >= 0; --i) {
        RemoveLeadingZeros(tempResult);

        // Буфер для промежуточных значений
        string buffer(operand2.length() - 1 - i, 'a');

        // Умножаем текущий разряд operand2 на весь Operand1
        for (int j = operand1.length() - 1; j >= 0; --j) {
            char product = MultiplyCharacters(operand2[i], operand1[j], carry);

            // Добавляем результат умножения к буферу
            buffer = product + buffer;
        }
        if (carry != 'a') {
            buffer = carry + buffer;
        }
        carry = 'a';

        // Добавляем буфер к результату с учетом текущего переноса
        AddOperands(tempResult, buffer, tempResult);
    }

    PadOperand(tempResult, 'a', TAB_SIZE);
    result = tempResult;
}

void DivideOperands(string& operand1, string& operand2, string& result) {
    bool sign = false, flagMP = false, flagMM = false;
    // Если выражение вида -a / b
    if (operand1[0] == '-' and operand2[0] != '-') {
        flagMP = true;
        // Обрезам операнд
        operand1 = operand1.substr(1, operand1.length());
    }
    // Если выражение вида a / -b
    if (operand2[0] == '-' and operand1[0] != '-') {
        sign = true;
        // Обрезам операнд
        operand2 = operand2.substr(1, operand2.length());
    }
    // Если выражение вида -a / (-b) = a * b
    if (operand1[0] == '-' and operand2[0] == '-') {
        flagMM = true;
        // Обрезаем операнды
        operand1 = operand1.substr(1, operand1.length());
        operand2 = operand2.substr(1, operand2.length());
    }

    // Проверка деления 0 на 0
    if (operand1 == "a" and operand2 == "a") {
        result = "[-gggggggg; gggggggg]";
        return;
    }

    // Проверка деления на 0
    if (operand2 == "a") {
        result = "Пустое множество.";
        return;
    }

    if (operand1 == "a" or isGreaterThan(operand2, operand1)) {
        result = "aaaaaaaa#";
        result += operand1;
        return;
    }


    if (operand2 == "b") {
        PadOperand(operand1, 'a', TAB_SIZE);
        if (flagMP) { // Если операнд1 отрицательный
            operand1 = "-" + operand1; // Возвращаем знак
        }
        result = operand1 + "#aaaaaaaa";
        if (sign) {
            result = "-" + result;
        }
        return;
    }

    string divisible = operand1;
    string add = "b";
    string add8 = "baaaaaaa";
    string add6 = "aabaaaaa";
    string add4 = "aaaabaaa";
    string add3 = "aaaaabaa";
    
    string res1, res2, res3, res4;
    MultiplyOperands(operand2, add8, res1);
    MultiplyOperands(operand2, add6, res2);
    MultiplyOperands(operand2, add4, res3);
    MultiplyOperands(operand2, add3, res4);
    if (res1.length() > 8) {
        res1 = "";
    }
    if (res2.length() > 8) {
        res2 = "";
    }
    if (res3.length() > 8) {
        res3 = "";
    }
    if (res4.length() > 8) {
        res4 = "";
    }

    while (true) {
        if (isGreaterThan(operand2, divisible)) {
            if (flagMP or flagMM) {
                if (divisible != "aaaaaaaa") {
                    AddOperands(result, add, result); // + 1
                    string approx;
                    RemoveLeadingZeros(result);
                    RemoveLeadingZeros(operand2);
                    MultiplyOperands(result, operand2, approx);
                    RemoveLeadingZeros(approx);
                    SubtractOperands(approx, operand1, divisible);
                }
                PadOperand(result, 'a', TAB_SIZE);
                if (flagMP) {
                    result = "-" + result;
                }
            }
            if (sign) {
                result = "-" + result;
            }
            result += "#" + divisible;
            return;
        }
        else if (res1 != "" and isGreaterThan(divisible, res1))
        {
            RemoveLeadingZeros(res1);
            SubtractOperands(divisible, res1, divisible);
            AddOperands(result, add8, result);
        }
        else if (res2 != "" and isGreaterThan(divisible, res2))
        {
            RemoveLeadingZeros(res2);
            SubtractOperands(divisible, res2, divisible);
            AddOperands(result, add6, result);
        }
        else if (res3 != "" and isGreaterThan(divisible, res3))
        {
            RemoveLeadingZeros(res3);
            SubtractOperands(divisible, res3, divisible);
            AddOperands(result, add4, result);
        }
        else if (res4 != "" and isGreaterThan(divisible, res4))
        {
            RemoveLeadingZeros(res4);
            SubtractOperands(divisible, res4, divisible);
            AddOperands(result, add3, result);
        }
        else {
            SubtractOperands(divisible, operand2, divisible);
            AddOperands(result, add, result);
        }
    }
}