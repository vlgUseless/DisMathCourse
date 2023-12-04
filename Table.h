#pragma once
#include <string>

#define TAB_SIZE 8
extern char TableAdd[TAB_SIZE][TAB_SIZE];
extern char TableAddShift[TAB_SIZE][TAB_SIZE];
extern char TableMult[TAB_SIZE][TAB_SIZE];
extern char TableMultShift[TAB_SIZE][TAB_SIZE];

using std::string;

void ReadingTable(char Table[TAB_SIZE][TAB_SIZE], std::string filename);
void PadOperand(string& operand, const char paddingChar, const int size);
void RemoveLeadingZeros(string& number);
bool isGreaterThan(string operand1, string operand2);
bool IsOperandValid(const std::string& operand);
void SwapOperands(string& operand1, string& operand2);

char AddCharacters(char a, char b, char& carry);
char AddCharacters(char a, char& carry);
void AddOperands(string& operand1, string& operand2, string& result);


char SubtractCharacters(char a, char b, char& borrow);
void SubtractOperands(string& operand1, string& operand2, string& result);


char MultiplyCharacters(char a, char b, char& carry);
void MultiplyOperands(string& operand1, string& operand2, string& result);

void DivideOperands(string& operand1, string& operand2, string& result);

