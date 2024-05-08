#include <stdio.h>

int main() {
  for (int i = 1; i < 256; i++) {
    if (i != 0 && !(i % 10)) {
      printf("\n");
    }
    printf("\"%c\",", i);
    if (!(i != 0 && !(i % 10))) {
      printf(" ");
    }
  }

  return 0;
}
