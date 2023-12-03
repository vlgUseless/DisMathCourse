#include "Table.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

using namespace std;

string DoOperation(const string& operand1, const string& operand2,
    const string& oper) {
    string result, new_operand1 = operand1, new_operand2 = operand2;
    bool sign = false;
    if (oper == "+") {
        // Если выражение вида -a + b = b - a
        if (operand1[0] == '-' and operand2[0] != '-') {
            // Обрезам операнд
            new_operand1 = operand1.substr(1, operand1.length());
            SubtractOperands(new_operand2, new_operand1, result);
        }
        // Если выражение вида a + (-b) = a - b
        else if (operand2[0] == '-' and operand1[0] != '-') {
            // Обрезам операнд
            new_operand2 = operand2.substr(1, operand2.length());
            SubtractOperands(new_operand1, new_operand2, result);
        }
        // Если выражение вида -a + (-b) = -(a + b)
        else if (operand1[0] == '-' and operand2[0] == '-') {
            sign = true; // Устанавливаем знак всего выражения
            // Обрезаем операнды
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
        // Если выражение вида -a - b = -(a + b)
        if (operand1[0] == '-' and operand2[0] != '-') {
            // Обрезам операнд
            new_operand1 = operand1.substr(1, operand1.length());
            AddOperands(new_operand1, new_operand2, result);
            result = '-' + result;
        }
        // Если выражение вида a - (-b) = a + b
        else if (operand2[0] == '-' and operand1[0] != '-') {
            // Обрезам операнд
            new_operand2 = operand2.substr(1, operand2.length());
            AddOperands(new_operand1, new_operand2, result);
        }
        // Если выражение вида -a - (-b) = b - a
        else if (operand1[0] == '-' and operand2[0] == '-') {
            // Обрезаем операнды
            new_operand1 = operand1.substr(1, operand1.length());
            new_operand2 = operand2.substr(1, operand2.length());
            SubtractOperands(new_operand2, new_operand1, result);
        }
        else {
            SubtractOperands(new_operand1, new_operand2, result);
        }
        
    }
    else if (oper == "*") {
        // Если выражение вида -a * b
        if (operand1[0] == '-' and operand2[0] != '-') {
            sign = true;
            // Обрезам операнд
            new_operand1 = operand1.substr(1, operand1.length());
        }
        // Если выражение вида a * -b
        else if (operand2[0] == '-' and operand1[0] != '-') {
                sign = true;
            // Обрезам операнд
            new_operand2 = operand2.substr(1, operand2.length());
        }
        // Если выражение вида -a * (-b) = a * b
        else if (operand1[0] == '-' and operand2[0] == '-') {
            // Обрезаем операнды
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
        // Проверка валидности операндов
        if (IsOperandValid(operand1) && IsOperandValid(operand2)) {
            // Удаляем незначащие нули
            RemoveLeadingZeros(operand1);
            RemoveLeadingZeros(operand2);
            // Выбор соответствующей операции в зависимости от оператора

            bool validOperator = (oper == "+") or (oper == "-") or
                (oper == "*") or (oper == "/");
            if (validOperator) {
                result = DoOperation(operand1, operand2, oper);
                
                // Обработка результата деления
                if (result.length() > 9 and (result[8] == '#' or
                    result[9] == '#')) {
                    // Частное
                    string quotient = result.substr(0, result.find('#'));
                    // Остаток
                    string remainder = result.substr(result.find('#')
                        + 1, result.length());
                    RemoveLeadingZeros(quotient);
                    RemoveLeadingZeros(remainder);
                    cout << "Частное: " << quotient << " Остаток: " 
                        << remainder << endl;
                    return true;
                }

                if (result == "Пустое множество.") {
                    cout << result << endl;
                    return true;
                }

                // Обработка переполнения
                if ((((result.length() > 8 and result[0] != '-') or
                    (result[0] == '-' and result.length() > 9)) and
                    result[0] != '[')) {
                    cout << "Переполнение." << endl;
                    return false;
                }   
                
                RemoveLeadingZeros(result);
                cout << "Результат: " << result << endl;
                return true;
            }
        }
        else {
            cout << "Ошибка: операнд содержит недопустимые символы или "
                << "превышает 8 символов." << endl;
        }
    }
    else {
        cout << "Ошибка ввода выражения." << endl;
    }
    return false;
}




int main()
{
    setlocale(LC_ALL, "Russian");

	//Считали все таблицы из соответствующих файлов:
	ReadingTable(TableAdd, "TableAdd.txt");
	ReadingTable(TableAddShift, "TableAddShift.txt");
	ReadingTable(TableMult, "TableMult.txt");
	ReadingTable(TableMultShift, "TableMultShift.txt");
	

    while (true) {
        bool validExpression = false;

        while (!validExpression) {
            string input;
            cout << "Введите выражение в формате 'операнд1 оператор операнд2': ";
            getline(cin, input);

            // Если ProcessExpression завершилась успешно, устанавливаем флаг, что выражение введено корректно
            validExpression = ProcessExpression(input);
        }
    }
}
