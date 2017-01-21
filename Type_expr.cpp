//
//  Type_expr.cpp
//  
//
//  Created by Alex Lostak on 4/17/16.
//
#ifndef _Type_expr_h
#define _Type_expr_h 1
//#include "String.h"
#include "Type_expr.h"
#include "Parse.h"
#include "BST.h"
//#include <stdio.h>

struct VariableThingy {
	String name;
	int value;
	bool operator<(const VariableThingy& other_thing) const {
		return name < other_thing.name;
	}

	bool operator==(const VariableThingy& other_thing) const {
		return name == other_thing.name;
	}
	bool operator!=(const VariableThingy& other_thing) const {
		return name != other_thing.name;
	}
};
BST<VariableThingy> symbolTable;

expr* expr::makeExpr(void) {
	expr* result = new expr(); //should this be a function or a constructor?
	read_next_token();
	String token = next_token();
	/*if (token == "text" || token == "var" || token == "output" || token == "set") {
		return result;
	}*/
	if (next_token_type == NUMBER) {
		result->type = FLAVOR_NUMBER;
		result->num = token_number_value;
	}
	else if (next_token_type == NAME) {
		result->type = FLAVOR_VAR;
		result->name = token;
		VariableThingy check;
		check.name = token;
		if (symbolTable.isIn(check) == false) {
			result->left = makeExpr();
		}
	}
	else if (next_token_type == STRING) {
		result->type = FLAVOR_STRING;
		result->name = token;
	}
	else if (next_token_type == SYMBOL) {
		//binary math operators
		if (token == "+") {
			result->type = FLAVOR_PLUS;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		else if (token == "-") {
			result->type = FLAVOR_MINUS;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		else if (token == "*") {
			result->type = FLAVOR_MULTIPLY;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		else if (token == "/") {
			result->type = FLAVOR_DIVIDE;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		else if (token == "%") {
			result->type = FLAVOR_MODULO;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		//binary logic operators
		else if (token == "&&") {
			result->type = FLAVOR_AND_AND;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		else if (token == "||") {
			result->type = FLAVOR_OR_OR;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		//comparison operators
		else if (token == "<") {
			result->type = FLAVOR_LESS_THAN;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		else if (token == ">") {
			result->type = FLAVOR_GREATER_THAN;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		else if (token == "==") {
			result->type = FLAVOR_EQUAL_EQUALS;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		else if (token == "!=") {
			result->type = FLAVOR_IN_EQUAL_TO;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		else if (token == "<=") {
			result->type = FLAVOR_LESS_THAN_EQUAL_TO;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		else if (token == ">=") {
			result->type = FLAVOR_GREATER_THAN_EQUAL_TO;
			result->left = makeExpr();
			result->right = makeExpr();
		}
		//unary operators
		else if (token == "!") {
			result->type = FLAVOR_NOT;
			result->left = makeExpr();
		}
		else if (token == "~") {
			result->type = FLAVOR_NEGATION;
			result->left = makeExpr();
		}
		/*else if (token == "\"") { 
			result->type = FLAVOR_TEXT_QUOTE;
			result->name = token; //wrong
			result = makeExpr();
		}*/
	}
	else if (next_token_type == END) {
		printf("ERROR: early endpoint");
	}
	else if (next_token_type == INVALID) {
		printf("ERROR: INVALID next_token_type");
	}
	else {
		printf("ERROR: no match for token type");
	}
	return result;
};

expr* expr::makeSetExpr(void) {
	//initially get value out of variable but after that don't 
	expr* result = new expr();
	read_next_token();
	String token = next_token();
	result->type = FLAVOR_VAR;
	result->name = token;
	result->left = makeExpr();
	return result;
}

expr* expr::evalExpr(expr* p) {

	if (p->type == FLAVOR_NUMBER) {
		return p;
	}
	else if (p->type == FLAVOR_VAR) {//do I need to access my symbol table here or should I hold my variable value in my expression tree?
									 //ANSWER: need to go to symbol table
		int symbolVal = 0;
		//find variable in symbol table
		//having trouble using vector because vector does not hold names
			//need way to implement that
			//using BST instead
		//if in symbol table get value
		VariableThingy temp;
		temp.name = p->name;
		VariableThingy *check;
		check = symbolTable.findValue(temp);
		if (check != nullptr) {
		//how do I get that variable value out of the tree?
		//need to then assign value to p
			p->num = check->value;
			return p;
		}
		else {
			printf("ERROR: UNDEFINED VARIABLE ATTEMPTING TO ACCESS SYMBOL TABLE");
			
			return nullptr;
		}
	}
	else if (p->type == FLAVOR_NEW_VAR) {
		VariableThingy temp;
		temp.name = p->name;
		if (symbolTable.isIn(temp) == false) {
			expr* value = evalExpr(p->left);
			temp.value = value->num;
			symbolTable.insert(temp);
			p->num = value->num;
			return p;
		}
		else {
			printf("variable %s incorrectly re-initialized\n", p->name.c_str());
			/*expr* left = evalExpr(p->left);
			p->num = left->num;
			VariableThingy *check;
			check = symbolTable.findValue(temp);
			check->value = p->num;
			return p;*/
			expr* makeVarError = expr::makeExpr();
			p->left = makeVarError;
			p->type = FLAVOR_SET;
			expr* evalVarError = expr::evalExpr(p);
			temp.value = evalVarError->num;
			p->num = evalVarError->num;
			return p;
		}
	}
	else if (p->type == FLAVOR_SET) {
		VariableThingy temp;
		temp.name = p->name;
		expr* left = evalExpr(p->left);
		p->num = left->num;
		VariableThingy *check;
		check = symbolTable.findValue(temp);
		if (check != nullptr) {
			check->value = p->num;
			return p;
		}
		else {
			//printf("ERROR: UNDEFINED VARIABLE ATTEMPTING TO ACCESS SYMBOL TABLE");
			printf("variable %s not declared\n", temp.name.c_str());
			temp.value = p->num;
			symbolTable.insert(temp);
			return p;
		}

	}
	//binary math operators
	else if (p->type == FLAVOR_PLUS) {
		//want to add whats on both sides
		//figure whats on left
		expr* left = evalExpr(p->left);
		//figure whats on right
		expr* right = evalExpr(p->right);
		//add together and set that equal to p->num
		p->num = left->num + right->num;
		return p;
	}
	else if (p->type == FLAVOR_MINUS) {
		expr* left = evalExpr(p->left);
		expr* right = evalExpr(p->right);
		p->num = left->num - right->num;
		return p;
	}
	else if (p->type == FLAVOR_MULTIPLY) {
		expr* left = evalExpr(p->left);
		expr* right = evalExpr(p->right);
		p->num = left->num * right->num;
		return p;
	}
	else if (p->type == FLAVOR_DIVIDE) {
		expr* left = evalExpr(p->left);
		expr* right = evalExpr(p->right);
		p->num = left->num / right->num;
		return p;
	}
	else if (p->type == FLAVOR_MODULO) {
		expr* left = evalExpr(p->left);
		expr* right = evalExpr(p->right);
		p->num = left->num % right->num;
		return p;
	}
	//binary operators
	else if (p->type == FLAVOR_AND_AND) {
		expr* left = evalExpr(p->left);
		expr* right = evalExpr(p->right);
		p->num = (left->num && right->num);
		return p;
	}
	else if (p->type == FLAVOR_OR_OR) {
		expr* left = evalExpr(p->left);
		expr* right = evalExpr(p->right);
		p->num = (left->num && right->num);
		return p;
	}
	//comparison operators
	else if (p->type == FLAVOR_LESS_THAN) {
		expr* left = evalExpr(p->left);
		expr* right = evalExpr(p->right);
		p->num = (left->num < right->num);
		return p;
	}
	else if (p->type == FLAVOR_GREATER_THAN) {
		expr* left = evalExpr(p->left);
		expr* right = evalExpr(p->right);
		p->num = (left->num > right->num);
		return p;
	}
	else if (p->type == FLAVOR_EQUAL_EQUALS) {
		expr* left = evalExpr(p->left);
		expr* right = evalExpr(p->right);
		p->num = (left->num == right->num);
		return p;
	}
	else if (p->type == FLAVOR_IN_EQUAL_TO) {
		expr* left = evalExpr(p->left);
		expr* right = evalExpr(p->right);
		p->num = (left->num != right->num);
		return p;
	}
	else if (p->type == FLAVOR_LESS_THAN_EQUAL_TO) {
		expr* left = evalExpr(p->left);
		expr* right = evalExpr(p->right);
		p->num = (left->num <= right->num);
		return p;
	}
	else if (p->type == FLAVOR_GREATER_THAN_EQUAL_TO) {
		expr* left = evalExpr(p->left);
		expr* right = evalExpr(p->right);
		p->num = (left->num >= right->num);
		return p;
	}
	//unary operators
	else if (p->type == FLAVOR_NOT) {//NOT CORRECT, P->NUM IS NOT PROPER TARGET
		expr* left = evalExpr(p->left);
		int notNum = left->num;
		p->num = !(notNum);
		return p;
	}
	else if (p->type == FLAVOR_NEGATION) {//NOT CORRECT, P->NUM IS NOT PROPER TARGET
		expr* left = evalExpr(p->left);
		int negNum = (left->num);
		p->num = -(negNum);
		return p;
	}
	else {
		//printf("ERROR: HIT CATCH ALL FOR evalExpr\n");
		return p;
	}
};




#endif