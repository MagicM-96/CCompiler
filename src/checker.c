#include "checker.h"

int isTypeCompatible(char* leftVarType, char* rightVarType) {
    if((!strcmp(leftVarType, "INT"))&&(!strcmp("INT", rightVarType))) {
        return 1;
    } else {
        return 0;
    }
}