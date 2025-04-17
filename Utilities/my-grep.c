/**
 * Creator: Teemu Hiltunen
 * Date: 29.7.2024
 * Project: 2, my-grep
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fileHandler(char filename[], char term[]);
void searcher(FILE *f, char term[]);

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("my-grep: searchterm [file ...]\n");
    exit(1);
  }

  if (argc == 2) {
    searcher(stdin, argv[1]);
  }

  // The second argument in argv is the first file to be read
  int i = 2;

  while (i < argc) {
    // The search term is found in the argv[1]
    fileHandler(argv[i], argv[1]);
    i++;
  }

  return 0;
}

// If one of the files can not be opened the program exits with stdout,
// otherwise the searchterm is searched from the file
void fileHandler(char filename[], char term[]) {
  FILE *f;

  if ((f = fopen(filename, "r")) == NULL) {
    printf("my-grep: cannot open file\n");
    exit(1);
  }

  searcher(f, term);

  fclose(f);

  return;
}

// Each line is read with getline and then parsed for term with strstr
// function.
void searcher(FILE *f, char term[]) {
  char *line = NULL;
  size_t buf = 0;
  while ((getline(&line, &buf, f)) > 0) {
    if ((strstr(line, term)) != NULL) {
      printf("%s", line);
    }
    free(line);
    line = NULL;
  }

  free(line);
  return;
}
