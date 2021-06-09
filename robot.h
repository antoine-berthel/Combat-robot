#ifndef DEF_ROBOT
#define DEF_ROBOT

#include <stdint.h>

#include "constantes.h"
#include "modele.h"
#include "parser.h"
#include "position.h"

void nextInstruction(robot *r);  // int si erreur
void miseAJourRobot(robot *r);
void degats(robot *r, float d);
robot *initRobot(float positionX, float positionY, char fichier[], int id);
void destruction(robot *r);

#endif
