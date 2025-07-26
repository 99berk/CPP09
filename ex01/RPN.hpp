#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN
{
    private:
        std::stack<int> _stack;
    
        bool isOperator(const std::string& token);
        bool isNumber(const std::string& token);
        int performOperation(int a, int b, char op);
    
    public:
        RPN();
        RPN(const RPN& other);
        RPN& operator=(const RPN& other);
        ~RPN();
        
        int calculate(const std::string& expression);
};

#endif