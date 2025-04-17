/**
 * Creator: Teemu Hiltunen
 * Date: 9.5.2023
 * Project: 1
 */

// C libraries:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct which is used in Linked list and holds pointers to previous and
// following items in
//  the linked list, this way we can easily print the contents of linked list in
//  reversed order:
typedef struct line {
  char *line;
  struct line *pPrevious;
  struct line *pNext;
} LINE;

/**
 * Function declarations
 */
LINE *alloLinkedItem(LINE *pAlloc);
LINE *readToLinkedList(FILE *stream, LINE *pStart);
LINE *fileOperations(char *pFilename, LINE *pStart, char *pOperation);
void printToFileStreamInReversedOrder(FILE *stream, LINE *pStart);
LINE *freeLinkedList(LINE *pToFree);

int main(int argc, char *argv[]) {
  // Checking wether the given arguments of filenames are same
  // if they are, the program stops

  if (argc == 3) {
    // Check whether the actual filenames match
    char *in_fname = strchr(argv[1], '/');
    char *out_fname = strchr(argv[2], '/');

    // Use original given filename if only filename is provided without path
    // (eg. / is not found as strchr() returns a pointer to first occurence of
    // character '/' )
    in_fname = (in_fname) ? in_fname + 1 : argv[1];
    out_fname = (out_fname) ? out_fname + 1 : argv[2];

    if (strcmp(in_fname, out_fname) == 0) {
      fprintf(stderr, "reverse: input and output file must differ\n");
      exit(1);
    }
  }

  LINE *pStart = NULL;

  switch (argc) {
  case 1: // 0 input arguments
    // User input from stdin is read to linked list:
    pStart = readToLinkedList(stdin, pStart);
    // After the user gives empty line, the program moves to
    // printing the user given input to stdout:
    printToFileStreamInReversedOrder(stdout, pStart);
    break;
  case 2: // Only input file to be read, print to stdout
    // Contents of file is read to linked list:
    pStart = fileOperations(argv[1], pStart, "r");

    // Contents is printed to stdout in reversed order:
    printToFileStreamInReversedOrder(stdout, pStart);
    break;
  case 3: // 2 arguments given, input to be read and output to be written
    // Contents of file is read to linked list:
    pStart = fileOperations(argv[1], pStart, "r");
    // Contents is written to file in reversed order:
    pStart = fileOperations(argv[2], pStart, "w");
    break;

  default: // Check on wether the user gave too many arguments
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
    break;
  }

  // Freeing the linked list
  pStart = freeLinkedList(pStart);

  exit(0);
}

/**
 * Function definitions
 */

// Function to allocate memory space for one item of the Linked List
LINE *alloLinkedItem(LINE *pAlloc) {
  if ((pAlloc = (LINE *)malloc(sizeof(LINE))) == NULL) {
    fprintf(stderr, "malloc failed\n");
    exit(1);
  }
  return pAlloc;
}

// File opening and closing operation is handled in one function
//   and based on parameters given to this function, the file is either read or
//   written.
LINE *readToLinkedList(FILE *stream, LINE *pStart) {
  LINE *pEnd = NULL;

  // As per man pages, initializing the line pointer to NULL will give the
  // getline -function ability to allocate enough memory for the line to be
  // stored.
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  while ((nread = getline(&line, &len, stream)) != -1) {
    // This condition is used to stop getting input from user and start printing
    // to stdout in case of 0 arguments given. This implementation version might
    // NOT be the best but
    //  the given assignment didn't specify clearly enough at which point
    //  the user given input needs to be printed back to the stdout.
    // So I decided to use newline as a turning point.
    if (stream == stdin) {
      if (nread == 1) {
        break;
      }
    }

    // Memory for one item of the linked list allocated
    LINE *pNew = alloLinkedItem(pNew);
    // Setting the pointer to next item to NULL to indicate the end of linked
    // list
    pNew->pNext = NULL;
    // Allocating enough memory for the line to be stored inside the linked list
    // item char array.
    //      We are not allowed to assume the MAXIMUM size of the line (getline
    //      will allocate it)
    if ((pNew->line = (char *)malloc(nread + 1)) == NULL) {
      fprintf(stderr, "malloc failed\n");
      exit(1);
    }
    // The line is copied to the linked list item variable
    strcpy(pNew->line, line);

    // The linked list item is added to the END of linked list
    if (pStart == NULL) // When the linked list is empty
    {
      pNew->pPrevious = NULL;
      pStart = pNew;
      pEnd = pNew;
    } else // pNew is added to the END of linked list
    {
      // pNew's pointer to previous is set to current end
      pNew->pPrevious = pEnd;
      // Current ends' pNext is set to point to the pNew
      pEnd->pNext = pNew;
      // and finally the pEnd is set to pNew
      pEnd = pNew;
    }
  }
  // As per man pages, after getline function has allocated memory, it should
  // also be freed
  //  getline will automatically resize the space accordingly inside the loop
  free(line);

  return pStart;
}

// This function handles all the file opening / closing tasks and based on
// pOperation parameter,
//  appropriate case is executed.
LINE *fileOperations(char *pFilename, LINE *pStart, char *pOperation) {
  FILE *stream;
  if ((stream = fopen(pFilename, pOperation)) == NULL) {
    fprintf(stderr, "reverse: cannot open file '%s'\n", pFilename);
    exit(1);
  }
  // different operations defined here:
  switch (*pOperation) { // Case of reading the given file:
  case 'r':
    pStart = readToLinkedList(stream, pStart);
    break;
  case 'w': // Case of writing the file.
    printToFileStreamInReversedOrder(stream, pStart);
    break;
  }

  fclose(stream);
  return pStart;
}

// Function to print information to the specified stream in reversed order:
void printToFileStreamInReversedOrder(FILE *stream, LINE *pStart) {
  // Helper variable for linked list walkthrough
  LINE *ptr = pStart;

  // Here we can get to the end of the Linked list,
  //   linked lists head and tail could have been stored in a struct and only
  //   struct to be passed and then just traverse the linked list from tail to
  //   head
  while (ptr->pNext != NULL) {
    ptr = ptr->pNext;
  }

  while (ptr != NULL) {
    fprintf(stream, "%s", ptr->line);
    ptr = ptr->pPrevious;
  }

  return;
}

// Function to free complete linked list
LINE *freeLinkedList(LINE *pToFree) {
  LINE *ptr = pToFree;
  while (ptr != NULL) {
    // We save the pointer to next item in linked list
    pToFree = ptr->pNext;

    // Then we free all the mallocated variables from linked list item if there
    // are any:
    free(ptr->line);

    // and then we free the item itself
    free(ptr);

    // Finally we move to the next item in the linked list
    ptr = pToFree;
  }

  pToFree = NULL;
  return pToFree;
}

// End of File
