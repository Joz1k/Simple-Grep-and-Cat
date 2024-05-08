#ifndef S21_CAT
#define S21_CAT

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  bool nonBlank;
  bool dollars;
  bool rowNumbers;
  bool squeezeRows;
  bool tabs;
  bool unprintable;
} ShortOptions;

static const char OPTIONS[] = "beEtnsTv";

static const struct option longOptions[] = {{"number-nonblank", 0, 0, 'b'},
                                            {"number", 0, 0, 'n'},
                                            {"squeeze-blank", 0, 0, 's'},
                                            {0, 0, 0, 0}};

static ShortOptions catFlags(int argc, char *const argv[]);
static void processUnprintable(char currentSym, ShortOptions *opts);
static void stringModification(char lastSymbol, char currentSym,
                               ShortOptions *opts, int *flag);
static void processFiles(FILE *file, char *lastSExists, ShortOptions *opts);
static void catFile(int count, char *const fpath[], ShortOptions *opts);

#endif