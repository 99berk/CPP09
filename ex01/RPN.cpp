#include "RPN.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

RPN::RPN() {}

RPN::RPN(const RPN& other) : _stack(other._stack) {}

RPN& RPN::operator=(const RPN& other)
{
    if (this != &other)
        _stack = other._stack;
    return *this;
}

RPN::~RPN() {}

bool RPN::isOperator(const std::string& token)
{
    const char* operators[] = { "+", "-", "*", "/" };
    for (size_t i = 0; i < 4; ++i)
    {
        if (token == operators[i])
            return true;
    }
    return false;
}

bool RPN::isNumber(const std::string& token)
{
    if (token.empty()) 
        return false;
    
    size_t start = 0;
    if (token[0] == '-' || token[0] == '+')
    {
        if (token.length() == 1) 
            return false;
        start = 1;
    }
    
    for (size_t i = start; i < token.length(); ++i)
    {
        if (!std::isdigit(token[i]))
            return false;
    }
    return true;
}

int RPN::performOperation(int a, int b, char op)
{
    switch (op) 
    {
        case '+': 
            return a + b;
        case '-': 
            return a - b;
        case '*': 
            return a * b;
        case '/':
            if (b == 0) 
                throw std::runtime_error("Division by zero");
            return a / b;
        default:
            throw std::runtime_error("Invalid operator");
    }
}

int RPN::calculate(const std::string& expression)
{
    size_t pos = 0;
    while (pos < expression.length())
    {
        while (pos < expression.length() && expression[pos] == ' ')
            ++pos;

        size_t token_end = pos;
        while (token_end < expression.length() && expression[token_end] != ' ')
            ++token_end;

        std::string token = expression.substr(pos, token_end - pos);

        if (!token.empty())
        {
            if (isNumber(token))
            {
                int num = std::atoi(token.c_str());
                if (num >= 10)
                    throw std::runtime_error("Numbers must be less than 10");
                _stack.push(num);
            }
            else if (isOperator(token))
            {
                if (_stack.size() < 2)
                    throw std::runtime_error("Insufficient operands");

                int b = _stack.top();
                _stack.pop();
                int a = _stack.top();
                _stack.pop();

                int result = performOperation(a, b, token[0]);
                _stack.push(result);
            }
            else
                throw std::runtime_error("Invalid token");
        }
        pos = token_end + 1;
    }

    if (_stack.size() != 1)
        throw std::runtime_error("Invalid expression");

    return _stack.top();
}
