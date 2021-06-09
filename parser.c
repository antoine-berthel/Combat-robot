#include "parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "affichage.h"
#include "constantes.h"
#include "main.h"

char isDigit(char name) { return (char)!!isdigit(name); }

char isInteger(char* name) {
  printDebug("isInteger : %s ", name);

  int i = 0;
  if (name[0] == '-') i++;
  for (; i < strlen(name); i++) {
    if (!isDigit(name[i])) {
      printDebug("0\n");
      return 0;
    }
  }

  printDebug("1\n");
  return 1;
}

char isNumber(char* name) {
  printDebug("isNumber : %s ", name);

  for (int i = 0; i < strlen(name); i++) {
    if (!isDigit(name[i])) {
      printDebug("0\n");
      return 0;
    }
  }

  printDebug("1\n");
  return 1;
}

operator isOperator(char* name) {
  printDebug("isOperator : %s ", name);

  if (strlen(name) != 1) return -1;

  if (name[0] == '+') return PLUS;
  if (name[0] == '-') return MINUS;
  if (name[0] == '*') return MULT;
  if (name[0] == '/') return DIV;
  if (name[0] == '%') return MOD;

  printDebug("0\n");
  return -1;
}

comparison isComparison(char* name) {
  printDebug("isComparison : %s ", name);

  if (strcmp(name, "<") == 0) return STRIC_INF;
  if (strcmp(name, "<=") == 0) return INF;
  if (strcmp(name, "=") == 0) return EQ;
  if (strcmp(name, "<>") == 0) return DIFF;
  if (strcmp(name, ">=") == 0) return SUP;
  if (strcmp(name, ">") == 0) return STRICT_SUP;

  printDebug("0\n");
  return -1;
}

expression* isExpression(FILE* f, char* name) {
  printDebug("isExpression : %s ", name);

  expression* expression1 = calloc(1, sizeof(expression));
  char buff[TAILLE_MOT];

  if (isInteger(name)) {
    expression1->type = INT;
    expression1->integer = strtol(name, NULL, 10);

    return expression1;
  }
  if (name[0] == '(') {
    expression1->type = PAR;
    expression1->expression1[0] = isExpression(f, &name[1]);
    if (expression1->expression1 == NULL) return NULL;
    int rewind = -fscanf(f, "%s ", buff);
    if (rewind > 0) return NULL;
    expression1->operator= isOperator(buff);
    if (expression1->operator== - 1) {
      fseek(f, rewind, SEEK_CUR);
      return NULL;
    }
    rewind -= fscanf(f, "%s ", buff);
    if (rewind > 0) return NULL;
    if (buff[strlen(buff) - 1] != ')') {
      fseek(f, rewind, SEEK_CUR);
      return NULL;
    }
    buff[strlen(buff) - 1] = '\0';
    expression1->expression1[1] = isExpression(f, buff);
    if (expression1->expression1[1] == NULL) {
      fseek(f, rewind, SEEK_CUR);
      return NULL;
    }

    return expression1;
  }
  if (strcmp(name, "CARDINAL") == 0 || strcmp(name, "SELF") == 0 ||
      strcmp(name, "SPEED") == 0) {
    if (strcmp(name, "CARDINAL") == 0) expression1->type = CARDINAL;
    if (strcmp(name, "SELF") == 0) expression1->type = SELF;
    if (strcmp(name, "SPEED") == 0) expression1->type = SPEED;

    return expression1;
  }
  if (strcmp(name, "PEEK") == 0 || strcmp(name, "RAND") == 0 ||
      strcmp(name, "STATE") == 0 || strcmp(name, "GPSX") == 0 ||
      strcmp(name, "GPSY") == 0) {
    if (strcmp(name, "PEEK") == 0) expression1->type = PEEK;
    if (strcmp(name, "RAND") == 0) expression1->type = RAND;
    if (strcmp(name, "STATE") == 0) expression1->type = STATE;
    if (strcmp(name, "GPSX") == 0) expression1->type = GPSX;
    if (strcmp(name, "GPSY") == 0) expression1->type = GPSY;

    int rewind = -fscanf(f, "%s ", buff);
    if (rewind > 0) return NULL;
    expression1->expression1[0] = isExpression(f, buff);
    if (expression1->expression1[0] == NULL) {
      fseek(f, rewind, SEEK_CUR);
      return NULL;
    }

    return expression1;
  }
  if (strcmp(name, "TARGETX") == 0 || strcmp(name, "TARGETY") == 0) {
    if (strcmp(name, "TARGETX") == 0) expression1->type = TARGETX;
    if (strcmp(name, "TARGETY") == 0) expression1->type = TARGETY;

    int rewind = 0;
    for (int i = 0; i < 3; i++) {
      rewind -= fscanf(f, "%s ", buff);
      if (rewind > 0) return NULL;
      expression1->expression1[i] = isExpression(f, buff);
      if (expression1->expression1[i] == NULL) {
        fseek(f, rewind, SEEK_CUR);
        return NULL;
      }
    }

    return expression1;
  }
  if (strcmp(name, "ANGLE") == 0 || strcmp(name, "DISTANCE") == 0) {
    if (strcmp(name, "ANGLE") == 0) expression1->type = ANGLE;
    if (strcmp(name, "DISTANCE") == 0) expression1->type = DISTANCE;

    int rewind = 0;
    for (int i = 0; i < 4; i++) {
      rewind -= fscanf(f, "%s ", buff);
      if (rewind > 0) return NULL;
      expression1->expression1[i] = isExpression(f, buff);
      if (expression1->expression1[i] == NULL) {
        fseek(f, rewind, SEEK_CUR);
        return NULL;
      }
    }

    return expression1;
  }

  printDebug("0\n");
  return NULL;
}

condition* isCondition(FILE* f, char* name) {
  printDebug("isCondition : %s ", name);

  condition* condition1 = calloc(1, sizeof(condition));
  char buff[TAILLE_MOT];

  condition1->expression1 = isExpression(f, name);
  if (condition1->expression1 == NULL) return NULL;

  int rewind = -fscanf(f, "%s ", buff);
  if (rewind > 0) return NULL;
  condition1->comparison = isComparison(buff);
  if (condition1->comparison == -1) {
    fseek(f, rewind, SEEK_CUR);
    return NULL;
  }

  rewind -= fscanf(f, "%s ", buff);
  if (rewind > 0) return NULL;
  condition1->expression2 = isExpression(f, buff);
  if (condition1->expression2 == NULL) {
    fseek(f, rewind, SEEK_CUR);
    return NULL;
  }

  printDebug("1\n");
  return condition1;
}

line* isLine(FILE* f, char* name) {
  printDebug("isLine : %s ", name);

  line* line1 = calloc(1, sizeof(line));
  char buff[TAILLE_MOT];

  if (!isNumber(name)) return NULL;
  line1->number = strtoul(name, NULL, 10);

  int rewind = -fscanf(f, "%s ", buff);
  if (rewind > 0) return NULL;
  line1->command = isCommand(f, buff);
  if (line1->command == NULL) {
    fseek(f, rewind, SEEK_CUR);
    return NULL;
  }

  printDebug("1\n");
  return line1;
}

program* isProgram(FILE* f, char* name) {
  printDebug("isProgram : %s ", name);

  char buff[TAILLE_MOT];
  int i = 0;

  line* lines[MAX_LINES];
  lines[0] = isLine(f, name);
  if (lines[0] == NULL || lines[0]->number != 0) return NULL;

  for (i = 1; i < MAX_LINES; i++) {
    int rewind = -fscanf(f, "%s ", buff);
    if (rewind > 0) break;
    lines[i] = isLine(f, buff);
    if (lines[i] == NULL || lines[i]->number != i) {
      printDebug("NULL\n");
      fseek(f, rewind, SEEK_CUR);
      break;
    }
  }
  program* program1 = calloc(1, sizeof(program) + i * sizeof(line*));
  program1->length = i * sizeof(line*);

  memcpy(program1->lines, lines, i * sizeof(line*));

  printDebug("1\n");
  printProgram(program1);
  return program1;
}

command* isCommand(FILE* f, char* name) {
  printDebug("isCommand : %s ", name);

  command* command1 = calloc(1, sizeof(command));
  char buff[TAILLE_MOT];
  if (strcmp(name, "WAIT") == 0) {
    command1->type = WAIT;

    int rewind = -fscanf(f, "%s ", buff);
    if (rewind > 0) return NULL;
    command1->expression1 = isExpression(f, buff);
    if (command1->expression1 == NULL) {
      fseek(f, rewind, SEEK_CUR);
      return NULL;
    }

    return command1;
  }
  if (strcmp(name, "POKE") == 0 || strcmp(name, "ENGINE") == 0 ||
      strcmp(name, "SHOOT") == 0) {
    if (strcmp(name, "POKE") == 0) command1->type = POKE;
    if (strcmp(name, "ENGINE") == 0) command1->type = ENGINE;
    if (strcmp(name, "SHOOT") == 0) command1->type = SHOOT;

    int rewind = -fscanf(f, "%s ", buff);
    if (rewind > 0) return NULL;
    command1->expression1 = isExpression(f, buff);
    if (command1->expression1 == NULL) {
      fseek(f, rewind, SEEK_CUR);
      return NULL;
    }
    rewind -= fscanf(f, "%s ", buff);
    if (rewind > 0) return NULL;
    command1->expression2 = isExpression(f, buff);
    if (command1->expression2 == NULL) {
      fseek(f, rewind, SEEK_CUR);
      return NULL;
    }

    return command1;
  }
  if (strcmp(name, "GOTO") == 0) {
    command1->type = GOTO;

    int rewind = -fscanf(f, "%s ", buff);
    if (rewind > 0) return NULL;
    if (!isNumber(buff)) {
      fseek(f, rewind, SEEK_CUR);
      return NULL;
    }
    command1->number = strtoul(buff, NULL, 10);

    return command1;
  }
  if (strcmp(name, "IF") == 0) {
    command1->type = IF;

    int rewind = -fscanf(f, "%s ", buff);
    if (rewind > 0) return NULL;
    command1->condition = isCondition(f, buff);
    if (command1->condition == NULL) {
      fseek(f, rewind, SEEK_CUR);
      return NULL;
    }

    rewind -= fscanf(f, "%s ", buff);
    if (rewind > 0) return NULL;
    if (strcmp(buff, "THEN") != 0) {
      fseek(f, rewind, SEEK_CUR);
      return NULL;
    }

    rewind -= fscanf(f, "%s ", buff);
    if (rewind > 0) return NULL;
    if (!isNumber(buff)) {
      fseek(f, rewind, SEEK_CUR);
      return NULL;
    }
    command1->number = strtoul(buff, NULL, 10);

    return command1;
  }
  return NULL;
}

program* parser(char nom[]) {
  FILE* fichier = fopen(nom, "r");
  if (fichier == NULL) handle_error("fopen");

  char buff[TAILLE_MOT] = "";
  if (fscanf(fichier, "%s ", buff) < 0) handle_error("fichier vide");
  program* p = isProgram(fichier, buff);
  fclose(fichier);
  if (p == NULL) handle_error("fichier non conforme");
  return p;
}
