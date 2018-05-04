#pragma once
#include "uthash.h"

typedef struct paramstruct
{
	int paramNr;
	int size;
	char* type;
	char* name;
	struct paramstruct* next;
} STRUCTPARAM;

typedef struct varstruct
{
	char* id;
	char* type;
	int size;
	int value;
	UT_hash_handle hh;
} STRUCTVAR;

typedef struct funcstruct
{
	char* id;
	char* type;		  // INT="1"; VOID="2"
	int paramcount;
	int isDefined;
	STRUCTPARAM* funcparams;	   // array
	STRUCTVAR* funcvars;
	UT_hash_handle hh;
} STRUCTFUNC;

typedef struct scopestack
{
	STRUCTVAR* scope;
	struct scopestack* next;
} SCOPESTACK;

typedef struct errorLineInfo
{	
	int firstLine;
	int firstColumn;
	int lastLine;
	int lastColumn;
} ERRORLINEINFO;

typedef struct vartype 
{
	char* type;
	char* var;
} TYPEVAR;

typedef struct tempCodeString {
	char* line;
	struct tempCodeString* next;
} TEMPCODESTRING;

typedef struct Vars{
    char* tempVar[4];
    char* varName;
    struct Vars* next;
}TEMPVARS;