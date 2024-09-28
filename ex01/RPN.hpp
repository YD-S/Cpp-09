//
// Created by Yash on 12/2/24.
//

#ifndef CPP_09_RPN_HPP
#define CPP_09_RPN_HPP
#include <iostream>
#include <stack>
#include <string>
#include <sstream>

class RPN {
private:
	std::stack<int> _stack;
	int _size;
public:
	RPN();
	RPN(const std::string &args);
	~RPN();
	RPN(const RPN &rhs);
	RPN &operator=(const RPN &rhs);
	int ft_stoi(const std::string &str);
	void print();
	void parse(std::string str);

	class EmptyStackException : public std::exception {
		public:
			virtual const char *what() const throw();
	};
};

#endif //CPP_09_RPN_HPP
