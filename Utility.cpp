#include "Utility.h"

uint8_t Utility::toIndex(uint32_t val, uint32_t maxVal, uint8_t size){
	return (val*size)/maxVal;
}

int32_t Utility::abs(int32_t x1){
	return x1 >= 0 ? x1 : -x1;
}

int32_t Utility::min(int32_t x, int32_t y){
	return x <= y ? x : y;
}

int32_t Utility::max(int32_t x, int32_t y){
	return x >= y ? x : y;
}

int32_t Utility::getSign(int32_t x){
	if(x > 0)
		return 1;
	else if(x < 0)
		return -1;
	 else
		return 0;
}

int32_t Utility::sameSign(int32_t x, int32_t y){
	return getSign(x) == getSign(y);
}

// converts num to a null-terminated string and puts it into str
// str must have enough space for at least the number of digits in num + one null terminator
uint8_t Utility::toString(char* str, uint32_t num){
	if(num < 10){
		str[0] = num + 0x30;
		str[1] = '\0';
		return 1;
	}
	uint8_t index = toString(str, num/10);
	str[index] = (num % 10) + 0x30;
	str[index+1] = '\0';
	return index + 1;
}

// adds str2 to str1, both must be null-terminated, str1 must have suffient space for itself, str2, and a null-terminator
void Utility::addStrings(char* result, char* str, char* str2){
	while(*str != '\0'){
		result[0] = str[0];
		result++;
		str++;
	}
	while(*str2 != '\0'){
		result[0] = str2[0];
		result++;
		str2++;
	}
	result[0] = '\0';
}

uint32_t Utility::sqrt(int32_t value) {
	uint32_t estimate, sqrt;
	uint32_t square = value;
	sqrt = square / 2;
	estimate = 0;

	while(sqrt != estimate) {
		estimate = sqrt;
		sqrt = (square / estimate + estimate) / 2;
	}
	return sqrt;
}
