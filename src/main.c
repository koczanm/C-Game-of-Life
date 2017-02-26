#include <stdio.h>
#include "bmp_reader.h"
                                                   /* +2 dla ramki żeby if'ami się nie bawić */
#define HEIGHT 27                                  /* wysokość                               */
#define WIDTH 40                                   /* szerokość                              */
#define NUMBER_OF_GENERATIONS 1000                 /* chyba za długa nazwa                   */

void Init(int grid[HEIGHT][WIDTH], char *in);      /* ładuje wejście           */
void NextGen(int grid[HEIGHT][WIDTH], int cState); /* tworzy kolejną generację */
void Print(int grid[HEIGHT][WIDTH], int cState);   /* wyświetla tablicę        */
void Simulate(int grid[HEIGHT][WIDTH]);            /* rozpoczyna symulacje     */

static char *usage = "./life <input>";

int main(int argc, char *argv[]) {
  /* glider - szybowiec */
  /*
  char *in = "01000"
             "00100"
             "11100";
  */
  char *in;
  FILE *fn;
  
  if (argc > 1) {
    fn = fopen(argv[1], "rb");
    if (fn != NULL) {
      in = ReadBMP(fn);
    } else {
      fprintf(stderr, "%s: Cannot open file: %s\n", argv[0], argv[1]);
	  return -2;
    }
  } else {
    fprintf(stderr, "%s: Too few arguments\n", argv[0]);
	printf("%s\n", usage);
	return -1;
  }
  /* glider gun */
  /*
  char *in = "000000000000000000000000000000000000000000000000000"
             "000000000000000000000000010000000000000000000000000"
             "000000000000000000000001010000000000000000000000000"
             "000000000000011000000110000000000001100000000000000"
             "000000000000100010000110000000000001100000000000000"
             "011000000001000001000110000000000000000000000000000"
             "011000000001000101100001010000000000000000000000000"
             "000000000001000001000000010000000000000000000000000"
             "000000000000100010000000000000000000000000000000000"
             "00000000000001100000000000000000000000000000000000";
  */

  int grid[HEIGHT][WIDTH];
  Init(grid, in);

  Simulate(grid);

  return 0;
}

void Init(int grid[HEIGHT][WIDTH], char *in) {
  int i, j, k = 0;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      if (in[k] != '\0' && i != 0 && j != 0
          && i != HEIGHT- 1 && j != WIDTH - 1) {
        grid[i][j] = in[k++] - '0';
      } else {
        grid[i][j] = 0;
      }
    }
  }
}

void NextGen(int grid[HEIGHT][WIDTH], int cState) {
  int i, j, k, l;
  for (i = 1; i < HEIGHT - 1; i++) {
    for (j = 1; j < WIDTH - 1; j++) {
      int neighbours = 0;
      for (k = -1; k <= 1; k++) {
        for (l = -1; l <= 1; l++) {                                       /* zlicza sąsiadów */
          if ((k != 0 || l != 0) &&                                       /*    [?][?][?]    */
              (grid[i + k][j + l] == cState || grid[i + k][j + l] > 2)) { /*    [?][x][?]    */
            neighbours++;                                                 /*    [?][?][?]    */
          }
        }
      }
      if ((grid[i][j] == cState && (neighbours == 2 || neighbours == 3)) /* żywa && (2 || 3 sąsiadów) = żywa */
          || (grid[i][j] == 0 && neighbours == 3)) {                     /* martwa && 3 sąsiadów      = żywa */
        if (grid[i][j] == 0) {
          grid[i][j] = (cState == 1) ? 2 : 1;
        } else {
          grid[i][j] = 3; /* żywa teraz i wcześniej */
        }
      }
      else if (grid[i][j] == cState) {
        grid[i][j] = 4; /* martwa teraz i żywa wcześniej */
      }
    }
  }
}

void Print(int grid[HEIGHT][WIDTH], int cState) {
  int i, j;
  for (i = 1; i < HEIGHT - 1; i++) {
    for (j = 1; j < WIDTH - 1; j++) {
      if (grid[i][j] == cState || grid[i][j] == 3) {
        grid[i][j] = cState;
        printf("o");
      } else {
        grid[i][j] = 0;
        printf(".");
      }
    }
    printf("\n");
  }
}

void Simulate(int grid[HEIGHT][WIDTH]) {
  int cState = 1; /* obecna tablica */

  int i;
  for (i = 0; i < NUMBER_OF_GENERATIONS; i++) {
    Print(grid, cState);

    NextGen(grid, cState);

    cState = (cState == 1) ? 2 : 1;

    
    getchar();
  }
}