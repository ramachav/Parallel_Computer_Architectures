#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
  MPI_Init(NULL, NULL);
  int pid;
  int num_p;
  int row_col = (argc == 2)? atoi(argv[1]) : 1024;
  string current_line;
  double **data_matrix = (double **)malloc(row_col * sizeof(double *));
  for(int i = 0; i < row_col; i++)
    data_matrix[i] = (double *)malloc(row_col * sizeof(double *));
  ifstream file;
  file.open("matrix.txt");
  for(int i = 0; i < row_col; i++) {
    for(int j = 0; j < row_col; j++) {
      file >> data_matrix[i][j];
    }
    getline(file, current_line);
  }

  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &num_p);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  MPI_Barrier(MPI_COMM_WORLD);

  double *array = (double *)malloc((row_col / num_p) * row_col * sizeof(double *));
  double *temp = (double *)malloc(row_col * sizeof(double *));
  long long int index = 0;

  
  //if(pid == 0)
  //printf("\n\nThe Finished Matrix: \n\n");
  MPI_Barrier(MPI_COMM_WORLD);
  
  for(int i = 0; i < row_col; i++) {
    if(i % num_p == pid) {
      for(int j = 0; j < row_col; j++) {
	array[index] = data_matrix[i][j];
	index++;
      }
    }
  }

  double start_time = MPI_Wtime();

  for(int k = 0; k < row_col; k++) {
    if(k % num_p == pid) {
      for(int i = ((k / num_p) * row_col + k + 1); i < ((1 + k / num_p) * row_col); i++) 
	array[i] = array[i] / array[(k / num_p) * row_col + k];
      array[(k / num_p) * row_col + k] = 1;
      int y = 0;
      for(int j = ((k / num_p) * row_col); j < ((1 + k / num_p) * row_col); j++) {
	temp[y] = array[j];
	y++;
	//printf("%lf\t",array[j]);
      }
      //printf("\n");
      MPI_Send(&temp[0],row_col,MPI_DOUBLE,((pid + 1) % num_p),1,MPI_COMM_WORLD);/*&array[(k / num_p) * row_col]*/
    }
    else {
      MPI_Recv(&temp[0],row_col,MPI_DOUBLE,((pid + num_p - 1) % num_p),1,MPI_COMM_WORLD,&status);
      if(k % num_p != (pid + 1) % num_p)
	MPI_Send(&temp[0],row_col,MPI_DOUBLE,((pid + 1) % num_p),1,MPI_COMM_WORLD);
    }
    for(int i = k + 1; i < row_col; i++) {
      if(i % num_p == pid) {
	for(int j = ((i / num_p) * row_col + k + 1); j < ((1 + i / num_p) * row_col); j++)
	  array[j] = array[j] - (array[(i / num_p) * row_col + k] * temp[j % row_col]);
	array[(i / num_p) * row_col + k] = 0;
      }
    }
    //MPI_Barrier(MPI_COMM_WORLD);
  }

  double end_time = MPI_Wtime();
  if(pid == 0)
    printf("\nTime taken to perform Gaussian Elimination on a %dx%d matrix = %lf secs\n",row_col,row_col,(end_time - start_time));
  
  MPI_Finalize();
  return 0;
}
