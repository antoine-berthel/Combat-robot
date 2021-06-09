
#include "robot.h"

#include <stdio.h>

#include "constantes.h"
#include "expression.h"
#include "main.h"
#include "math.h"
#include "modele.h"

robot *initRobot(float positionX, float positionY, char fichier[], int id) {
  robot *r = calloc(1, sizeof(robot));
  r->degat = 0;
  r->mort = 0;
  r->vitesse = 0;
  r->nb_missiles = 0;
  r->angle = 0;
  r->position.x = positionX;
  r->position.y = positionY;

  r->script = parser(fichier);
  r->id = id;
  r->ligne = 0;
  r->wait = 0;
  return r;
}

void destruction(robot *r) {
  if (r->degat < 1) return;
  r->mort = 1;
}

void degats(robot *r, float d) {
  if (r->mort) return;
  r->degat += d;
  destruction(r);
}

void miseAJourRobot(robot *r) {
  if (r->mort) return;
  r->position.x = TargetX(r->position.x, r->angle,
                          r->vitesse / 100 * VITESSE_MAX * UNITE_COEFF);
  r->position.y = TargetY(r->position.y, r->angle,
                          r->vitesse / 100 * VITESSE_MAX * UNITE_COEFF);
  if (r->position.x + SIZE_X / 2 > X) {
    r->position.x = X - 5;
    degats(r, COLLISION);
    return;
  }
  if (r->position.y + SIZE_Y / 2 > Y) {
    r->position.y = Y - 5;
    degats(r, COLLISION);
    return;
  }
  if (r->position.x - SIZE_X / 2 < 0) {
    r->position.x = 5;
    degats(r, COLLISION);
    return;
  }
  if (r->position.y - SIZE_Y / 2 < 0) {
    r->position.y = 5;
    degats(r, COLLISION);
  }
}