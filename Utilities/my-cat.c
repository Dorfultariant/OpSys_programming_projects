/**
 * Creator: Teemu Hiltunen
 * Date: 30.7.2024
 * Project: 2 my-cat
 */

#include <stdio.h>
#include <stdlib.h>

#define BUFFER 4096

/*
    Function fileHandler is responsible for opening and closing a file
    printer is only responsible for printing the files contents in preferred
   form
*/
void fileHandler(char filename[]);
void printer(FILE *f);

int main(int argc, char *argv[]) {
  if (argc == 1) {
    exit(0);
  }

  int i = 1;

  do {
    fileHandler(argv[i]);
    i++;
  } while (i < argc);

  return 0;
}

// If one of the files can not be opened the program exits with stdout,
// otherwise the printer is used to print the lines
void fileHandler(char filename[]) {
  FILE *f;

  if ((f = fopen(filename, "r")) == NULL) {
    printf("my-cat: cannot open file\n");
    exit(1);
  }
  printer(f);
  fclose(f);
  return;
}

void printer(FILE *f) {
  char line[BUFFER];
  while (fgets(line, BUFFER - 1, f) != NULL) {
    printf("%s", line);
  }
  return;
}
