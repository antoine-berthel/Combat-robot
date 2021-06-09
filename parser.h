#ifndef DEF_PARSER
#define DEF_PARSER

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constantes.h"

typedef enum { PLUS, MINUS, MULT, DIV, MOD } operator;

typedef enum { STRIC_INF, INF, EQ, DIFF, SUP, STRICT_SUP } comparison;

typedef enum {
  INT,
  PAR,
  PEEK,
  RAND,
  CARDINAL,
  SELF,
  SPEED,
  STATE,
  GPSX,
  GPSY,
  ANGLE,
  TARGETX,
  TARGETY,
  DISTANCE
} type_expression;

typedef enum { WAIT, POKE, GOTO, IF, ENGINE, SHOOT } type_command;

typedef struct expression {
  type_expression type;
  int32_t integer;
  operator operator;
  struct expression* expression1[4];
} expression;

typedef struct {
  expression* expression1;
  comparison comparison;
  expression* expression2;
} condition;

typedef struct {
  type_command type;
  expression* expression1;
  expression* expression2;
  condition* condition;
  uint32_t number;
} command;

typedef struct {
  uint32_t number;
  command* command;
} line;

typedef struct {
  size_t length;
  line* lines[];
} program;

char isDigit(char name);
char isInteger(char* name);
char isNumber(char* name);
operator isOperator(char* name);
comparison isComparaison(char* name);
expression* isExpression(FILE* f, char* name);
condition* isCondition(FILE* f, char* name);
line* isLine(FILE* f, char* name);
program* isProgram(FILE* f, char* name);
condition* isCondition(FILE* f, char* name);
command* isCommand(FILE* f, char* name);

program* parser(char nom[]);

#endif