#include "arene.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "constantes.h"
#include "expression.h"
#include "main.h"
#include "missile.h"
#include "modele.h"
#include "robot.h"

arene *initArene(char *fichiers[ROBOT_MAX]) {
  arene *a = calloc(1, sizeof(arene));

  a->nb_missile = 0;
  a->l_robot[0] = initRobot(5.0, 5.0, fichiers[0], 0);
  a->l_robot[1] = initRobot(9995.0, 5.0, fichiers[1], 1);
  a->l_robot[2] = initRobot(5.0, 9995.0, fichiers[2], 2);
  a->l_robot[3] = initRobot(9995.0, 9995.0, fichiers[3], 3);

  return a;
}

void cycle_physique(arene *a) {
  for (int i = 0; i < a->nb_missile; i++) {
    miseAJourMissile(a, i);
    if (a->l_missile[i]->aExplose) continue;
    for (int j = 0; j < ROBOT_MAX; j++) {
      collisionRtoM(a, a->l_robot[j], a->l_missile[i]);
    }
  }
  for (int i = 0; i < a->nb_missile; i++) {
    if (!a->l_missile[i]->aExplose) continue;
    missile *m = a->l_missile[i];
    a->l_missile[i] = a->l_missile[--a->nb_missile];
    m->parent->nb_missiles--;
    free(m);
    i--;
  }
  for (int i = 0; i < ROBOT_MAX; i++) {
    miseAJourRobot(a->l_robot[i]);
    for (int j = i + 1; j < ROBOT_MAX; j++) {
      collisionRtoR(a->l_robot[i], a->l_robot[j]);
    }
  }
}

void collisionRtoR(robot *r1, robot *r2) {
  if (abs(r1->position.x - r2->position.x) < SIZE_X &&
      abs(r1->position.y - r2->position.y) < SIZE_Y) {
    degats(r1, COLLISION);
    degats(r2, COLLISION);
    r1->vitesse = 0;
    r2->vitesse = 0;
  }
}

void collisionRtoE(robot *r, missile *m) {
  uint32_t dist =
      distance(r->position.x, r->position.y, m->position.x, m->position.y);
  if (dist <= DIST_50) {
    degats(r, DEGAT_50);
    return;
  }
  if (dist <= DIST_200) {
    degats(r, DEGAT_200);
    return;
  }
  if (dist <= DIST_400) {
    degats(r, DEGAT_400);
    return;
  }
}

void collisionRtoM(arene *a, robot *r, missile *m) {
  if (abs(r->position.x - m->position.x) >= SIZE_X ||
      abs(r->position.y - m->position.y) >= SIZE_Y)
    return;
  if (m->parent == r) return;
  exploseRobots(a, m);
}

void exploseRobots(arene *a, missile *m) {
  for (int i = 0; i < ROBOT_MAX; i++) collisionRtoE(a->l_robot[i], m);
  explose(m);
}
