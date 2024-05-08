#include "cat.h"

static ShortOptions catFlags(int argc, char *const argv[]) {
  int c;
  ShortOptions shortOptions = {0};
  while ((c = getopt_long(argc, argv, OPTIONS, longOptions, 0)) != -1) {
    switch (c) {
      case 'b':
        shortOptions.nonBlank = true;
        break;
      case 'e':
        shortOptions.dollars = true;
        shortOptions.unprintable = true;
        break;
      case 'E':
        shortOptions.dollars = true;
        break;
      case 'n':
        shortOptions.rowNumbers = true;
        break;
      case 's':
        shortOptions.squeezeRows = true;
        break;
      case 't':
        shortOptions.tabs = true;
        shortOptions.unprintable = true;
        break;
      case 'T':
        shortOptions.tabs = true;
        break;
      case 'v':
        shortOptions.unprintable = true;
        break;
      default:
        printf("Try 'cat --help' for more information.\n");
        exit(1);
        break;
    }
  }
  if (shortOptions.nonBlank) {
    shortOptions.rowNumbers = false;
  }
  return shortOptions;
}

static void processUnprintable(char currentSym, ShortOptions *opts) {
  if (opts->unprintable) {
    putc('^', stdout);
    if (currentSym == 127) {
      putc(currentSym - 64, stdout);
    } else {
      putc(currentSym + 64, stdout);
    }
  } else {
    putc(currentSym, stdout);
  }
}

static void stringModification(char lastSymbol, char currentSym,
                               ShortOptions *opts, int *flag) {
  static int count = 0;
  if (opts->rowNumbers && lastSymbol == '\n' &&
      ((*flag == 1 || (*flag == 0 && count == 0)))) {
    printf("%6d\t", ++count);
  } else if (opts->nonBlank && lastSymbol == '\n' && currentSym != '\n' &&
             (*flag == 1 || (*flag == 0 && count == 0))) {
    printf("%6d\t", ++count);
  }
  if (*flag == 0) {
    *flag = 1;
  }
  if (opts->dollars && currentSym == '\n') {
    putc('$', stdout);
  }
  if (currentSym >= 32 && currentSym <= 126) {
    putc(currentSym, stdout);
  } else if (currentSym == '\n') {
    putc(currentSym, stdout);
  } else if (currentSym == '\t') {
    if (opts->tabs) {
      putc('^', stdout);
      putc('I', stdout);
    } else {
      putc(currentSym, stdout);
    }
  } else if ((currentSym < 32 && currentSym >= 0) || currentSym == 127) {
    processUnprintable(currentSym, opts);
  } else {
    if (opts->unprintable) {
      char bizzareSym = (signed char)currentSym + 128;
      putc('M', stdout);
      putc('-', stdout);
      if (bizzareSym >= 32 && bizzareSym <= 126) {
        putc(bizzareSym, stdout);
      } else {
        processUnprintable(bizzareSym, opts);
      }
    } else {
      putc(currentSym, stdout);
    }
  }
}

static void processFiles(FILE *file, char *lastSExists, ShortOptions *opts) {
  size_t spaceCounter = 1;
  int flag = 0;
  if (*lastSExists == '\n') {
    flag = 1;
  }
  char lastS = '\n';
  char currS = fgetc(file);
  while (!feof(file)) {
    if (opts->squeezeRows) {
      spaceCounter = (currS == '\n') ? spaceCounter + 1 : 0;
    }
    if (currS != '\n' || spaceCounter < 3) {
      stringModification(lastS, currS, opts, &flag);
    }
    lastS = currS;
    currS = fgetc(file);
  }
  *lastSExists = lastS;
}

static void catFile(int count, char *const fpath[], ShortOptions *opts) {
  FILE *file = NULL;
  static char lastSExists = '\0';
  while (count > 0) {
    file = fopen(*fpath, "r");
    if (file == NULL) {
      printf("s21_cat: %s: No such file or directory\n", *fpath);
    } else {
      processFiles(file, &lastSExists, opts);
      fflush(stdout);
      fclose(file);
    }
    ++fpath;
    --count;
  }
}

int main(int argc, char *const argv[]) {
  ShortOptions opt = catFlags(argc, argv);
  catFile(argc - optind, argv + optind, &opt);
  return 0;
}