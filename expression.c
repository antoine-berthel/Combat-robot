#include "expression.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "affichage.h"
#include "arene.h"
#include "constantes.h"
#include "parser.h"
#include "robot.h"

int32_t par(int32_t exp1, operator op, int32_t exp2) {
  switch (op) {
    case PLUS:
      return exp1 + exp2;
    case MINUS:
      return exp1 - exp2;
    case MULT:
      return exp1 * exp2;
    case DIV:
      return exp1 / exp2;
    case MOD:
      return exp1 % exp2;
    default:
      return 0;
  }
}

int32_t peek(robot *r, int32_t exp) { return r->adresses[exp]; }

int32_t aleat(int32_t exp) { return rand() % exp; }

int32_t cardinal() { return ROBOT_MAX; }

int32_t self(robot *r) { return r->id; }

int32_t speed(robot *r) { return r->vitesse; }

int32_t state(arene *a, int32_t exp) { return PV_MAX - a->l_robot[exp]->degat; }

int32_t gpsx(arene *a, int32_t exp) { return a->l_robot[exp]->position.x - 5; }

int32_t gpsy(arene *a, int32_t exp) { return a->l_robot[exp]->position.y - 5; }

int32_t angle(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  return atan((y2 - y1) - (x2 - x1)) * 180 / M_PI;
}

float TargetX(float x1, int32_t angle, float length) {
  return x1 + length * cos(angle * M_PI / 180);
}

float TargetY(float y1, int32_t angle, float length) {
  return y1 + length * sin(angle * M_PI / 180);
}

float distance(float x1, float y1, float x2, float y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

char conditions(arene *a, robot *r, condition *condition) {
  int32_t exp1 = Expression(a, r, condition->expression1);
  int32_t exp2 = Expression(a, r, condition->expression2);
  switch (condition->comparison) {
    case STRIC_INF:
      return exp1 < exp2;
    case INF:
      return exp1 <= exp2;
    case EQ:
      return exp1 == exp2;
    case DIFF:
      return exp1 != exp2;
    case SUP:
      return exp1 >= exp2;
    case STRICT_SUP:
      return exp1 > exp2;
    default:
      return 0;
  }
}

void Wait(robot *r, int32_t delay) {
  if (delay < 0) return;
  r->wait = delay;
}

void Goto(robot *r, uint32_t nb) { r->ligne = nb; }

void poke(robot *r, int32_t add, int32_t value) { r->adresses[add] = value; }

void ifThen(robot *r, char condition, uint32_t number) {
  if (condition) {
    Goto(r, number);
  }
}

void Engine(robot *r, uint32_t angle, uint32_t speed) {
  r->vitesse = speed;
  r->angle = angle;
}

void Shoot(arene *a, robot *r, uint32_t angle, uint32_t distance) {
  if (a->nb_missile >= ROBOT_MAX * MISSILES_MAX ||
      r->nb_missiles >= MISSILES_MAX)
    return;
  missile *m = initMissile(r, distance, angle);
  a->l_missile[a->nb_missile] = m;
  a->nb_missile += 1;
  r->nb_missiles += 1;
}

void Command(arene *a, robot *r, command *c) {
  switch (c->type) {
    case WAIT:
      Wait(r, Expression(a, r, c->expression1));
      break;
    case GOTO:
      Goto(r, c->number);
      break;
    case IF:
      ifThen(r, conditions(a, r, c->condition), c->number);
      break;
    case POKE:
      poke(r, Expression(a, r, c->expression1),
           Expression(a, r, c->expression2));
      break;
    case ENGINE:
      Engine(r, Expression(a, r, c->expression1),
             Expression(a, r, c->expression2));
      break;
    case SHOOT:
      Shoot(a, r, Expression(a, r, c->expression1),
            Expression(a, r, c->expression2));
      break;
    default:
      break;
  }
}

uint32_t Expression(arene *a, robot *r, expression *exp) {
  switch (exp->type) {
    case INT:
      return exp->integer;
      break;
    case PAR:
      return par(Expression(a, r, exp->expression1[0]), exp->operator,
                 Expression(a, r, exp->expression1[1]));
    case PEEK:
      return peek(r, Expression(a, r, exp->expression1[0]));
    case RAND:
      return aleat(Expression(a, r, exp->expression1[0]));
    case CARDINAL:
      return cardinal();
    case SELF:
      return self(r);
    case SPEED:
      return speed(r);
    case STATE:
      return state(a, Expression(a, r, exp->expression1[0]));
    case GPSX:
      return gpsx(a, Expression(a, r, exp->expression1[0]));
    case GPSY:
      return gpsy(a, Expression(a, r, exp->expression1[0]));
    case ANGLE:
      return angle(Expression(a, r, exp->expression1[0]),
                   Expression(a, r, exp->expression1[1]),
                   Expression(a, r, exp->expression1[2]),
                   Expression(a, r, exp->expression1[3]));
    case TARGETX:
      return TargetX(Expression(a, r, exp->expression1[0]),
                     Expression(a, r, exp->expression1[1]),
                     Expression(a, r, exp->expression1[2]));
    case TARGETY:
      return TargetY(Expression(a, r, exp->expression1[0]),
                     Expression(a, r, exp->expression1[1]),
                     Expression(a, r, exp->expression1[2]));
    case DISTANCE:
      return distance(Expression(a, r, exp->expression1[0]),
                      Expression(a, r, exp->expression1[1]),
                      Expression(a, r, exp->expression1[2]),
                      Expression(a, r, exp->expression1[3]));
    default:
      return 0;
  }
}

void cycle_script(arene *a) {
  for (int i = 0; i < ROBOT_MAX; i++) {
    robot *r = a->l_robot[i];

    if (r->ligne >= r->script->length / sizeof(line *)) {
      r->mort = 1;
      continue;
    }
    if (r->mort) {
      continue;
    }
    if (r->wait > 0) {
      r->wait--;
      continue;
    }
    command *c = r->script->lines[r->ligne++]->command;
    printDebug("robot : %i\n", i);
    printCommand(c);
    printDebug("\n");
    Command(a, r, c);
  }
  printDebug("\n");
}