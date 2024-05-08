#ifndef S21_GREP
#define S21_GREP

#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char** data;
  size_t curSize;
  size_t maxSize;
  regex_t* regularExpressions;
} Pattern;

typedef struct {
  bool patt;                 // e
  bool ignore;               // i
  bool invertion;            // v
  bool matchedlineCount;     // c
  bool matchedFiles;         // l
  bool rowNumber;            // n
  bool matchedWithoutNames;  // h
  bool suppressed;           // s
  bool getPatternFromFile;   // f
  bool nonHollow;            // o
  Pattern pattern;
} ShortOptions;

static const char OPTIONS[] = "e:ivclnhsf:o";

static void* safeMalloc(size_t size);
static void* safeRealloc(void* patt, size_t size);
static void processPatternFromString(ShortOptions* opts, char* myString);
static void processPatternFromFile(ShortOptions* opts, char* filename);
static ShortOptions grepFlags(int argc, char* const argv[]);
static bool compare(char* filesString, ShortOptions* opts, regmatch_t* pmatch);
static void grepNonHollow(FILE* file, ShortOptions* opts, int countFiles,
                          char* filename[]);
static void matchedFilenames(FILE* file, ShortOptions* opts, char* filename[]);
static void countMatchedLines(FILE* file, ShortOptions* opts, int countFiles,
                              char* filename[]);
static void processFiles(FILE* file, ShortOptions* opts, int countFiles,
                         char* filename[]);
static void grepFile(int count, char* fpath[], ShortOptions* opts);
static void grepPattern(ShortOptions* opts);

#endif