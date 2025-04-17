/**
 * Creator: Teemu Hiltunen
 * Date: 27.7.2024
 * Project: 2, my-zip
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Function definitions
char *file_to_buffer(char *buffer, char *filename, long *total_size);
void encoder(char *buffer);

int main(int argc, char *argv[]) {

  if (argc <= 1) {
    printf("my-zip: file1 [file2 ...]\n");
    exit(1);
  }
  // buffer to which file content is stored and its size:
  char *buffer = NULL;
  long total_size = 0;

  // Buffer all of the files to memory:
  for (size_t idx = 1; idx < argc; idx++) {
    buffer = file_to_buffer(buffer, argv[idx], &total_size);
  }

  // RL-encode the buffer:
  encoder(buffer);

  // Free and NULL the buffer pointer
  free(buffer);
  buffer = NULL;

  return 0;
}

/*
 * Function to buffer a files contents to heap memory and to be encoded later.
 *
 * Src used with the buffering of multiple files:
 * https://stackoverflow.com/questions/2029103/correct-way-to-read-a-text-file-into-a-buffer-in-c
 *
 */
char *file_to_buffer(char *buffer, char *filename, long *total_size) {
  FILE *f = NULL;
  char *n_ptr = NULL;

  if ((f = fopen(filename, "r")) == NULL) {
    printf("my-zip: could not open file\n");
    exit(1);
  }

  // Go to the end of file
  if (fseek(f, 0L, SEEK_END) == 0) {
    // Check the size
    long buf_size = ftell(f);

    if (buf_size == -1) {
      perror("Buffer size error:  ");
      exit(1);
    }

    // Realloacte to new size with slot for terminator
    n_ptr = realloc(buffer, (*total_size + buf_size + 1));

    if (n_ptr == NULL) {
      perror("Could not allocate: ");
      exit(1);
    }

    // Seek back to beginning of file
    if (fseek(f, 0L, SEEK_SET) != 0) {
      perror("Seek to beginning failed: ");
      exit(1);
    }

    // Set the original pointer to new location
    buffer = n_ptr;

    // Read files contents to increased buffer
    fread(buffer + *total_size, sizeof(char), buf_size, f);

    if (ferror(f) != 0) {
      fputs("Could not read file", stderr);
      exit(1);
    }

    // Update the total_size of buffer
    *total_size += buf_size;

    // Terminate
    buffer[*total_size] = '\0';
  }

  // Close file
  fclose(f);
  return buffer;
}

/*
 * Function to print to stdout stream the buffer contents via Run Length
 * Encoding.
 */
void encoder(char *buffer) {
  // Set the first character as previous
  char prev = buffer[0];

  // As the count of numbers can be 4 bytes long, it results to unsigned int of
  // 32 bits as the count can not be below zero
  uint32_t count = 0;

  // Since we added terminator, we can iterate the buffer easily
  for (char *ptr = buffer; *ptr != '\0'; ptr++) {
    if (prev != *ptr) {
      // Print the binary and ascii
      fwrite(&count, sizeof(count), 1, stdout);
      printf("%c", prev);

      // Update for new character
      prev = *ptr;
      count = 1;
    } else // Count same char
    {
      count++;
    }
  }

  // Last print
  fwrite(&count, sizeof(count), 1, stdout);
  printf("%c", prev);

  return;
}
