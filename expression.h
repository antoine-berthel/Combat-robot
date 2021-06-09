#ifndef DEF_EXPRESSION
#define DEF_EXPRESSION

#include <stdint.h>

#include "arene.h"
#include "parser.h"
#include "robot.h"

int32_t par(int32_t exp1, operator op, int32_t exp2);
int32_t peek(robot *r, int32_t exp);
int32_t aleat(int32_t exp);
int32_t cardinal();
int32_t self();
int32_t speed();
int32_t state(arene *a, int32_t exp);
int32_t gpsx(arene *a, int32_t exp);
int32_t gpsy(arene *a, int32_t exp);
int32_t angle(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
float TargetX(float x1, int32_t angle, float length);
float TargetY(float y1, int32_t angle, float length);
float distance(float x1, float y1, float x2, float y2);
void Wait(robot *r, int32_t delay);
char conditions(arene *a, robot *r, condition *condition);
void poke(robot *r, int32_t add, int32_t value);
void ifThen(robot *r, char condition, uint32_t number);
void Engine(robot *r, uint32_t angle, uint32_t speed);
void Shoot(arene *a, robot *r, uint32_t angle, uint32_t distance);
void Command(arene *a, robot *r, command *c);
uint32_t Expression(arene *a, robot *r, expression *exp);
void cycle_script(arene *a);
#endif