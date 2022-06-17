#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <map>
using namespace std;
class Function
{
private:
    static char OPEN_PAR;
    static char CLOSE_PAR;
    static char PLUS;
    static char MINUS;
    static char MULT;
    static char DIV;
    static char FACT;
    static char EXPO;
    static string SINE;
    static string COSINE;
    static string TANGENT;

    map<string, double> variables;

    bool checkParenthesis(string input);
    bool variableExists(string key, const map<string, double>& vars);
    bool isDigit(char c);
    bool isCharacter(char c);
    bool isOperator(char c);
    bool isInVector(const vector<string>& src, string s);
    bool isDecimal(string input);
    int factorial(int n);
    double degToRad(double x);
    double solveTrig(double input, int function);
    bool checkSintaxis(string input);
    string solvePlus(string input);
    string solveFactorial(string input);
    string solvePower(string input);
    string solveMultDiv(string input);
    string makeBasicOperation(string input, string number1_s, string number2_s, char op, int i);
    vector<string> getKeywords();

public:
    Function() {};

    void setVariable(string key, double val);
    string replaceVariables(string input);
    string calculate(string input);
};