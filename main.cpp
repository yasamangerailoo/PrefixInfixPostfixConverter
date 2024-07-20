#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <string>

using namespace std;

int precedence(char op);
bool isValidOperand(char ch);
bool isValidInfix(const string& infix);
int evaluateInfix(string infix);
string infixToPostfix(string infix);
string infixToPrefix(string infix);
string postfixToInfix(string postfix);
string prefixToInfix(string prefix);
bool isValidPostfix(string postfix);
bool isValidPrefix(string prefix);
int evaluatePostfix(string postfix);
int evaluatePrefix(string prefix);

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

bool isValidOperand(char ch) {
    string validChars = "+-*/^";
    return isalnum(ch) || validChars.find(ch) != string::npos;
}

bool isValidInfix(const string& infix) {
    int n = infix.length();
    int openBrackets = 0;
    bool lastWasOperator = true;

    for (int i = 0; i < n; ++i) {
        char ch = infix[i];

        if (ch == ' ') continue;

        if (isalnum(ch)) {
            lastWasOperator = false;
        } else if (ch == '(') {
            openBrackets++;
            lastWasOperator = true;
        } else if (ch == ')') {
            openBrackets--;
            if (openBrackets < 0 || lastWasOperator) return false;
            lastWasOperator = false;
        } else if (isValidOperand(ch)) {
            if (lastWasOperator) return false;
            lastWasOperator = true;
        } else {
            return false;
        }
    }

    return openBrackets == 0 && !lastWasOperator;
}

int evaluateInfix(string infix) {
    stack<int> operands;
    stack<char> operators;

    for (int i = 0; i < infix.length(); ++i) {
        char ch = infix[i];

        if (isdigit(ch)) {
            int operand = 0;
            while (i < infix.length() && isdigit(infix[i])) {
                operand = operand * 10 + (infix[i] - '0');
                ++i;
            }
            operands.push(operand);
            --i;
        } else if (ch == '(') {
            operators.push(ch);
        } else if (ch == ')') {
            while (!operators.empty() && operators.top() != '(') {
                int op2 = operands.top(); operands.pop();
                int op1 = operands.top(); operands.pop();
                char op = operators.top(); operators.pop();
                switch (op) {
                    case '+': operands.push(op1 + op2); break;
                    case '-': operands.push(op1 - op2); break;
                    case '*': operands.push(op1 * op2); break;
                    case '/':
                        if (op2 == 0) {
                            cout << "Error: Division by zero" << endl;
                            return 0;
                        }
                        operands.push(op1 / op2);
                        break;
                    case '^': operands.push(pow(op1, op2)); break;
                }
            }
            if (!operators.empty()) operators.pop();
        } else {
            while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                int op2 = operands.top(); operands.pop();
                int op1 = operands.top(); operands.pop();
                char op = operators.top(); operators.pop();
                switch (op) {
                    case '+': operands.push(op1 + op2); break;
                    case '-': operands.push(op1 - op2); break;
                    case '*': operands.push(op1 * op2); break;
                    case '/':
                        if (op2 == 0) {
                            cout << "Error: Division by zero" << endl;
                            return 0;
                        }
                        operands.push(op1 / op2);
                        break;
                    case '^': operands.push(pow(op1, op2)); break;
                }
            }
            operators.push(ch);
        }
    }

    while (!operators.empty()) {
        int op2 = operands.top(); operands.pop();
        int op1 = operands.top(); operands.pop();
        char op = operators.top(); operators.pop();
        switch (op) {
            case '+': operands.push(op1 + op2); break;
            case '-': operands.push(op1 - op2); break;
            case '*': operands.push(op1 * op2); break;
            case '/':
                if (op2 == 0) {
                    cout << "Error: Division by zero" << endl;
                    return 0;
                }
                operands.push(op1 / op2);
                break;
            case '^': operands.push(pow(op1, op2)); break;
        }
    }

    return operands.top();
}

string infixToPostfix(string infix) {
    stack<char> stk;
    string postfix = "";
    for (char& ch : infix) {
        if (isalnum(ch))
            postfix += ch;
        else if (ch == '(')
            stk.push(ch);
        else if (ch == ')') {
            while (!stk.empty() && stk.top() != '(') {
                postfix += stk.top();
                stk.pop();
            }
            if (!stk.empty()) stk.pop();
        }
        else {
            while (!stk.empty() && precedence(stk.top()) >= precedence(ch)) {
                postfix += stk.top();
                stk.pop();
            }
            stk.push(ch);
        }
    }
    while (!stk.empty()) {
        postfix += stk.top();
        stk.pop();
    }
    return postfix;
}

string infixToPrefix(string infix) {
    reverse(infix.begin(), infix.end());
    for (char& ch : infix) {
        if (ch == '(') ch = ')';
        else if (ch == ')') ch = '(';
    }
    string prefix = infixToPostfix(infix);
    reverse(prefix.begin(), prefix.end());
    return prefix;
}

string postfixToInfix(string postfix) {
    stack<string> stk;
    for (char& ch : postfix) {
        if (isalnum(ch))
            stk.push(string(1, ch));
        else {
            if (stk.size() < 2) {
                cout << "Invalid postfix expression" << endl;
                return "";
            }
            string op2 = stk.top(); stk.pop();
            string op1 = stk.top(); stk.pop();
            string exp = "(" + op1 + ch + op2 + ")";
            stk.push(exp);
        }
    }
    if (stk.size() != 1) {
        cout << "Invalid postfix expression" << endl;
        return "";
    }
    return stk.top();
}

string prefixToInfix(string prefix) {
    stack<string> stk;
    reverse(prefix.begin(), prefix.end());
    for (char& ch : prefix) {
        if (isalnum(ch))
            stk.push(string(1, ch));
        else {
            if (stk.size() < 2) {
                cout << "Invalid prefix expression" << endl;
                return "";
            }
            string op1 = stk.top(); stk.pop();
            string op2 = stk.top(); stk.pop();
            string exp = "(" + op1 + ch + op2 + ")";
            stk.push(exp);
        }
    }
    if (stk.size() != 1) {
        cout << "Invalid prefix expression" << endl;
        return "";
    }
    return stk.top();
}

bool isValidPostfix(string postfix) {
    int count = 0;
    for (char& ch : postfix) {
        if (isalnum(ch))
            count++;
        else
            count--;
        if (count < 1) return false;
    }
    return count == 1;
}

bool isValidPrefix(string prefix) {
    int count = 0;
    reverse(prefix.begin(), prefix.end());
    for (char& ch : prefix) {
        if (isalnum(ch))
            count++;
        else
            count--;
        if (count < 1) return false;
    }
    return count == 1;
}

int evaluatePostfix(string postfix) {
    if (!isValidPostfix(postfix)) {
        cout << "Invalid postfix expression for evaluation" << endl;
        return 0;
    }
    stack<int> stk;
    for (char& ch : postfix) {
        if (isdigit(ch))
            stk.push(ch - '0');
        else {
            int op2 = stk.top(); stk.pop();
            int op1 = stk.top(); stk.pop();
            switch (ch) {
                case '+': stk.push(op1 + op2); break;
                case '-': stk.push(op1 - op2); break;
                case '*': stk.push(op1 * op2); break;
                case '/':
                    if (op2 == 0) {
                        cout << "Error: Division by zero" << endl;
                        return 0;
                    }
                    stk.push(op1 / op2);
                    break;
                case '^': stk.push(pow(op1, op2)); break;
                default:
                    cout << "Unknown operator: " << ch << endl;
                    return 0;
            }
        }
    }
    return stk.top();
}

int evaluatePrefix(string prefix) {
    if (!isValidPrefix(prefix)) {
        cout << "Invalid prefix expression for evaluation" << endl;
        return 0;
    }
    stack<int> stk;
    reverse(prefix.begin(), prefix.end());
    for (char& ch : prefix) {
        if (isdigit(ch))
            stk.push(ch - '0');
        else {
            int op1 = stk.top(); stk.pop();
            int op2 = stk.top(); stk.pop();
            switch (ch) {
                case '+': stk.push(op1 + op2); break;
                case '-': stk.push(op1 - op2); break;
                case '*': stk.push(op1 * op2); break;
                case '/':
                    if (op2 == 0) {
                        cout << "Error: Division by zero" << endl;
                        return 0;
                    }
                    stk.push(op1 / op2);
                    break;
                case '^': stk.push(pow(op1, op2)); break;
                default:
                    cout << "Unknown operator: " << ch << endl;
                    return 0;
            }
        }
    }
    return stk.top();
}

int main() {
    char choice;
    do {
        string expression;
        string type;
        cout << "Enter the type of expression (infix, postfix, prefix): ";
        cin >> type;
        cout << "Enter the expression: ";
        cin.ignore();
        getline(cin, expression);

        if (type == "infix") {
            if (!isValidInfix(expression)) {
                cout << "Invalid infix expression" << endl;
            } else {
                string postfix = infixToPostfix(expression);
                string prefix = infixToPrefix(expression);
                cout << "Postfix: " << postfix << endl;
                cout << "Prefix: " << prefix << endl;
                cout << "Evaluated: " << evaluateInfix(expression) << endl;

                // Write results to file
                ofstream outputFile("dsproject.c", ios_base::app);
                if (!outputFile.is_open()) {
                    cout << "Unable to open file";
                    return 1; // Exit program with an error
                }
                outputFile << "Infix: " << expression << endl;
                outputFile << "Postfix: " << postfix << endl;
                outputFile << "Prefix: " << prefix << endl;
                outputFile << "Evaluated: " << evaluateInfix(expression) << endl << endl;
                outputFile.close();
            }
        } else if (type == "postfix") {
            if (!isValidPostfix(expression)) {
                cout << "Invalid postfix expression" << endl;
            } else {
                string infix = postfixToInfix(expression);
                if (!infix.empty()) {
                    cout << "Infix: " << infix << endl;
                    string prefix = infixToPrefix(infix);
                    cout << "Prefix: " << prefix << endl;
                    cout << "Evaluated: " << evaluatePostfix(expression) << endl;

                    // Write results to file
                    ofstream outputFile("dsproject.c", ios_base::app);
                    if (!outputFile.is_open()) {
                        cout << "Unable to open file";
                        return 1; // Exit program with an error
                    }
                    outputFile << "Infix: " << infix << endl;
                    outputFile << "Postfix: " << expression << endl;
                    outputFile << "Prefix: " << prefix << endl;
                    outputFile << "Evaluated: " << evaluatePostfix(expression) << endl << endl;
                    outputFile.close();
                }
            }
        } else if (type == "prefix") {
            if (!isValidPrefix(expression)) {
                cout << "Invalid prefix expression" << endl;
            } else {
                string infix = prefixToInfix(expression);
                if (!infix.empty()) {
                    cout << "Infix: " << infix << endl;
                    string postfix = infixToPostfix(infix);
                    cout << "Postfix: " << postfix << endl;
                    cout << "Evaluated: " << evaluatePrefix(expression) << endl;

                    // Write results to file
                    ofstream outputFile("dsproject.c", ios_base::app);
                    if (!outputFile.is_open()) {
                        cout << "Unable to open file";
                        return 1; // Exit program with an error
                    }
                    outputFile << "Infix: " << infix << endl;
                    outputFile << "Postfix: " << postfix << endl;
                    outputFile << "Prefix: " << expression << endl;
                    outputFile << "Evaluated: " << evaluatePrefix(expression) << endl << endl;
                    outputFile.close();
                }
            }
        } else {
            cout << "Invalid type" << endl;
        }

        cout << "Do you want to enter another expression? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}
