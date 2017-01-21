//
//  statement.cpp
//  
//
//  Created by Alex Lostak on 5/3/16.
//
//

#ifndef _statement_h
#define _statement_h 1
//#include "String.h"
#include "Parse.h"
#include "statement.h"
//#include "Type_expr.h"
//#include "Vector.h"
//#include <stdio.h>

Vector<statement*> statementTable;

statement* statement::makeStatement(void) {
	statement* result = new statement();
	read_next_token();
	String token = next_token();

	if (next_token_type == END) {
		result->flavor = STATEMENT_END;
		return result;
	}
	else if (token == "text") {
		result->flavor = STATEMENT_TEXT;
		//need to handle this properly but not dealing with it now
		read_next_token();
		String textOutput;
		textOutput = next_token();
		//printf("%s", textOutput.c_str());
		result->leaf = new expr();
		result->leaf->type = FLAVOR_TEXT;
		result->leaf->name = textOutput;
		return result;
	}
	else if (token == "output") {
		result->flavor = STATEMENT_OUTPUT;
		result->leaf = expr::makeExpr();
		return result;
	}
	else if (token == "var") {
		result->flavor = STATEMENT_VAR;
		result->leaf = expr::makeExpr();
		result->leaf->type = FLAVOR_NEW_VAR;
		return result;
	}
	else if (token == "set") {
		result->flavor = STATEMENT_SET;
		result->leaf = expr::makeSetExpr();
		result->leaf->type = FLAVOR_SET;
		return result;
	}
	else if (token == "do") {
		result->flavor = STATEMENT_DO;
 		result->leaf = expr::makeExpr();
		String(peek_next_token);
		int i = 0;
		result->body.push_back(makeStatement());
		//i++;
		while ( result->body[i]->flavor != STATEMENT_OD) {
			result->body.push_back(makeStatement());
			/*if (result->body[i]->flavor != STATEMENT_OD) {
				i++;
			}*/
			i++;

		}
		
		return result;
	}
	else if (token == "od") {
		result->flavor = STATEMENT_OD;
		result->leaf = nullptr;
		return result;
	}
	else if (token == "if") {
		result->flavor = STATEMENT_IF;
		result->leaf = expr::makeExpr();
		String(peek_next_token);
		int i = 0;
		result->body.push_back(makeStatement());
		while (result->body[i]->flavor != STATEMENT_FI && result->body[i]->flavor != STATEMENT_ELSE) {
			result->body.push_back(makeStatement());
			i++;
		}
		return result;
	}
	else if (token == "fi") {
		result->flavor = STATEMENT_FI;
		result->leaf = nullptr;
		return result;
	}
	else if (token == "else") {
		result->flavor = STATEMENT_ELSE;
		result->leaf = nullptr;
		int i = 0;
		result->body.push_back(makeStatement());
		while (result->body[i]->flavor != STATEMENT_FI) {
			result->body.push_back(makeStatement());
			i++;
		}
		return result;
	}
	else if (token == "//") {
		result->flavor = STATEMENT_COMMENT;
		return result;
	}
	//NEED TO STILL DEAL WITH IF AND ELSE AS WELL AS COMMENT
	else {
		//printf("UNDEFINED makeSTATEMENT BEHAVIOR\n");
		return result;
	}
}

/*statement* statement::makeElseStatement(void) {
	statement* result = new statement();
	read_next_token();
	String token = next_token();

	if (next_token_type == END) {
		result->flavor = STATEMENT_END;
		return result;
	}
	else if (token == "text") {
		result->flavor = STATEMENT_TEXT;
		//need to handle this properly but not dealing with it now
		read_next_token();
		String textOutput;
		textOutput = next_token();
		//printf("%s", textOutput.c_str());
		result->leaf = new expr();
		result->leaf->type = FLAVOR_TEXT;
		result->leaf->name = textOutput;
		return result;
	}
	else if (token == "output") {
		result->flavor = STATEMENT_OUTPUT;
		result->leaf = expr::makeExpr();
		return result;
	}
	else if (token == "var") {
		result->flavor = STATEMENT_VAR;
		result->leaf = expr::makeExpr();
		result->leaf->type = FLAVOR_NEW_VAR;
		return result;
	}
	else if (token == "set") {
		result->flavor = STATEMENT_SET;
		result->leaf = expr::makeSetExpr();
		result->leaf->type = FLAVOR_SET;
		return result;
	}
	else if (token == "do") {
		result->flavor = STATEMENT_DO;
		result->leaf = expr::makeExpr();
		String(peek_next_token);
		int i = 0;
		result->body.push_back(makeStatement());
		//i++;
		while (result->body[i]->flavor != STATEMENT_OD) {
			result->body.push_back(makeStatement());
			/*if (result->body[i]->flavor != STATEMENT_OD) {
			i++;
			}*//*
			i++;

		}

		return result;
	}
	else if (token == "od") {
		result->flavor = STATEMENT_OD;
		result->leaf = nullptr;
		return result;
	}
	else if (token == "if") {
		result->flavor = STATEMENT_IF;
		result->leaf = expr::makeExpr();
		String(peek_next_token);
		int i = 0;
		result->body.push_back(makeStatement());
		while (result->body[i]->flavor != STATEMENT_FI) {
			result->body.push_back(makeStatement());
			i++;
		}
		return result;
	}
	else if (token == "fi") {
		result->flavor = STATEMENT_FI;
		result->leaf = nullptr;
		return result;
	}
	else if (token == "else") {
		result->flavor = STATEMENT_ELSE;
		result->leaf = nullptr;
		return result;
	}
	else if (token == "//") {
		result->flavor = STATEMENT_COMMENT;
		return result;
	}
	//NEED TO STILL DEAL WITH IF AND ELSE AS WELL AS COMMENT
	else {
		printf("UNDEFINED makeSTATEMENT BEHAVIOR\n");
		return result;
	}
}*/
void statement::evalStatement(statement* p) {//made not need a return type

	if (p->flavor == STATEMENT_END) {
		return;
	}
	else if (p->flavor == STATEMENT_TEXT) {
		printf("%s", p->leaf->name.c_str());
		return;
	}
	else if (p->flavor == STATEMENT_OUTPUT) {
		p->leaf = expr::evalExpr(p->leaf);
		printf("%d", p->leaf->num);
		return;
	}
	else if (p->flavor == STATEMENT_VAR) {
		p->leaf = expr::evalExpr(p->leaf);
		return;
	}
	else if (p->flavor == STATEMENT_SET) {
		p->leaf = expr::evalExpr(p->leaf);
		return;
	}
	else if (p->flavor == STATEMENT_COMMENT) {
		skip_line();
		return;
	}
	else if (p->flavor == STATEMENT_DO) {
		bool keep_going = true;
		//evaluate if first statement is true
		p->leaf = expr::evalExpr(p->leaf);
		while (p->leaf->num == 1) {
			int i = 0;
			int size = p->body.size();
			while (i < size) {
				//execute entire body
				evalStatement(p->body[i]);
				i++;
			}
			p->leaf = expr::evalExpr(p->leaf);
		}
		return;
	}
	else if (p->flavor == STATEMENT_OD) {
		return;
	}
	else if (p->flavor == STATEMENT_IF) {
		p->leaf = expr::evalExpr(p->leaf);
		
			int i = 0;
			int size = p->body.size();
			if (p->leaf->num == 1) {
				//want to execute until an else statement
				if (p->leaf->num == 1) {//then our statement is true
					while (i < size) {
						if (p->body[i]->flavor == STATEMENT_ELSE) {
							while (i < size) {//inrement through rest of function to skip the else statements
								i++;
							}
						}
						else {
							evalStatement(p->body[i]);
							i++;
						}
					}
				}
			}
			else {
				while (p->body[i]->flavor != STATEMENT_ELSE) {
					i++;
				}
				while (i < size) {
					evalStatement(p->body[i]);
					i++;
				}
			}

		return;
	}
	else if (p->flavor == STATEMENT_FI) {
		return;
	}
	else if (p->flavor == STATEMENT_ELSE) {
		int i = 0;
		int size = p->body.size();
		while (i < size) {
			evalStatement(p->body[i]);
			i++;
		}
	}
	else {
		//printf("UNDEFINED evalSTATEMENT BEHAVIOR");
		return;
	}

}
#endif 














