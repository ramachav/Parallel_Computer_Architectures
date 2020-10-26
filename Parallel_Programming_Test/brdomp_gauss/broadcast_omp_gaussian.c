#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

using namespace std;

int main(int argc, char* argv[] ) {

  string current_line;
  int row_col = (argc == 3)? atoi(argv[1]) : 1024;
  double **data_matrix = (double **)malloc(row_col * sizeof(int *));
  for(int i = 0; i < row_col; i++)
    data_matrix[i] = (double *)malloc(row_col * sizeof(int *));
  ifstream file;
  file.open("matrix.txt");
  for(int i = 0; i < row_col; i++) {
    for(int j = 0; j < row_col; j++) {
      file >> data_matrix[i][j];
    }
    getline(file, current_line);
  }

  int numThreads = (argc == 3)? atoi(argv[2]) : atoi(argv[1]);
  omp_set_num_threads(numThreads);
  double start_time = omp_get_wtime();

  for(int k = 0; k < row_col; k++) {
#pragma omp parallel for
    for(int j = k + 1; j < row_col; j++)
      data_matrix[k][j] = data_matrix[k][j] / data_matrix[k][k];
    data_matrix[k][k] = 1;
#pragma omp parallel for schedule(static,1)
    for(int i = k + 1; i < row_col; i++) {
      for(int j = k + 1; j < row_col; j++)
	data_matrix[i][j] = data_matrix[i][j] - (data_matrix[i][k] * data_matrix[k][j]);
      data_matrix[i][k] = 0;
    }
  }

  double end_time = omp_get_wtime();

  /*printf("\n\nThe Finished Matrix: \n\n");
  for(int x = 0; x < row_col; x++) {
    for(int y = 0; y < row_col; y++)
      printf("%lf\t", data_matrix[x][y]);
    printf("\n");
    }*/
  
  printf("\nTime taken to perform Gaussian Elimination on a %dx%d matrix = %lf secs",row_col,row_col,(end_time - start_time));
  return 0;
}
