/**
 * Creator: Teemu Hiltunen
 * Date: 30.7.2024
 * Project: 2, my-unzip
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Function definition:
void file_handler(char *filename);
void decoder(FILE *f);

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("my-unzip: file1 [file2 ...]\n");
    exit(1);
  }

  for (int idx = 1; idx < argc; idx++) {
    file_handler(argv[1]);
  }

  return 0;
}

/*
 * Function to open file and sent pointer to decoder for decoding of the
 * contents:
 */
void file_handler(char *filename) {
  FILE *f;
  f = fopen(filename, "r");
  if (!f) {
    printf("Opening file '%s' failed\n", filename);
    exit(1);
  }
  decoder(f);

  fclose(f);
  return;
}

/*
 * Function to decode the contents of a file and print the result to stdout.
 */
void decoder(FILE *f) {
  uint32_t char_count = 0;
  char c;

  // Loop until the end of file is reached (fread returns 0)
  for (;;) {
    if ((fread(&char_count, sizeof(uint32_t), 1, f)) == 0)
      break;

    if ((fread(&c, sizeof(char), 1, f)) == 0)
      break;

    // Print the character char_count amount of times:
    for (size_t idx = 0; idx < char_count; idx++) {
      printf("%c", c);
    }
  }

  return;
}
