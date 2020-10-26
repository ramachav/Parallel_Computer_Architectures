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
  double *array = (double *)malloc(row_col * sizeof(double *));
  int *map = (int *)malloc(row_col * sizeof(int *));
  ifstream file;
  file.open("matrix.txt");
  for(int i = 0; i < row_col; i++) {
    for(int j = 0; j < row_col; j++) {
      file >> data_matrix[i][j];
    }
    getline(file, current_line);
  }

  int numThreads = (argc == 3)? atoi(argv[2]) : atoi(argv[1]);
  int *flag = (int *)malloc(numThreads * sizeof(int *));
  
  for(int x = 0; x < row_col; x++) 
    map[x] = x % numThreads;
  for(int x = 0; x < numThreads; x++)
    flag[x] = 0;
  
  omp_set_num_threads(numThreads);
  double start_time = omp_get_wtime();
  
  for(int k = 0; k < row_col; k++) {
    //if(k % numThreads == omp_get_thread_num()) {    //k % numThreads
      for(int j = k + 1; j < row_col; j++)
	data_matrix[k][j] = data_matrix[k][j] / data_matrix[k][k];//array[j] = array[j] / array[k];
      data_matrix[k][k] = 1;
      //}
    #pragma omp barrier
  #pragma omp parallel for schedule(static,1)
    for(int i = k + 1; i < row_col; i++) {
      //if(i % numThreads == omp_get_thread_num()) {
	for(int j = k + 1; j < row_col; j++)
	  data_matrix[i][j] = data_matrix[i][j] - (data_matrix[i][k] * data_matrix[k][j]);//array[j]);
	data_matrix[i][k] = 0;
	//map[i] = (map[i] + numThreads - 1) % numThreads;
	//flag[(i + 1) % numThreads] = 1;
	//      }
	//else {
	//while(!flag[omp_get_thread_num()]);
	//flag[omp_get_thread_num()] = 0;
	//}
    }
    //while(map[k + 1] != omp_get_thread_num());
    
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
