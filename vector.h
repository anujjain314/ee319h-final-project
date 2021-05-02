#ifndef _vector_h
#define _vector_h
#include <stdint.h>

template <class T>
class vector{
	T *arr;
	uint8_t size;
	uint8_t capacity;
	public:
		vector<T>(uint8_t capacity);
		~vector<T>();
		bool push_back(T item);
		bool remove(uint16_t index);
		bool remove(T item);
		T& operator [] (uint8_t i);
		uint8_t len();
		bool isFull();
		T& back();
};
#include "vector.cpp"

#endif
