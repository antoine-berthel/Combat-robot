#ifndef DEF_MAIN
#define DEF_MAIN

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0)

#endif
