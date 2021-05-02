#include "vector.h"
#ifndef _vector_cpp
#define _vector_cpp

template<class T>
vector<T>::vector(uint8_t capacity){
	arr = new T[capacity];
	this->capacity = capacity;
	size = 0;
}

template<class T>
vector<T>::~vector(){
	delete [] arr;
}

template<class T>
bool vector<T>::push_back(T item){
	if(size == capacity)
		return false;
	size++;
	arr[size-1] = item;
	return true;
}

template<class T>
bool vector<T>::remove(uint16_t index){
	if(index >= size)
		return false;
	for(uint8_t i = index; i < size - 1; i++){
		arr[i] = arr[i+1];
	}
	size--;
	return true;
}

template<class T>
bool vector<T>::remove(T item){
	uint8_t i = 0;
	for(; i < size; i++){
		if(arr[i] == item)
			return remove(i);
	}
	return false;
}

template<class T>
T& vector<T>::operator [] (uint8_t i){
	return arr[i];
}

template<class T>
uint8_t vector<T>::len(){
	return size;
}

template<class T>
bool vector<T>::isFull(){
	return size == capacity;
}

template<class T>
T& vector<T>::back(){
	return arr[size-1];
}

#endif
