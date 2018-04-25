#include "tempcode.h"
#include <stdio.h>

int tempVars = 0;
int endif = 0;
int endelse = 0;
int endwhile = 0;
int funcs = 0;

/**
 * @brief Function writes tempCode and saves it in a char*.
 * 
 * @param opcode Opcode to create (see enum in header file)
 * @param ret1 Parameter where the returning string is written on
 * @param op1 operand 1 for operation (required/null if not existing)
 * @param op2 operand 2 for operation (required/null if not existing)
 * @param op3 operand 3 for operation (required/null if not existing)
 */
void addCode(int opcode, char** ret1, char* op1, char* op2, char* op3){
	
}