#include "mainwindow.h"
#ifndef MAIN_H
#define MAIN_H
#ifdef MAIN
  #define EXTERN
#else
  #define EXTERN extern
#endif
EXTERN MainWindow *mainwin;
#endif // MAIN_H