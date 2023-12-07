#include "Table.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <map>

using namespace std;

char order[] = "abehcdfg";

char TableAdd[TAB_SIZE][TAB_SIZE];
char TableAddShift[TAB_SIZE][TAB_SIZE];
char TableMult[TAB_SIZE][TAB_SIZE];
char TableMultShift[TAB_SIZE][TAB_SIZE];

// ������� ��� ������ ������ �� ����� � �������
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

// ������� ��� ���������� �������� �� ����� 8
void PadOperand(string& operand, const char paddingChar, const int size) {
    while (operand.length() < size) {
        operand = paddingChar + operand;
    }
}

// ������� ��� �������� ������� ����� � �����
void RemoveLeadingZeros(string& number) {
    string num = number;
    bool sign = false;
    if (num[0] == '-') {
        sign = true;
        num = num.substr(1, num.length());
    }
    // ������� ������ ��������� �����
    size_t firstNonZero = num.find_first_not_of('a');

    // ���� ��� ����� ����, �� ��������� ����� "a"
    if (firstNonZero > 8) {
        num = "a";
    }
    
    
    else {
        // ������� ������� ����
        num = num.substr(firstNonZero);
    }
    number = num;
    if (sign) {
        number = '-' + number;
    }
}

// ������� ��� ������ �������� ���������
void SwapOperands(string& operand1, string& operand2) {
    string temp = operand1;
    operand1 = operand2;
    operand2 = temp;
}

bool isGreaterThan(string operand1,string operand2) {
    RemoveLeadingZeros(operand1);
    RemoveLeadingZeros(operand2);

    if (operand1.length() > operand2.length() or operand1 == operand2) {
        return true;
    }
    else if (operand1.length() < operand2.length()) {
        return false;
    }
    

    if (operand1[0] == '-') {
        operand1 = operand1.substr(1, operand1.length());
    }
    if (operand2[0] == '-') {
        operand2 = operand2.substr(1, operand2.length());
    }


    for (int i = 0; i < TAB_SIZE; i++) {
        if (operand1[i] != 'a' or operand2[i] != 'a') {
            for (int j = 0; j < TAB_SIZE; j++) {
                if (operand1[i] != operand2[i]) {
                    // ���� ������� ��������1 ����������� � ������ ������
                    if (operand1[i] == order[j]) {
                        return false;
                    }
                    // ���� ������� ��������2 ����������� � ������ ������
                    else if (operand2[i] == order[j]) {
                        return true;
                    }
                }

            }
        }
    }
}

// ������� ��� �������� ���������� �����
bool IsOperandValid(const string& operand) {
    if ((operand.length() > 8 and operand[0] != '-') or 
        operand.length() > 9) {
        return false;
    }
    
    // ������������ �������� �������� �� ����������
    for (char ch : operand) {
        // ���������� �����
        if (ch == '-') {
            continue;
        }

        if (!isalpha(ch) || (ch < 'a' || ch > 'h')) {
            return false; // ���� ������ ������������ ������, ������� false
        }
    }
    return true; // ��� ������� ���������
}

// ������� ��� �������� ���� �������� � ������ �������� �� ���������� �������
char AddCharacters(char a, char b, char& carry) {
    // �������� ������� �� ��������������� ������ ���������� �������
    char result = TableAdd[a - 'a'][b - 'a'];

    // ��������� ���������� �������
    result = AddCharacters(result, carry);

    // ��������� �������
    carry = max(TableAddShift[a - 'a'][b - 'a'], carry);

    return result;
}

char AddCharacters(char a, char& carry) {
    // �������� ������� � ������� �� ��������������� ������ ���������� �������
    char result = TableAdd[a - 'a'][carry - 'a'];
    carry = TableAddShift[a - 'a'][carry - 'a'];

    return result;
}


// ������� ��� �������� ���� ���������
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



// ������� ��� ��������� ���� �������� � ������ ���� �� ���������� �������
char SubtractCharacters(char a, char b, char& borrow) {
    // ������� ������, ��������������� ������� b � ������� ��������
    int rowIndex = b - 'a';

    // ������� �������, ��������������� ������� a � ������� ��������
    int colIndex = 0;
    for (; colIndex < TAB_SIZE; ++colIndex) {
        if (TableAdd[rowIndex][colIndex] == a) {
            break;
        }
    }

    // ��������� ��������� � ���������� ����
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

    // ��������� ��� ��� ���������� �������
    borrow = TableAddShift[rowIndex][colIndex];

    return result;
}

// ������� ��� ��������� ���� ���������
void SubtractOperands(string& operand1, string& operand2, string& result) {
    char borrow = 'a';  // ��������� ��� - ���������� �������
    bool sign = false;
    string tempResult;  // ��������� ��������� ����������

    int size = 0;
    
    if (isGreaterThan(operand2, operand1)){
        // ������ ����� ������� � ���������� �����
        sign = true;
        size = operand2.length();
        SwapOperands(operand1, operand2);
    }
    else {
        size = operand1.length();
    }
    PadOperand(operand2, 'a', size); // ��������� �������2 �������� ������ ��� ����� �����


    for (int i = size - 1; i >= 0; --i) {
        char diff = SubtractCharacters(operand1[i], operand2[i], borrow);
        tempResult = diff + tempResult;
    }

    // ��������� ���� ������, ���� ���� ���
    PadOperand(tempResult, 'a', TAB_SIZE);

    if ((borrow != 'a' or sign) and tempResult != "aaaaaaaa") {
        tempResult = '-' + tempResult;
    }

    result = tempResult;
}



// ������� ��� ��������� ���� �������� � ������ �������� �� ���������� �������
char MultiplyCharacters(char a, char b, char& carry) {
    // ��������� ��������� � ���������� ��������
    char result = TableMult[a - 'a'][b - 'a'];

    // ��������� ������� �������
    result = AddCharacters(result, carry);

    // ��������� ������� �������
    carry = AddCharacters(TableMultShift[a - 'a'][b - 'a'], carry);

    return result;
}


// ������� ��� ��������� ���� ���������
void MultiplyOperands(string& operand1, string& operand2, string& result) {
    // �������������� ��������� ������
    string tempResult;

    char carry = 'a';  // ��������� ������� ��� �������� �������
    
    if (operand1 == "a" or operand2 == "a") {
        result = "a";
        return;
    }

    
    // ���������
    for (int i = operand2.length() - 1; i >= 0; --i) {
        RemoveLeadingZeros(tempResult);

        // ����� ��� ������������� ��������
        string buffer(operand2.length() - 1 - i, 'a');

        // �������� ������� ������ operand2 �� ���� Operand1
        for (int j = operand1.length() - 1; j >= 0; --j) {
            char product = MultiplyCharacters(operand2[i], operand1[j], carry);

            // ��������� ��������� ��������� � ������
            buffer = product + buffer;
        }
        if (carry != 'a') {
            buffer = carry + buffer;
        }
        carry = 'a';

        // ��������� ����� � ���������� � ������ �������� ��������
        AddOperands(tempResult, buffer, tempResult);
    }

    PadOperand(tempResult, 'a', TAB_SIZE);
    result = tempResult;
}

void DivideOperands(string& operand1, string& operand2, string& result) {
    bool sign = false, flagMP = false, flagMM = false;
    // ���� ��������� ���� -a / b
    if (operand1[0] == '-' and operand2[0] != '-') {
        flagMP = true;
        // ������� �������
        operand1 = operand1.substr(1, operand1.length());
    }
    // ���� ��������� ���� a / -b
    if (operand2[0] == '-' and operand1[0] != '-') {
        sign = true;
        // ������� �������
        operand2 = operand2.substr(1, operand2.length());
    }
    // ���� ��������� ���� -a / (-b) = a * b
    if (operand1[0] == '-' and operand2[0] == '-') {
        flagMM = true;
        // �������� ��������
        operand1 = operand1.substr(1, operand1.length());
        operand2 = operand2.substr(1, operand2.length());
    }

    // �������� ������� 0 �� 0
    if (operand1 == "a" and operand2 == "a") {
        result = "[-gggggggg; gggggggg]";
        return;
    }

    // �������� ������� �� 0
    if (operand2 == "a") {
        result = "������ ���������.";
        return;
    }

    if (operand1 == "a" or isGreaterThan(operand2, operand1)) {
        result = "aaaaaaaa#";
        result += operand1;
        return;
    }


    if (operand2 == "b") {
        PadOperand(operand1, 'a', TAB_SIZE);
        if (flagMP) { // ���� �������1 �������������
            operand1 = "-" + operand1; // ���������� ����
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

string DoOperation(const string& operand1, const string& operand2,
    const string& oper) {
    string result, new_operand1 = operand1, new_operand2 = operand2;
    bool sign = false;
    if (oper == "+") {
        // ���� ��������� ���� -a + b = b - a
        if (operand1[0] == '-' and operand2[0] != '-') {
            // ������� �������
            new_operand1 = operand1.substr(1, operand1.length());
            SubtractOperands(new_operand2, new_operand1, result);
        }
        // ���� ��������� ���� a + (-b) = a - b
        else if (operand2[0] == '-' and operand1[0] != '-') {
            // ������� �������
            new_operand2 = operand2.substr(1, operand2.length());
            SubtractOperands(new_operand1, new_operand2, result);
        }
        // ���� ��������� ���� -a + (-b) = -(a + b)
        else if (operand1[0] == '-' and operand2[0] == '-') {
            sign = true; // ������������� ���� ����� ���������
            // �������� ��������
            new_operand1 = operand1.substr(1, operand1.length());
            new_operand2 = operand2.substr(1, operand2.length());
            AddOperands(new_operand1, new_operand2, result);
        }
        else {
            AddOperands(new_operand1, new_operand2, result);
        }

        if (sign) {
            result = "-" + result;
        }
    }
    else if (oper == "-") {
        // ���� ��������� ���� -a - b = -(a + b)
        if (operand1[0] == '-' and operand2[0] != '-') {
            // ������� �������
            new_operand1 = operand1.substr(1, operand1.length());
            AddOperands(new_operand1, new_operand2, result);
            result = '-' + result;
        }
        // ���� ��������� ���� a - (-b) = a + b
        else if (operand2[0] == '-' and operand1[0] != '-') {
            // ������� �������
            new_operand2 = operand2.substr(1, operand2.length());
            AddOperands(new_operand1, new_operand2, result);
        }
        // ���� ��������� ���� -a - (-b) = b - a
        else if (operand1[0] == '-' and operand2[0] == '-') {
            // �������� ��������
            new_operand1 = operand1.substr(1, operand1.length());
            new_operand2 = operand2.substr(1, operand2.length());
            SubtractOperands(new_operand2, new_operand1, result);
        }
        else {
            SubtractOperands(new_operand1, new_operand2, result);
        }

    }
    else if (oper == "*") {
        // ���� ��������� ���� -a * b
        if (operand1[0] == '-' and operand2[0] != '-') {
            sign = true;
            // ������� �������
            new_operand1 = operand1.substr(1, operand1.length());
        }
        // ���� ��������� ���� a * -b
        else if (operand2[0] == '-' and operand1[0] != '-') {
            sign = true;
            // ������� �������
            new_operand2 = operand2.substr(1, operand2.length());
        }
        // ���� ��������� ���� -a * (-b) = a * b
        else if (operand1[0] == '-' and operand2[0] == '-') {
            // �������� ��������
            new_operand1 = operand1.substr(1, operand1.length());
            new_operand2 = operand2.substr(1, operand2.length());
        }
        MultiplyOperands(new_operand1, new_operand2, result);
        if (sign and result != "a") {
            result = "-" + result;
        }
    }
    else if (oper == "/") {
        DivideOperands(new_operand1, new_operand2, result);
    }
    return result;
}


bool ProcessExpression(const string& input) {
    istringstream iss(input);
    string operand1, operand2, oper;
    string result;

    if (iss >> operand1 >> oper >> operand2) {
        // �������� ���������� ���������
        if (IsOperandValid(operand1) && IsOperandValid(operand2)) {
            // ������� ���������� ����
            RemoveLeadingZeros(operand1);
            RemoveLeadingZeros(operand2);
            // ����� ��������������� �������� � ����������� �� ���������

            bool validOperator = (oper == "+") or (oper == "-") or
                (oper == "*") or (oper == "/");
            if (validOperator) {
                result = DoOperation(operand1, operand2, oper);

                // ��������� ���������� �������
                if (result.length() > 9 and (result[8] == '#' or
                    result[9] == '#')) {
                    // �������
                    string quotient = result.substr(0, result.find('#'));
                    // �������
                    string remainder = result.substr(result.find('#')
                        + 1, result.length());
                    RemoveLeadingZeros(quotient);
                    RemoveLeadingZeros(remainder);
                    cout << "�������: " << quotient << " �������: "
                        << remainder << endl;
                    return true;
                }

                if (result == "������ ���������.") {
                    cout << result << endl;
                    return true;
                }

                // ��������� ������������
                if ((((result.length() > 8 and result[0] != '-') or
                    (result[0] == '-' and result.length() > 9)) and
                    result[0] != '[')) {
                    cout << "������������." << endl;
                    return false;
                }

                RemoveLeadingZeros(result);
                cout << "���������: " << result << endl;
                return true;
            }
        }
        else {
            cout << "������: ������� �������� ������������ ������� ��� "
                << "��������� 8 ��������." << endl;
        }
    }
    else {
        cout << "������ ����� ���������." << endl;
    }
    return false;
}