#include "missile.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "arene.h"
#include "constantes.h"
#include "expression.h"
#include "main.h"
#include "modele.h"
#include "robot.h"

missile *initMissile(robot *parent, int distance, int angle) {
  missile *m = calloc(1, sizeof(missile));
  m->parent = parent;
  m->position = parent->position;
  m->distanceExplosion = distance;
  m->angle = angle;
  m->distance = 0;
  m->aExplose = 0;
  return m;
}

void explose(missile *m) { m->aExplose = 1; }

char RinTrajectory(arene *a, missile *m) {
  for (int i = 0; i < ROBOT_MAX; i++) {
    if (a->l_robot[i] == m->parent ||
        distance(a->l_robot[i]->position.x, a->l_robot[i]->position.y,
                 m->parent->position.x, m->parent->position.y) >
            distance(m->position.x, m->position.y, m->parent->position.x,
                     m->parent->position.y)) {
      continue;
    }
    int x1 = a->l_robot[i]->position.x - m->parent->position.x;
    int y1 = a->l_robot[i]->position.y - m->parent->position.y;
    int x2 = m->position.x - m->parent->position.x;
    int y2 = m->position.y - m->parent->position.y;
    if (x1 * y2 == y1 * x2) {
      m->position = a->l_robot[i]->position;
      exploseRobots(a, m);
      return 1;
    }
  }
  return 0;
}

void miseAJourMissile(arene *a, int i) {
  missile *m = a->l_missile[i];
  int dist = m->distanceExplosion - m->distance < MISSILE_SPEED
                 ? m->distanceExplosion
                 : MISSILE_SPEED;
  dist *= UNITE_COEFF;
  m->distance += dist;
  float x = m->position.x;
  float y = m->position.y;
  m->position.x = TargetX(m->position.x, m->angle, dist);
  m->position.y = TargetY(m->position.y, m->angle, dist);
  if (m->position.x >= X) {
    m->position.x = X;
    m->position.y = (X - x) * (m->position.y - y) / (m->position.x - x);
    exploseRobots(a, m);
    return;
  }
  if (m->position.y >= Y) {
    m->position.x = (Y - y) * (m->position.x - x) / (m->position.y - y);
    m->position.y = Y;
    exploseRobots(a, m);
    return;
  }
  if (m->position.x <= 0) {
    m->position.x = 0;
    m->position.y = x * (m->position.y - y) / (m->position.x - x);
    exploseRobots(a, m);
    return;
  }
  if (m->position.y <= 0) {
    m->position.x = y * (m->position.x - x) / (m->position.y - y);
    m->position.y = 0;
    exploseRobots(a, m);
    return;
  }
  if (RinTrajectory(a, m)) return;
  if (m->distance >= m->distanceExplosion) {
    exploseRobots(a, m);
  }
}
