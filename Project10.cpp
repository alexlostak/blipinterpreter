//#include "Type_expr.h"
//#include "Vector.h"
//#include "Type_expr.h"
//#include <stdio.h>
#include "statement.h"
#include "Parse.h"

void input(String &input) {
	input = next_token();
}


void run(void) {
	/*//determine statement coming from input file
	read_next_token();
	String statementType;
	input(statementType);
	if (next_token_type == END) {
		return;
	}
	//text statement
	if (statementType == "text") {
		
	}
	//output statement
	else if (statementType == "output") {
       
        run();
	}
	//var statement
	else if (statementType == "var") {
		
		run();
	}
	//set statement
	else if (statementType == "set") {
		
		run();

	}
	else if (statementType == "//") {
		skip_line();
		run();
	}*/
	statement* program = new statement;
	//statement::evalStatement(program);
	//read_next_token();
	int i = 0;
	while (next_token_type != END) {
		program->body.push_back(program->makeStatement());
		program->evalStatement(program->body[i]);
		i++;

	}
	/*int i = 0;
	int size = program->body.size();
	while (i < size) {
		program->evalStatement(program->body[i]);
		i++;
	}*/

	return;
	
}


int main(void) {
	//printf("Test3.blip\n");
	//set_input("test6.blip");
	run();
	//printf("Test Complete");
}