#include "functions.h"

using namespace std;

char Function::OPEN_PAR = '(';
char Function::CLOSE_PAR = ')';
char Function::PLUS = '+';
char Function::MINUS = '-';
char Function::MULT = '*';
char Function::DIV = '/';
char Function::FACT = '!';
char Function::EXPO = '^';
string Function::SINE = "sin";
string Function::COSINE = "cos";
string Function::TANGENT = "tan";

string Function::calculate(string input)
{ 
    //|| !checkSintaxis(input)
    if (!checkParenthesis(input))
        return "Error";

    vector<char> p_stack;
    vector<int> p_index;

    // Solve parenthesis first
    for (int i = 0; i < input.size(); i++)
    {
        char c = input[i];

        if (c == OPEN_PAR)
        {
            p_stack.push_back(c);
            p_index.push_back(i);
        }
        if (c == CLOSE_PAR)
        {
            int j = p_index.back();
            string subInput = input.substr(j + 1, i - j - 1);
            string subInputRes = calculate(subInput);

            bool isTrig = false;
            int trigFunct = 0;
            if (j > 2)
            {
                string prev = input.substr(j - 3, 3);
                if (prev == SINE || prev == COSINE || prev == TANGENT)
                    isTrig = true;

                if (prev == SINE)
                    trigFunct = 1;
                else if (prev == COSINE)
                    trigFunct = 2;
                else if (prev == TANGENT)
                    trigFunct = 3;
            }

            if (isTrig)
            {
                double res = solveTrig(stod(subInputRes), trigFunct);
                string res_s = to_string(res);

                input.erase(j, i - j + 1);
                input.insert(j, subInputRes);

                input.erase(j - 3, subInputRes.size() + 3);
                j -= 3;
                if (input.size() == 0)
                    input += res_s;
                else
                    input.insert(j, res_s);
            }
            else
            {
                input.erase(j, i - j + 1);
                input.insert(j, subInputRes);
            }

            i = j;
            p_index.pop_back();
            p_stack.pop_back();
        }
    }

    input = solveFactorial(input);
    input = solvePower(input);
    input = solveMultDiv(input);
    input = solvePlus(input);

    return input;
}

double Function::solveTrig(double input, int function)
// Precondition: function 1. sine
//                        2. cosine
//                        3. tangent
{
    //double angle = degToRad(input);

    if (function == 1)
        return sin(input);
    else if (function == 2)
        return cos(input);
    else if (function == 3)
        return tan(input);
    else
        return 0;
}

string Function::solvePlus(string input)
{
    string num_s = "0";
    double zero = 0;

    const char plusOp = PLUS;
    const char minOp = MINUS;
    char op = PLUS;

    for (int i = 0; i < input.size(); i++)
    {
        char current = input[i];

        if (isDigit(current))
            num_s += current;
        else
        {
            if (op == plusOp)
                zero += stod(num_s);
            else if (op == minOp)
                zero -= stod(num_s);

            if (current == plusOp)
                op = plusOp;
            else if (current == minOp)
                op = minOp;

            num_s = "";

            char next = input[i + 1];
            if (next == plusOp && i + 1 < input.size())
                i++;
            else if (next == minOp && i + 1 < input.size())
            {
                op = op == plusOp ? minOp : plusOp;
                i++;
            }
        }
    }
    // Final operation
    if (op == plusOp)
        zero += stod(num_s);
    else if (op == minOp)
        zero -= stod(num_s);

    // cout << "[log]: Finished computation.\n";
    // cout << "[log]: Final result: " << zero;

    return to_string(zero);
}

string Function::solveMultDiv(string input)
{
    string number1_s, number2_s;
    const char noOperator = '\0';
    const char multOp = MULT;
    const char divOp = DIV;
    const char minOp = MINUS;
    const char plusOp = PLUS;

    char op = noOperator;
    int op_index = -10;
    for (int i = 0; i < input.size(); i++)
    {
        char c = input[i];

        if ((c == multOp || c == divOp) && op == noOperator)
        {
            op_index = i;
            op = c;
        }
        else
        {
            if (isDigit(c))
            {
                if (op != noOperator)
                    number2_s += c;
                else
                    number1_s += c;
            }
            else if (c == minOp && op_index == i - 1)
                // Negative number detected (second operand)
                number2_s += c;
            else if ((c == multOp || c == divOp) && op != noOperator)
                // Detected another operator, compute the previous one
            {
                // This piece of code computes the operation
                input = makeBasicOperation(input, number1_s, number2_s, op, i);
                // RESET VALUES
                int op_size = number1_s.size() + number2_s.size() + 1;
                i -= (op_size + 1);
                number1_s = "";
                number2_s = "";
                op = noOperator;
            }
            else if (c == plusOp || c == minOp)
                // Plus or minus detected
            {
                if (op == noOperator)
                {
                    if (c == MINUS)
                        number1_s += c;
                    else
                    {
                        number1_s = "";
                        number2_s = "";
                    }
                }
                else
                    // Compute the last operator
                {
                    // This piece of code computes the operation
                    input = makeBasicOperation(input, number1_s, number2_s, op, i);
                    // RESET VALUES
                    int op_size = number1_s.size() + number2_s.size() + 1;
                    i -= (op_size + 1);
                    number1_s = "";
                    number2_s = "";
                    op = noOperator;
                }
            }
        }
    }

    if (op != noOperator)
    {
        if (op == multOp)
        {
            stringstream ssr(input);
            input = makeBasicOperation(input, number1_s, number2_s, op, input.size() - 1);
        }
        else
        {
            stringstream ssr(input);
            input = makeBasicOperation(input, number1_s, number2_s, op, input.size() - 1);
        }
    }

    return input;
}

string Function::makeBasicOperation(string input, string number1_s, string number2_s, char op, int i)
// Solves multiplications or division
// Precondition: input is the calculation
//               number1_s and number2_s are the operands
//               op is the operation to be performed
//               i is the index where it is standed
// Postcondition: Returns the same input string with the operation computed
{
    double res;

    if (op == MULT)
        res = stod(number1_s) * stod(number2_s);
    else if (op == DIV)
        res = stod(number1_s) / stod(number2_s);
    else if (op == PLUS)
        res = stod(number1_s) + stod(number2_s);
    else if (op == MINUS)
        res = stod(number1_s) - stod(number2_s);

    string res_s = to_string(res);

    int op_size = number1_s.size() + number2_s.size() + 1;
    if (op_size == input.size())
        input = res_s;
    else
    {
        int size_before = input.size();
        if (i == size_before - 1)
            // The last operation of the line (i is 1 smaller)
        {
            input.erase(i - op_size + 1, op_size);
            input += res_s;
        }
        else
            // i is one unit more (there is more computation)
        {
            input.erase(i - op_size, op_size);
            input.insert(i - op_size, res_s);
        }
    }

    return input;
}

string Function::solvePower(string input)
// This function changes all the power operations with their values in
// a string input
// Precondition: string expression
// Postcondition: expression with power calculated
{
    const char exp_op = EXPO;

    while (true)
    {
        string temp1, temp2;
        stringstream ss(input);

        getline(ss, temp1, exp_op);
        if (temp1.size() == input.size())
            break;

        getline(ss, temp2, exp_op);

        int cut_start = temp1.size();
        string fact1_s, fact2_s;

        for (int i = temp1.size() - 1; i >= 0; i--)
        {
            if (isOperator(temp1[i])) {
                if (temp1[i] == MINUS) {
                    fact1_s = temp1[i] + fact1_s;
                    cut_start--;
                }
                break;
            }
            else
            {
                fact1_s = temp1[i] + fact1_s;
                cut_start--;
            }
        }

        for (int i = 0; i < temp2.size(); i++)
        {
            if (isOperator(temp2[i]))
            {
                if (i == 0 && temp2[i] == MINUS) {
                     fact2_s += temp2[i];     
                }
                else
                    break;
            }
            else
                fact2_s += temp2[i];
        }

        int cut_size = fact1_s.size() + fact2_s.size() + 1;

        double res = pow(stod(fact1_s), stod(fact2_s));

        input.erase(cut_start, cut_size);
        input.insert(cut_start, to_string(res));

        ss.clear();
        ss.str(input);
    }
    if (input.find("-nan(ind)") != string::npos || input.find("inf") != string::npos)
        throw 2;
    return input;
}

string Function::solveFactorial(string input)
// This function changes all the factorials with their values in
// a string input
// Precondition: string expression
// Postcondition: expression with factorial replaced
{
    int lastOperatorIndex = -1;

    for (int i = 0; i < input.size(); i++)
    {
        char c = input[i];
        if (!isDigit(c))
        {
            bool factCalculated = false;

            if (c == FACT)
            {
                string subInput = input.substr(lastOperatorIndex + 1, i - lastOperatorIndex - 1);

                if (isDecimal(subInput))
                    return "Error";

                int num = stoi(subInput);
                num = factorial(num);

                subInput += FACT;
                input.erase(lastOperatorIndex + 1, subInput.size());
                input.insert(lastOperatorIndex + 1, to_string(num));
                factCalculated = true;
                i--;
            }
            if (!factCalculated)
                lastOperatorIndex = i;

            factCalculated = false;
        }
    }

    return input;
}

int Function::factorial(int n)
{
    return (n == 0) || (n == 1) ? 1 : n * factorial(n - 1);
}

double Function::degToRad(double x)
// Convert degree to radians
{
    return (x * 3.1415926535) / 180;
}

string Function::replaceVariables(string input)
// This function detects variables and replaces it with their value
// Precondition: input is the calculation string,
// vars is the map containing the variables
// Postcondition: String with the variables replaced
{
    vector<string> kwords = getKeywords();

    // cout << "[log]: Before: " << input << endl;

    string word;
    int wordStartIndex = 0;
    for (int i = 0; i < input.size(); i++)
    {
        char c = input[i];

        if (isCharacter(c))
        {
            if (word.size() == 0)
                wordStartIndex = i;

            word += c;
        }
        else
        {
            if (word.size() != 0)
                // Replace the variable with it's value
            {
                // First, check if it is sin, cos or tan
                if (word == kwords[0] || word == kwords[1] || word == kwords[1])
                {
                    word = "";
                }
                else if (variableExists(word, this->variables))
                {
                    double v = this->variables[word];
                    // Save
                    string v_s = to_string(v);

                    input.erase(i - word.size(), word.size());
                    input.insert(i - word.size(), v_s);
                    word = "";

                    i = wordStartIndex;
                }
                else
                {
                    return "Error";
                }
            }
        }
    }

    if (word.size() != 0)
        // Replace the last variable
    {
        if (variableExists(word, this->variables))
        {
            double v = this->variables[word];

            input.erase(input.size() - word.size(), word.size());
            input += to_string(v);
        }
        else
        {
            return "Error";
        }
    }

    // cout << "[log]: After: " << input << endl;
    // cout << "[log]: Variables replaced." << endl;

    return input;
}

void Function::setVariable(string key, double val)
// Adds a new variable in the variable list
// If it already exists, it overrides it
{
    if (variableExists(key, this->variables))
        this->variables[key] = val;
    else
        this->variables.insert(pair<string, double>(key, val));
}

bool Function::variableExists(string key, const map<string, double>& vars)
// Returns true if a given variable name exists.
// Precondition: key is the name of the variable, vars is the variables map
{
    if (vars.find(key) == vars.end())
        return false;

    return true;
}

vector<string> Function::getKeywords()
// Returns a vector containing the keywords
// Keyword: Reserved name that can not used as variable name
{
    vector<string> kwords;
    kwords.push_back(SINE);
    kwords.push_back(COSINE);
    kwords.push_back(TANGENT);
    kwords.push_back("y");

    return kwords;
}

bool Function::checkParenthesis(string input)
// This function checks if the parenthesis in a given expression
// is well formed
// Precondition: input is the expression
// Postcondition: Returns true if the parenthesis are well formed
{
    vector<char> p_stack;
    bool next = false;
    for (int i = 0; i < input.size(); i++)
    {
        char c = input[i];

        if (c == OPEN_PAR) {
            p_stack.push_back(c);
            next = true;
        }
        else if (c == CLOSE_PAR)
        {
            if (p_stack.size() == 0 || next)
                return false;
            else
                p_stack.pop_back();
        }
        else
            next = false;
    }

    if (p_stack.size() != 0)
        return false;

    return true;
}

bool Function::checkSintaxis(string input)
// Check if the operatos of a given expression are well formed
{
    for (int i = 0; i < input.size(); i++)
    {
        char c = input[i];

        if (i == input.size() - 1)
        {
            if (isOperator(c) && c != FACT)
                return false;
        }
        else if (c == PLUS || c == MINUS)
        {
            if (!isDigit(input[i + 1]))
                return false;
        }
        else if (c == MULT || c == DIV || c == '.')
        {
            if (!isDigit(input[i - 1]) || !isDigit(input[i + 1]))
                return false;
        }
        else if (c == FACT)
        {
            if (!isDigit(input[i - 1]) || !isOperator(input[i + 1]))
                return false;
        }
    }

    return true;
}

bool Function::isDecimal(string input)
// Returns true if the input contains decimal places
// Precondition: input is a SINGLE number in string format
// It MUST be in correct format
{
    int periods = 0;
    for (int i = 0; i < input.size(); i++)
    {
        char c = input[i];
        if (c == '.' && i != input.size() - 1)
            periods++;
    }

    return periods == 1 ? true : false;
}

bool Function::isDigit(char c)
// Returns true if the given char c is a digit, periods
// are counted as a digit
{
    int a = (int)c;

    if ((a >= 48 && a <= 57) || a == 46)
        return true;

    return false;
}

bool Function::isCharacter(char c)
// Returns true if a given char c is an alphabetic character
{
    int a = (int)c;

    if ((a >= 65 && a <= 90) || (a >= 97 && a <= 122))
        return true;

    return false;
}

bool Function::isOperator(char c)
// Returns true if a given character is an operator
{
    if (c == PLUS || c == MINUS || c == MULT || c == DIV || c == EXPO || c == FACT)
        return true;

    return false;
}

bool Function::isInVector(const vector<string>& src, string s)
{
    for (int i = 0; i < src.size(); i++)
        if (src[i] == s)
            return true;

    return false;
}