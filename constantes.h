#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES

#ifndef CLOCK_MONOTONIC_RAW
#define CLOCK_MONOTONIC_RAW 4
#endif

#include <glib.h>

extern gint APS;
extern gint CPS;
extern gboolean debug;

#define X 10000
#define Y 10000
#define ROBOT_MAX 4

#define SPEED_ROBOT 500
#define DISTANCE_MAX 7000
#define DIST_400 400
#define DIST_200 200
#define DIST_50 50
#define DEGAT_400 0.03
#define DEGAT_200 0.05
#define DEGAT_50 0.1
#define ADRESSES_MAX 2000
#define PV_MAX 100

#define SIZE_X 10
#define SIZE_Y 10
#define VITESSE_MAX 10
#define MISSILES_MAX 2
#define COLLISION 0.02
#define MAX_LINES 512
#define M_PI 3.14
#define MISSILE_SPEED 500
#define TAILLE_MOT 30

#define SECOND_TO_NS (long)1000000000

#define SLEEP_GAME_NS (long)10000000
#define AFFICHAGE_PAR_SECONDS 60
#define CYCLE_PAR_SECONDS 10
#define DELAY_AFFICHAGE_NS SECOND_TO_NS / APS
#define DELAY_CYCLE_NS SECOND_TO_NS / CPS
#define UNITE_COEFF (float)CPS* SLEEP_GAME_NS / SECOND_TO_NS

#define MSIZE_H (3 * (COLS / 4))

#endif