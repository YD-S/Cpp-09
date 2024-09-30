//
// Created by Yash on 12/2/24.
//

#include "RPN.hpp"

RPN::RPN() : _size(0) {}

RPN::~RPN() {}

RPN::RPN(const RPN &rhs) {
	*this = rhs;
}

RPN &RPN::operator=(const RPN &rhs) {
	if (this != &rhs) {
		_stack = rhs._stack;
		_size = rhs._size;
	}
	return *this;
}

void RPN::print() {
	std::stack<int> temp = _stack;
	for (int i = 0; i < _size; i++) {
		std::cout << temp.top() << std::endl;
		temp.pop();
	}
}

int RPN::ft_stoi(const std::string &str) {
	std::istringstream iss(str);
	int result;

	if (!(iss >> result)) {
		throw std::invalid_argument("Error");
	}
	if (iss.fail() || !iss.eof()) {
		throw std::out_of_range("ft_stoi: Out of range");
	}
	return result;
}

void RPN::parse(std::string str) {
	std::string::iterator it = str.begin();
    while (it != str.end()) {
        char c = *it;
        if (c == ' ') {
            ++it;
            continue;
        }
        if ((c >= '0' && c <= '9') || c == '+' || c == '-' || c == '*' || c == '/') {
            if (c >= '0' && c <= '9') {
                std::string::iterator next = it + 1;
                if (next != str.end() && *next != ' ') {
                    throw std::runtime_error("Invalid input: multi-digit numbers are not allowed");
                }
            }
            ++it;
        } else {
            throw std::runtime_error("Invalid character in input string");
        }
    }

	std::string token;
	std::istringstream iss;
	iss.str(str);

	if(str.empty()) {
		throw RPN::EmptyStackException();
	}
	while (iss >> token) {
		if (token == "+" || token == "-" || token == "*" || token == "/") {
			if (_size < 2) {
				throw RPN::EmptyStackException();
			}
			int a = _stack.top();
			_stack.pop();
			int b = _stack.top();
			_stack.pop();
			if (a == 0 && token == "/") {
				throw std::invalid_argument("Division by zero");
			}
			if (token == "+") {
				_stack.push(b + a);
			} else if (token == "-") {
				_stack.push(b - a);
			} else if (token == "*") {
				_stack.push(b * a);
			} else if (token == "/") {
				_stack.push(b / a);
			}
			_size--;
		} else {
			_stack.push(ft_stoi(token));
			_size++;
		}
	}
	if (_stack.size() != 1) {
		throw RPN::EmptyStackException();
	}
}

const char *RPN::EmptyStackException::what() const throw() {
	return "Stack is empty! Try again.";
}