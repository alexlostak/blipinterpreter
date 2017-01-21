#pragma once
//
//  statement.hpp
//  
//
//  Created by Alex Lostak on 5/3/16.
//
//
//NOT SURE WHAT FILES TO INCLUDE HERE, TYPE_EXPR NOW CONTAINS VECTOR TO HELP THIS BUT IF i INCLUDE IT HERE THEN IT CAUSES ERRORS
/*#ifndef _statement_h
#define _statement_h 1*/
#include "Type_expr.h"
//#include "Vector.h"
//#include <stdio.h>


enum statement_type {
	STATEMENT_TEXT,
	STATEMENT_OUTPUT,
	STATEMENT_VAR,
	STATEMENT_SET,
	STATEMENT_COMMENT,
	STATEMENT_DO,
	STATEMENT_OD,
	STATEMENT_IF,
	STATEMENT_FI,
	STATEMENT_ELSE,
	STATEMENT_END
};

struct statement {
	statement_type flavor;
	Vector <statement*> body;
	expr* leaf;
	Vector <statement*> body2;

	statement(void) {
		this->leaf = nullptr;
	}
	~statement(void) {
		if (this->leaf != nullptr) {
			delete this->leaf;
		}
	}
	static statement* makeStatement(void);
	static statement* makeElseStatement(void);
	static void evalStatement(statement* p);
};





//#endif /* statement_ */
