#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sys/time.h>

using namespace std;

int main(int argc, char* argv[] ) {

  struct timeval start_time, end_time;
  string current_line;
  int row_col = (argc == 2)? atoi(argv[1]) : 1024;
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
  gettimeofday(&start_time, NULL);
  //long double start_time = (long double)(tv.tv_sec) + (long double)(tv.tv_usec) / 1000000;
  
  for(int k = 0; k < row_col; k++) {
    for(int j = k + 1; j < row_col; j++)
      data_matrix[k][j] = data_matrix[k][j] / data_matrix[k][k];
    data_matrix[k][k] = 1;
    for(int i = k + 1; i < row_col; i++) {
      for(int j = k + 1; j < row_col; j++)
	data_matrix[i][j] = data_matrix[i][j] - (data_matrix[i][k] * data_matrix[k][j]);
      data_matrix[i][k] = 0;
    }
  }

  gettimeofday(&end_time, NULL);
  //long double end_time = (long double)(tv.tv_sec) + (long double)(tv.tv_usec) / 1000000;
  double seconds = (double)(end_time.tv_sec - start_time.tv_sec) + (double)(end_time.tv_usec - start_time.tv_usec) / 1000000;
  
  /*cout<<"\n\nThe Finished Matrix: \n\n";
  for(int x = 0; x < row_col; x++) {
    for(int y = 0; y < row_col; y++)
      printf("%lf\t", data_matrix[x][y]);
    printf("\n");
    }*/
  printf("\nTime taken to perform Gaussian Elimination on a %dx%d matrix = %lf secs",row_col,row_col,seconds);
  return 0;
}
