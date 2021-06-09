#include "main.h"

#include <glib.h>
#include <glib/gprintf.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arene.h"
#include "constantes.h"
#include "missile.h"
#include "modele.h"
#include "parser.h"
#include "plateau.h"
#include "robot.h"

gint APS = AFFICHAGE_PAR_SECONDS;
gint CPS = CYCLE_PAR_SECONDS;
gboolean debug = FALSE;
static gchar **scripts;

static GOptionEntry entries[] = {
    {"debug", 'd', 0, G_OPTION_ARG_NONE, &debug, "debug", NULL},
    {"APS", 'a', 0, G_OPTION_ARG_INT, &APS, "affichage par seconde", "APS"},
    {"CPS", 'c', 0, G_OPTION_ARG_INT, &CPS, "cycle par seconde", "CPS"},
    {"script", 's', 0, G_OPTION_ARG_FILENAME_ARRAY, &scripts,
     "scripts des robots", "BLOB"},
    {NULL}};

int main(int argc, char *argv[]) {
  GError *error = NULL;
  GOptionContext *context;
  context = g_option_context_new("- test");
  g_option_context_add_main_entries(context, entries, NULL);

  if (!g_option_context_parse(context, &argc, &argv, &error)) {
    g_print("option parsing failed: %s\n", error->message);
    exit(1);
  }

  if (scripts == NULL || g_strv_length(scripts) != ROBOT_MAX)
    handle_error("nombre de scripts insufisant");

  arene *plat = initArene(scripts);

  plateau(plat);
}