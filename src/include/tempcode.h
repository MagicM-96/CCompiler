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
	OPARRAY_ST
};

typedef struct tempCodeString {
	char* line;
	struct tempCodeString* next;
} TEMPCODESTRING;

void addCode();
void addStr(char* str);
void printStr();