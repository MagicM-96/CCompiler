#pragma once

enum {
	ASSIGN,
	ADD,
	SUB,
	MUL,
	DIV,
	MINUS,
	IFEQ,
	IFNE,
	IFGT,
	IFGE,
	IFLT,
	IFLE,
	GOTO,
	RETURNR,
	RETURN,
	CALLR,
	CALL,
	ARRAY_LD,
	ARRAY_ST
};

void addCode();