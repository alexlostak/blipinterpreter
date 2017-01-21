#ifndef _VECTOR_H
#define _VECTOR_H

#include "String.h"
//#include "Parse.h"
#include <stdint.h>
#include <stdio.h>
//#include <stdlib.h>

/* constexpr is a C++-11 keyword that means "it's a real constant"
* both C and C++ support a keyword "const" which means "it's mostly a constant"
* the differences between "const" and "constexpr" are subtle (frankly, they're obscure)
* but the bottom line is that if you have a C++-11 compiler (or later) then you
* should declare your constants using the "constexpr" keyword */
//uint32_t minimum_size = 8;

struct vectorVariable {
	String name;
	int val;
};
template <typename T>
struct Vector {
private: 
	T* data;
	uint32_t len;
	uint32_t cap;
	//void check_range(uint32_t k);
	void check_range(uint32_t k) {
		if (k >= this->len) {
			printf("out of range index %d in vector with length %d\n", k, this->len);
			abort();
		}
	}

	//void copy(Vector& orig);
	void copy(Vector& orig) {
		delete[] this->data;
		uint32_t minimum_size = 8;
		this->len = orig.len;
		this->cap = this->len;
		if (this->len < minimum_size) {
			this->cap = minimum_size;
		}
		this->data = new int[this->cap];
		for (uint32_t k = 0; k < len; k += 1) {
			this->data[k] = orig.data[k];
		}
	}

	//void destroy(void);
public:
	uint32_t size(void) { // implicit param named "this"
		return this->len;
	}
	template <typename T>
	Vector(uint32_t init_size_of_vector);
	Vector(void) {
		len = 0;
		cap = 8;
		data = new T[cap];
	}

	Vector(Vector& orig) { copy(orig); } // copy constructor (hence, I called "copy")
	Vector& operator=(Vector& rhs) {
		if (this != &rhs) {
			delete[] data;;
			copy(rhs);
		}
		return *this;
	}

	~Vector(void) { 
		//destroy();
		delete[] data;
	}
	/*T& operator[](uint32_t index);
	void pop_back(void) { len = len - 1;  }
	void push_back(T value);*/


	Vector(uint32_t sz) {
		len = sz;
		uint32_t minimum_size = 8;
		(*this).cap = sz;
		if ((*this).cap < minimum_size) {
			(*this).cap = minimum_size;
		}

		(*this).data = new int[(*this).cap];

		for (int32_t k = 0; k < (*this).len; k += 1) {
			(*this).data[k] = 0;
		}

	}

	//Vector::~Vector(void) {
	//	delete [] data;
	//	this->len = 0; // <--- silly!!!
	//	delete this; // <--- deadly!
	//}
	T& operator[](uint32_t k) {
		this->check_range(k);
		return this->data[k];
	}


	void push_back(T val) {
		if ((*this).len == (*this).cap) {
			(*this).cap *= 2;

			T* new_data = new T[cap];

			for (uint32_t k = 0; k < (*this).len; k += 1) {
				new_data[k] = (*this).data[k];
			}

			delete[] data;

			(*this).data = new_data;
			(*this).data[(*this).len] = val;
		}
		(*this).data[(*this).len] = val;
		(*this).len += 1;
	}
};


#endif /* VECTOR_H */