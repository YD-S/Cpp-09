
#include "PmergeMe.hpp"
#include <ctime>

int input(char *a) {
	for (int i = 0; a[i]; i++) {
		if (std::isdigit(a[i]) == 0 || std::atoi(a) < 0) {
			std::cout << "Invalid input" << std::endl;
			exit(1);
		}
	}
	return std::atoi(a);
}

int main(int ac, char **av) {
	std::vector<int> v;
	std::deque<int> d;
	double time;
	(void)ac;
	for (int i = 1; i < ac; i++){
		int j = input(av[i]);
		v.push_back(j);
		d.push_back(j);
	}
	std::cout << "Before: ";
	for (std::vector<int>::iterator it = v.begin(); it != v.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	struct timespec start, end = {};
	clock_gettime(CLOCK_REALTIME, &start);
	mergeSort(v);
	clock_gettime(CLOCK_REALTIME, &end);
	std::cout << "After: ";
	for (std::vector<int>::iterator it = v.begin(); it != v.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	time = (end.tv_sec - start.tv_sec) * 1000000. + (end.tv_nsec - start.tv_nsec) / 1000.;
	std::cout << "Time to process a range of " << v.size() <<  " elements with std::vector : " << time << " us" << std::endl;
	clock_gettime(CLOCK_REALTIME, &start);
	mergeSort(d);
	clock_gettime(CLOCK_REALTIME, &end);
	time = (end.tv_sec - start.tv_sec) * 1000000. + (end.tv_nsec - start.tv_nsec) / 1000.;
	std::cout << "Time to process a range of " << d.size() <<  " elements with std::deque : " << time << " us" << std::endl;

	return 0;
}

