#include "Table.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

using namespace std;


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
