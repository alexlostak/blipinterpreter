#pragma once
#include "Vector.h"
//#include "String.h"

enum FLAVOR {
	FLAVOR_NUMBER,
	FLAVOR_STRING,
	FLAVOR_VAR,
	FLAVOR_PLUS,
	FLAVOR_MINUS,
	FLAVOR_MULTIPLY,
	FLAVOR_DIVIDE,
	FLAVOR_MODULO,
	FLAVOR_AND_AND,
	FLAVOR_OR_OR,
	FLAVOR_LESS_THAN,
	FLAVOR_GREATER_THAN,
	FLAVOR_EQUAL_EQUALS,
	FLAVOR_IN_EQUAL_TO,
	FLAVOR_LESS_THAN_EQUAL_TO,
	FLAVOR_GREATER_THAN_EQUAL_TO,
	FLAVOR_NEGATION,
	FLAVOR_NOT,
	FLAVOR_NEW_VAR,
	FLAVOR_SET,
	FLAVOR_TEXT,
	FLAVOR_TEXT_QUOTE
};
struct expr {
	FLAVOR type;
	expr* left = nullptr;
	expr* right = nullptr;
	int num;
	String name;


	//approach with constructor for each flavor
	//may not actually need this for the way that makeExpr works
	expr(void) {
		this->left = nullptr;
		this->right = nullptr;
	}
	expr(int value) {
		this->type = FLAVOR_NUMBER;
		this->left = nullptr;
		this->right = nullptr;
		this->num = value;
	}
	expr(String name) {
		this->type = FLAVOR_STRING;
		this->left = nullptr;
		this->right = nullptr;
		this->name = name;
	}
	//how would this be done for plus and other expressions?
	~expr(void) {
		if (this->left != nullptr) { delete left; }
		if (this->right != nullptr) { delete right; }

	}
	static expr* makeExpr(void);
	static expr* makeSetExpr(void);
	static expr* evalExpr(expr* p);
};
