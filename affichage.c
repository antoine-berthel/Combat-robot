#include "affichage.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constantes.h"
#include "main.h"
#include "parser.h"

void printProgram(program* prog) {
  if (prog == NULL) {
    printDebug("Ceci n'est pas valide");
    return;
  }
  for (int i = 0; i < prog->length / sizeof(line*); i++) {
    printLine(prog->lines[i]);
  }
}

void printCommand(command* com) {
  printDebug("%s ", commandToString[com->type]);
  switch (com->type) {
    case WAIT:
      printExpression(com->expression1);
      break;
    case GOTO:
      printNumber(com->number);
      break;
    case IF:
      printCondition(com->condition);
      printDebug("THEN ");
      printNumber(com->number);
      break;
    case POKE:
    case ENGINE:
    case SHOOT:
      printExpression(com->expression1);
      printExpression(com->expression2);
      break;
    default:
      break;
  }
}

void printExpression(expression* exp) {
  printDebug("%s ", expressionToString[exp->type]);
  switch (exp->type) {
    case INT:
      printInteger(exp->integer);
      break;
    case PAR:
      printExpression(exp->expression1[0]);
      printOperator(exp->operator);
      printExpression(exp->expression1[1]);
      break;
    case ANGLE:
    case DISTANCE:
      for (int i = 0; i < 4; i++) printExpression(exp->expression1[i]);
      break;
    case TARGETX:
    case TARGETY:
      for (int i = 0; i < 3; i++) printExpression(exp->expression1[i]);
      break;
    case PEEK:
    case RAND:
    case STATE:
    case GPSX:
    case GPSY:
      printExpression(exp->expression1[0]);
      break;
    case CARDINAL:
    case SELF:
    case SPEED:
      break;
    default:
      break;
  }
}

void printNumber(uint32_t number) { printDebug("%u ", number); }

void printInteger(int32_t integer) { printDebug("%i ", integer); }

void printCondition(condition* cond) {
  printExpression(cond->expression1);
  printComparison(cond->comparison);
  printExpression(cond->expression2);
}

void printOperator(operator op) { printDebug("%s ", operatorToString[op]); }

void printComparison(comparison comp) {
  printDebug("%s ", comparisonToString[comp]);
}

void printLine(line* l) {
  printNumber(l->number);
  printCommand(l->command);
  printDebug("\n");
}

int printDebug(const char* format, ...) {
  if (!debug) return 0;
  va_list vl;
  va_start(vl, format);
  int ret = vprintf(format, vl);
  va_end(vl);
  return ret;
}
