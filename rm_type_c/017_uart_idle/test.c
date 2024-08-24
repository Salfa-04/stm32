#include <stdint.h>
#include <stdio.h>

void read(void**);

int main(void) {
  // FILE* fp = NULL;

  // printf("HELLO, WORLD!: %p\n", fp);

  // read((void**)&fp);

  // printf("HELLO, WORLD!: %p\n", fp);

  // for (int i = 0; i < 16; i++) {
  //   char t = getc(fp);
  //   putchar(t);
  // }

  // fclose(fp);

  uint16_t i = 7;
  while (i--) printf("HELLO, WORLD!: %d\n", i);

  printf("HELLO, WORLD : %d\n", i);

  return 0;
}

void read(void** ref) {
  FILE* file = fopen("CMakeLists.txt", "rt");
  if (file == NULL) printf("Wrong!\n");
  *ref = file;
}
