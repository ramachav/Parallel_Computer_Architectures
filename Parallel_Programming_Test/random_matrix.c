#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

  int row = atoi(argv[1]);
  int col = atoi(argv[2]);
  int **array = (int **)malloc(row * sizeof(int *));

  for(int i = 0; i < row; i++) {
    array[i] = (int *)malloc(col * sizeof(int *));
    for(int j = 0; j < col; j++) {
      array[i][j] = (rand() % 10) + 1;
      printf("%d ",array[i][j]);
    }
    printf("\n");
  }
  return 0;
}
