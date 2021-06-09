#ifndef DEF_MISSILE
#define DEF_MISSILE

#include "arene.h"
#include "modele.h"
#include "position.h"
#include "robot.h"

missile *initMissile(robot *parent, int distance, int angle);
void miseAJourMissile(arene *a, int i);
void explose(missile *m);

#endif