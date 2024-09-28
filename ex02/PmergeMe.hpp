//
// Created by Yash on 12/2/24.
//

#ifndef CPP_09_PMERGEME_HPP
#define CPP_09_PMERGEME_HPP
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <deque>
#include <ctime>

template <typename T>
void merge(T &cont, T &left, T &right){
	int i = 0, j = 0, k = 0;
	int sizeLeft = left.size();
	int sizeRight = right.size();

	while (i < sizeLeft && j < sizeRight){
		if (left[i] < right[j]){
			cont[k] = left[i];
			i++;
		}
		else{
			cont[k] = right[j];
			j++;
		}
		k++;
	}
	while (i < sizeLeft){
		cont[k] = left[i];
		i++;
		k++;
	}
	while (j < sizeRight){
		cont[k] = right[j];
		j++;
		k++;
	}
}

template <typename T>
void insertSort(T &cont){
	int i, j, key;
	int size = cont.size();

	for (i = 1; i < size; i++){
		key = cont[i];
		j = i - 1;
		while (j >= 0 && cont[j] > key){
			cont[j + 1] = cont[j];
			j = j - 1;
		}
		cont[j + 1] = key;
	}
}


template <typename T>
void mergeSort(T &cont){
	if (cont.size() > 1 && cont.size() < 43)
		insertSort(cont);
	T left, right;
	int mid = cont.size() / 2;

	if (cont.size() <= 1)
		return;
	left.insert(left.begin(), cont.begin(), cont.begin() + mid);
	right.insert(right.begin(), cont.begin() + mid, cont.end());
	mergeSort(left);
	mergeSort(right);
	merge(cont, left, right);
}

#endif //CPP_09_PMERGEME_HPP
