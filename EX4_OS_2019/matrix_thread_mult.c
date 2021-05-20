//Name : Abdallah Natsheh
//ID : 

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <pthread.h>
static int MATRIX_DIM=3;
static int **RESULT_MAT;
static int counter=0;



void Fill_Matrix(int ***mtx)
{
    int num=0;
for(int i=0; i<MATRIX_DIM; i++) {
      for(int j=0;j<MATRIX_DIM;j++) {
         scanf("%d",&num);
         (*mtx)[i][j]=num;
      }
   }
}

void FillMatrixWithZeros(int ***mat )
{
 *mat = (int **)malloc(sizeof(int *)*MATRIX_DIM);
   for(int i = 0; i< MATRIX_DIM; i++) {
        (*mat)[i] = (int *)malloc(sizeof(int) *MATRIX_DIM);
        for(int j = 0; j < MATRIX_DIM; j++) {
            (*mat)[i][j] = -1;
        }
   }
}

int multI_J(int **mtx1,int **mtx2,int MATRIX_DIM,int i,int j) //calcultes the number in position i,j in the result matrix
{
   int k,num=0;
    for(k=0;k<MATRIX_DIM;k++)
        num+=mtx1[i][k]*mtx2[k][j];
    return num;
}

void Print_Matrix(int **mtx )
{
 for(int i=0; i<MATRIX_DIM; i++) {
      for(int j=0;j<MATRIX_DIM;j++) {
         printf("%d ", mtx[i][j]);
         }
      printf("\n");
      }
   }

struct matrix
{
    int **mtx1;
    int **mtx2;
    int **resultMatrix;
    int matrix_dim;
    int i; //coulmn
};

void* multI_J2(void *m) // thread  begin control in this function
{  
    struct matrix *m2;
    m2=m;
    m2->i=counter;
   int k=0;
   for(k=0;k<m2->matrix_dim;k++){
   RESULT_MAT[m2->i][k]=multI_J(m2->mtx1,m2->mtx2,m2->matrix_dim,m2->i,k);}
    counter++;
	pthread_exit(0); //Exit the thread
} 

void freeArray(int **array )// FREE THE ARRAY
{ 
   int i;
    for(i=0;i<MATRIX_DIM;i++)
    {
        free(array[i]);
    }
    free(array);
}

 int main() 
 {
   printf("Enter enter MATRIX_DIM (number between 1 and 10):\n");
   int num1=0;
   scanf("%d",&num1);
   MATRIX_DIM=num1;

   struct matrix m1;
   m1.i=0;
   m1.matrix_dim=MATRIX_DIM;

  FillMatrixWithZeros(&RESULT_MAT);
  FillMatrixWithZeros(&m1.mtx1);    
  FillMatrixWithZeros(&m1.mtx2);
  printf("Enter elements of first matrix\n");
   Fill_Matrix(&m1.mtx1);
 //Displaying array elements
  printf("Enter elements of second matrix\n");
  Fill_Matrix(&m1.mtx2);

 //CREATING THREADS!
pthread_t tid[MATRIX_DIM]; //Thread ID
	for(int j=0;j<MATRIX_DIM;j++) 
      	{
		 if(pthread_create(&tid[j],NULL,multI_J2,(void*)&m1)<0) //Create the thread
		 {	
		 	perror("thread create error");
		 	exit(1);  
		 } 
	}
	for(int j=0;j<MATRIX_DIM;j++)
		pthread_join(tid[j], NULL); //make sure the parent waits for all thread to complete

  printf("Product of the matrices:\n");
  Print_Matrix(RESULT_MAT);
freeArray(m1.mtx1);
freeArray(m1.mtx2);
freeArray(RESULT_MAT);

  return 0;
  }








