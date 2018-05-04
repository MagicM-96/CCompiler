#pragma once

enum {
	OPASSIGN,
	OPADD,
	OPSUB,
	OPMUL,
	OPDIV,
	OPMINUS,
	OPIFEQ,
	OPIFNE,
	OPIFGT,
	OPIFGE,
	OPIFLT,
	OPIFLE,
	OPGOTO,
	OPRETURNR,
	OPRETURN,
	OPCALLR,
	OPCALL,
	OPARRAY_LD,
	OPARRAY_ST,
	OPIFOR,
	OPIFAND,
	CREATEVAR
};

void addCode(int opcode, char** ret1, char* op1, char* op2, char* op3);
void createVar(char* id, char* type,char** ret);
void loadNum(int val, char** ret);
void addStr(char* str);
int isVariable(char* var, char** ret);