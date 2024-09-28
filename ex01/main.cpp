//
// Created by Yash on 12/2/24.
//

#include "RPN.hpp"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: ./rpn \"1 2 + 4 * 5 + 3 -\"" << std::endl;
		return 1;
	}
	try {
		std::string str = argv[1];
		RPN rpn;
		rpn.parse(str);
		rpn.print();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}