#ifndef DEF_AFFICHAGE
#define DEF_AFFICHAGE

#include "parser.h"

static const char* commandToString[] = {"WAIT", "POKE",   "GOTO",
                                        "IF",   "ENGINE", "SHOOT"};

static const char* expressionToString[] = {
    "INT",   "PAR",  "PEEK", "RAND",  "CARDINAL", "SELF",    "SPEED",
    "STATE", "GPSX", "GPSY", "ANGLE", "TARGETX",  "TARGETY", "DISTANCE"};

static const char* operatorToString[] = {"+", "-", "*", "/", "%"};

static const char* comparisonToString[] = {"<", "<=", "=", "<>", ">=", ">"};

void printProgram(program* prog);
void printCommand(command* com);
void printExpression(expression* exp);
void printNumber(uint32_t number);
void printInteger(int32_t integer);
void printCondition(condition* cond);
void printOperator(operator op);
void printComparison(comparison comp);
void printLine(line* l);
int printDebug(const char* format, ...);

#endif