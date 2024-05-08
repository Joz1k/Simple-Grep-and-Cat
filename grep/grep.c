#include "grep.h"

static void *safeMalloc(size_t size) {
  void *temp;
  temp = malloc(size);
  if (temp == NULL) {
    printf("Ошибка памяти");
    exit(1);
  }
  return temp;
}

static void *safeRealloc(void *patt, size_t size) {
  void *temp;
  temp = realloc(patt, size);
  if (temp == NULL) {
    printf("Ошибка памяти");
    exit(1);
  }
  return temp;
}

static void processPatternFromString(ShortOptions *opts, char *myString) {
  char *temp = safeMalloc(sizeof(char) * strlen(myString) + sizeof(char));
  strcpy(temp, myString);
  char *changed = strtok(temp, "\n");
  while (changed != NULL) {
    if (opts->pattern.curSize == opts->pattern.maxSize) {
      opts->pattern.maxSize += 128;
      opts->pattern.data = safeRealloc(opts->pattern.data,
                                       opts->pattern.maxSize * sizeof(char *));
    }
    opts->pattern.data[opts->pattern.curSize] =
        safeMalloc(sizeof(char) * strlen(changed) + sizeof(char));
    strcpy(opts->pattern.data[opts->pattern.curSize++], changed);
    changed = strtok(NULL, "\n");
  }
  free(temp);
}

static void processPatternFromFile(ShortOptions *opts, char *filename) {
  FILE *file = fopen(filename, "r");
  if (file != NULL) {
    size_t size = 0;
    size_t maxSize = opts->pattern.maxSize;
    char *buffer = safeMalloc(sizeof(char) * opts->pattern.maxSize);
    char s = fgetc(file);
    while (!feof(file)) {
      buffer[size++] = s;
      s = fgetc(file);
      if (size == maxSize) {
        maxSize += opts->pattern.maxSize;
        buffer = safeRealloc(buffer, maxSize * sizeof(char));
      }
    }
    buffer[size] = '\0';
    processPatternFromString(opts, buffer);
    free(buffer);
    fclose(file);
  } else {
    printf("s21_grep: %s: No such file or directory\n", filename);
    free(opts->pattern.data);
    exit(1);
  }
}

static ShortOptions grepFlags(int argc, char *const argv[]) {
  int c;
  int optionIndex = 0;
  ShortOptions shortOptions = {0};
  shortOptions.pattern.curSize = 0;
  shortOptions.pattern.maxSize = 128;
  shortOptions.pattern.data =
      safeMalloc(sizeof(char *) * shortOptions.pattern.maxSize);
  static const struct option longOptions[] = {0};
  while ((c = getopt_long(argc, argv, OPTIONS, longOptions, &optionIndex)) !=
         -1) {
    switch (c) {
      case 'e':
        shortOptions.patt = true;
        processPatternFromString(&shortOptions, optarg);
        break;
      case 'i':
        shortOptions.ignore = true;
        break;
      case 'v':
        shortOptions.invertion = true;
        break;
      case 'c':
        shortOptions.matchedlineCount = true;
        break;
      case 'l':
        shortOptions.matchedFiles = true;
        break;
      case 'n':
        shortOptions.rowNumber = true;
        break;
      case 'h':
        shortOptions.matchedWithoutNames = true;
        break;
      case 's':
        shortOptions.suppressed = true;
        break;
      case 'f':
        shortOptions.getPatternFromFile = true;
        processPatternFromFile(&shortOptions, optarg);
        break;
      case 'o':
        shortOptions.nonHollow = true;
        break;
      default:
        printf("Try 'cat --help' for more information.\n");
        free(shortOptions.pattern.data);
        exit(1);
        break;
    }
  }
  if (!shortOptions.patt && !shortOptions.getPatternFromFile) {
    processPatternFromString(&shortOptions, argv[optind++]);
  }
  return shortOptions;
}

static bool compare(char *filesString, ShortOptions *opts, regmatch_t *pmatch) {
  bool res = false;
  size_t nmatch;
  if (pmatch == NULL) {
    nmatch = 0;
  } else {
    nmatch = 1;
  }
  for (size_t i = 0; i < opts->pattern.curSize; i++) {
    if (regexec(&opts->pattern.regularExpressions[i], filesString, nmatch,
                pmatch, 0) == 0) {
      res = true;
      break;
    }
  }
  if (opts->invertion) {
    res = res == true ? false : true;
    if (!opts->matchedlineCount && !opts->matchedFiles) {
      res = opts->nonHollow == true ? false : res;
    }
  }
  return res;
}

static void grepNonHollow(FILE *file, ShortOptions *opts, int countFiles,
                          char *filename[]) {
  int count = 0;
  char *filesString = safeMalloc(sizeof(char) * opts->pattern.maxSize);
  size_t stringLength = opts->pattern.maxSize;
  regmatch_t pmatch = {0};
  while (getline(&filesString, &stringLength, file) > 0) {
    char *remain = filesString;
    count++;
    while (compare(remain, opts, &pmatch) == true) {
      if (!opts->matchedWithoutNames && countFiles > 1 && opts->rowNumber) {
        printf("%s:%d:%.*s\n", *filename, count,
               (int)(pmatch.rm_eo - pmatch.rm_so), remain + pmatch.rm_so);
      } else if (!opts->matchedWithoutNames && countFiles > 1 &&
                 !opts->rowNumber) {
        printf("%s:%.*s\n", *filename, (int)(pmatch.rm_eo - pmatch.rm_so),
               remain + pmatch.rm_so);
      } else if ((opts->matchedWithoutNames || countFiles <= 1) &&
                 opts->rowNumber) {
        printf("%d:%.*s\n", count, (int)(pmatch.rm_eo - pmatch.rm_so),
               remain + pmatch.rm_so);
      } else {
        printf("%.*s\n", (int)(pmatch.rm_eo - pmatch.rm_so),
               remain + pmatch.rm_so);
      }
      remain = remain + pmatch.rm_eo;
    }
  }
  free(filesString);
}

static void matchedFilenames(FILE *file, ShortOptions *opts, char *filename[]) {
  char *filesString = safeMalloc(sizeof(char) * opts->pattern.maxSize);
  size_t stringLength = opts->pattern.maxSize;
  while (getline(&filesString, &stringLength, file) != EOF) {
    if (compare(filesString, opts, NULL) == true) {
      printf("%s\n", *filename);
      break;
    }
  }
  free(filesString);
}

static void countMatchedLines(FILE *file, ShortOptions *opts, int countFiles,
                              char *filename[]) {
  int count = 0;
  char *filesString = safeMalloc(sizeof(char) * opts->pattern.maxSize);
  size_t stringLength = opts->pattern.maxSize;
  while (getline(&filesString, &stringLength, file) > 0) {
    if (compare(filesString, opts, NULL) == true) {
      count++;
    }
  }
  if (countFiles > 1 && !opts->matchedWithoutNames) {
    printf("%s:%d\n", *filename, count);
  } else {
    printf("%d\n", count);
  }
  free(filesString);
}

static void processFiles(FILE *file, ShortOptions *opts, int countFiles,
                         char *filename[]) {
  int count = 0;
  char *filesString = safeMalloc(sizeof(char) * opts->pattern.maxSize);
  size_t stringLength = opts->pattern.maxSize;
  regmatch_t pmatch;
  static int endOfFile = 0;
  endOfFile++;
  char prevChar = 127;
  while (getline(&filesString, &stringLength, file) > 0) {
    count++;
    if (compare(filesString, opts, &pmatch) == true) {
      if (!opts->matchedWithoutNames && countFiles > 1 && opts->rowNumber) {
        printf("%s:%d:%s", *filename, count, filesString);
      } else if (!opts->matchedWithoutNames && countFiles > 1 &&
                 !opts->rowNumber) {
        printf("%s:%s", *filename, filesString);
      } else if ((opts->matchedWithoutNames || countFiles <= 1) &&
                 opts->rowNumber) {
        printf("%d:%s", count, filesString);
      } else {
        printf("%s", filesString);
      }
      prevChar = filesString[strlen(filesString) - 1];
    }
  }
  if (endOfFile <= countFiles && prevChar != '\n') {
    printf("\n");
  }
  free(filesString);
}

static void grepFile(int count, char *fpath[], ShortOptions *opts) {
  FILE *file = NULL;
  int countFiles = 0, countTemp = count;
  char **fpathTemp = fpath;
  while (countTemp > 0) {
    file = fopen(*fpathTemp, "r");
    fpathTemp++;
    countTemp--;
    if (file != NULL) {
      countFiles++;
      fflush(stdout);
      fclose(file);
    }
  }
  while (count > 0) {
    file = fopen(*fpath, "r");
    if (file == NULL) {
      if (!opts->suppressed) {
        printf("s21_grep: %s: No such file or directory\n", *fpath);
      }
    } else {
      if (opts->matchedFiles) {
        matchedFilenames(file, opts, fpath);
      } else if (opts->matchedlineCount) {
        countMatchedLines(file, opts, countFiles, fpath);
      } else if (opts->nonHollow) {
        grepNonHollow(file, opts, countFiles, fpath);
      } else {
        processFiles(file, opts, countFiles, fpath);
      }
      fflush(stdout);
      fclose(file);
    }
    ++fpath;
    --count;
  }
}

static void grepPattern(ShortOptions *opts) {
  int caseIndifference = opts->ignore ? REG_ICASE : 0;
  opts->pattern.regularExpressions =
      safeMalloc(sizeof(regex_t) * opts->pattern.curSize);
  for (size_t i = 0; i < opts->pattern.curSize; i++) {
    regcomp(&opts->pattern.regularExpressions[i], opts->pattern.data[i],
            caseIndifference);
  }
}

int main(int argc, char *argv[]) {
  ShortOptions opt = grepFlags(argc, argv);
  grepPattern(&opt);
  if (argc - optind <= 0) {
    printf("Pattern, please\n");

  } else {
    grepFile(argc - optind, argv + optind, &opt);
  }
  for (size_t i = 0; i < opt.pattern.curSize; i++) {
    free(opt.pattern.data[i]);
    regfree(&opt.pattern.regularExpressions[i]);
  }
  free(opt.pattern.data);
  free(opt.pattern.regularExpressions);
  return 0;
}
