#ifndef DEF_MODELE
#define DEF_MODELE

#include "parser.h"
#include "stdint.h"

typedef struct {
  float x;
  float y;
} pos;

typedef struct {
  pos position;
  float vitesse;
  float degat;
  int nb_missiles;
  int angle;
  char mort;
  program *script;
  int id;
  int32_t wait;
  uint32_t adresses[ADRESSES_MAX];
  uint32_t ligne;
} robot;

typedef struct {
  robot *parent;
  pos position;
  int distance;
  int distanceExplosion;
  int angle;
  char aExplose;
} missile;

typedef struct {
  robot *l_robot[ROBOT_MAX];
  missile *l_missile[ROBOT_MAX * MISSILES_MAX];
  int nb_missile;
} arene;

#endif