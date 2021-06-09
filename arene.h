#ifndef DEF_ARENE
#define DEF_ARENE

#include "constantes.h"
#include "missile.h"
#include "modele.h"
#include "robot.h"

arene *initArene(char *fichiers[ROBOT_MAX]);
void cycle_physique(arene *a);
void collisionRtoR(robot *r1, robot *r2);
void collisionRtoE(robot *r, missile *m);
void collisionRtoM(arene *a, robot *r, missile *m);
void exploseRobots(arene *a, missile *m);

#endif
