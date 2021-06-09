#include "plateau.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "affichage.h"
#include "constantes.h"
#include "expression.h"
#include "main.h"
#include "plateau.h"
#include "robot.h"

char isWinner(arene *p) {
  int count = 0;
  for (int i = 0; i < ROBOT_MAX; i++) {
    if (!p->l_robot[i]->mort) continue;
    count++;
  }
  return count >= ROBOT_MAX - 1;
}

void plateau(arene *plateau) {
  WINDOW *vue;
  WINDOW *stats;

  if (!debug) {
    vue = initVue();
    stats = initStats();

    // Start color
    startColor();
  }

  struct timespec delay;
  delay.tv_sec = 0;
  delay.tv_nsec = SLEEP_GAME_NS;

  struct timespec dernier_cycle, actuel_cycle;
  struct timespec dernier_affichage, actuel_affichage;
  clock_gettime(CLOCK_MONOTONIC_RAW, &dernier_cycle);
  clock_gettime(CLOCK_MONOTONIC_RAW, &dernier_affichage);

  while (!isWinner(plateau)) {
    timeout(1);
    int ch = getch();
    if (ch == 'a') {
      end_vue(vue, stats);
      break;
    } else if (ch == 'p') {
      delay.tv_nsec -= 1000000;
    } else if (ch == 'm' && delay.tv_nsec > 1000000) {
      delay.tv_nsec += 1000000;
    }
    noecho();

    clock_gettime(CLOCK_MONOTONIC_RAW, &actuel_cycle);
    if (dernier_cycle.tv_sec * SECOND_TO_NS + dernier_cycle.tv_nsec +
            DELAY_CYCLE_NS <
        actuel_cycle.tv_sec * SECOND_TO_NS + actuel_cycle.tv_nsec) {
      cycle_script(plateau);
      clock_gettime(CLOCK_MONOTONIC_RAW, &dernier_cycle);
    }
    cycle_physique(plateau);
    clock_gettime(CLOCK_MONOTONIC_RAW, &actuel_affichage);
    if (!debug &&
        dernier_affichage.tv_sec * SECOND_TO_NS + dernier_affichage.tv_nsec +
                DELAY_AFFICHAGE_NS <
            actuel_affichage.tv_sec * SECOND_TO_NS + actuel_affichage.tv_nsec) {
      wclear(vue);
      wclear(stats);
      cycle_plateau(vue, stats, plateau);
      clock_gettime(CLOCK_MONOTONIC_RAW, &dernier_affichage);
    }
    nanosleep(&delay, NULL);
  }

  if (!debug) {
    wclear(vue);
    wclear(stats);
    cycle_plateau(vue, stats, plateau);
    mvwprintw(vue, 10, 10, "Felicitation ! Il y'a un gagnant !");
    getch();
    end_vue(vue, stats);
  }
}

WINDOW *initVue() {
  WINDOW *vue;
  initscr();
  vue = subwin(stdscr, LINES, MSIZE_H, 0, 0);
  return vue;
}

WINDOW *initStats() {
  WINDOW *stats;
  initscr();
  stats = subwin(stdscr, LINES, COLS / 4, 0, MSIZE_H);
  return stats;
}

void cycle_plateau(WINDOW *vue, WINDOW *stats, arene *plateau) {
  // Pour gérer la modification de la taille du terminal
  box(vue, ACS_VLINE,
      ACS_HLINE);  // ACS_VLINE et ACS_HLINE sont des constantes
                   // qui génèrent des bordures par défaut
  box(stats, ACS_VLINE,
      ACS_HLINE);  // ACS_VLINE et ACS_HLINE sont des constantes qui génèrent
                   // des bordures par défaut
  // Cycle d'affichage
  add_stats(plateau, stats);
  placer_robot(plateau, vue);
  position_missile(plateau, vue);
}

void startColor() {
  if (has_colors() == FALSE) {
    endwin();
    printDebug("Your terminal does not support color\n");
    exit(1);
  }
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_CYAN, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);
}

void end_vue(WINDOW *vue, WINDOW *stats) {
  getch();
  endwin();
  free(vue);
}

void add_stats(arene *plateau, WINDOW *stats) {
  int couleur = 1;
  int pos = 1;
  for (int i = 0; i < ROBOT_MAX; i++) {
    wattron(stats, COLOR_PAIR(couleur));
    mvwaddch(stats, pos, 1, robot_visu[i]);
    wattroff(stats, COLOR_PAIR(couleur));
    couleur++;
    pos++;
    mvwprintw(stats, pos, 1, "%.5f, %.5f", plateau->l_robot[i]->position.x,
              plateau->l_robot[i]->position.y);
    pos++;
    mvwprintw(stats, pos, 1, "Missiles en cours: %i",
              plateau->l_robot[i]->nb_missiles);
    pos++;
    if (!plateau->l_robot[i]->mort) {
      mvwprintw(stats, pos, 1, "Vie : %f",
                PV_MAX * (1 - plateau->l_robot[i]->degat));
    } else {
      mvwprintw(stats, pos, 1, "Vie : MORT");
    }
    pos += 2;
  }
  wrefresh(stats);
}

void placer_robot(arene *plateau, WINDOW *vue) {
  float echy = (LINES / 10000.0);
  float echx = (MSIZE_H / 10000.0);
  int couleur = 1;

  for (int i = 0; i < ROBOT_MAX; i++) {
    if (!plateau->l_robot[i]->mort) {
      wattron(vue, COLOR_PAIR(couleur));
      robot_mur(plateau->l_robot[i], robot_visu[i], echx, echy, vue);
      wattroff(vue, COLOR_PAIR(couleur));
    } else {
      wattron(vue, COLOR_PAIR(couleur));
      robot_mur(plateau->l_robot[i], 'x', echx, echy, vue);
      wattroff(vue, COLOR_PAIR(couleur));
    }
    couleur++;
  }
}

void position_missile(arene *plateau, WINDOW *vue) {
  float echy = (LINES / 10000.0);
  float echx = (MSIZE_H / 10000.0);
  for (int i = 0; i < plateau->nb_missile; i++) {
    missile *m = plateau->l_missile[i];
    for (int j = 0; j < ROBOT_MAX; j++) {
      if (m->parent == plateau->l_robot[j]) {
        wattron(vue, COLOR_PAIR(j + 1));
        missile_mur(m, echx, echy, vue);
        wattroff(vue, COLOR_PAIR(j + 1));
      }
    }
    wrefresh(vue);
  }
}

void missile_mur(missile *m, float echx, float echy, WINDOW *vue) {
  float x = m->position.x * echx;
  float y = m->position.y * echy;
  if (x < 1) {
    x = 1;
  }
  if (y < 1) {
    y = 1;
  }
  if (x > (MSIZE_H - 2.0)) {
    x = MSIZE_H - 2.0;
  }
  if (y > (LINES - 2.0)) {
    y = LINES - 2.0;
  }
  mvwaddch(vue, y, x, 'o');
  wrefresh(vue);
}

void robot_mur(robot *r, char c, float echx, float echy, WINDOW *vue) {
  float x = r->position.x * echx;
  float y = r->position.y * echy;
  if (x < 1) {
    x = 1;
  }
  if (y < 1) {
    y = 1;
  }
  if (x > (MSIZE_H - 2.0)) {
    x = MSIZE_H - 2.0;
  }
  if (y > (LINES - 2.0)) {
    y = LINES - 2.0;
  }
  mvwaddch(vue, y, x, c);
  wrefresh(vue);
}
