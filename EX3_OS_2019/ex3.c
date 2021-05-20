

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <signal.h>
static int MATRIX_DIM=3;
#define SIGCHLD 17

int multI_J(int mtx1[MATRIX_DIM][MATRIX_DIM],int mtx2[MATRIX_DIM][MATRIX_DIM],int i,int j) //calcultes the number in position i,j in the result matrix
{
int k,num=0;
for(k=0;k<MATRIX_DIM;k++)
num+=mtx1[i][k]*mtx2[k][j];
return num;
}

int multI_J2(int mtx1[MATRIX_DIM][MATRIX_DIM],int mtx2[MATRIX_DIM][MATRIX_DIM],int mtx3[MATRIX_DIM][MATRIX_DIM],int i) //calcultes the number in position i,j in the result matrix
{
int k=0;
for(k=0;k<MATRIX_DIM;k++){
mtx3[i][k]=multI_J(mtx1,mtx2,i,k);
}
}

void Fill_Matrix(int mtx [MATRIX_DIM][MATRIX_DIM]) //used to fill the matrix with the numbers you want :)
{
for(int i=0; i<MATRIX_DIM; i++) {
for(int j=0;j<MATRIX_DIM;j++) {
scanf("%d", &mtx[i][j]);
  }
 }
}

void FillMatrixWithZeros(int mtx [MATRIX_DIM][MATRIX_DIM])//fill matrix with zeros
{
for(int i=0; i<MATRIX_DIM; i++) {
for(int j=0;j<MATRIX_DIM;j++) {
mtx[i][j]=0;
}
}
}

void Print_Matrix(int  mtx [MATRIX_DIM][MATRIX_DIM]) //print matrix
{
for(int i=0; i<MATRIX_DIM; i++) {
for(int j=0;j<MATRIX_DIM;j++) {
printf("%d ", mtx[i][j]);
if(j==MATRIX_DIM-1){
printf("\n");
            }
        }
    }
}
void sig_handler(int sig)

{
if (sig == SIGCHLD)//SIGCHILD
{
signal(SIGCHLD, sig_handler);
wait(NULL);
}
}

int main()
{
printf("Enter enter MATRIX_DIM (number between 1 and 10)\n");
int num1=0;
scanf("%d",&num1);
if(num1 > 10){printf("error enter number between 1 and 10 "); exit(0);}
MATRIX_DIM=num1;

int mtx1 [MATRIX_DIM][MATRIX_DIM];
int mtx2 [MATRIX_DIM][MATRIX_DIM];
int mtx5 [MATRIX_DIM][MATRIX_DIM];
int mtx6 [MATRIX_DIM][MATRIX_DIM];
int mtx7 [MATRIX_DIM][MATRIX_DIM];

 printf("Enter elements of first matrix\n");
 Fill_Matrix(&mtx1);
//Displaying array elements

printf("\n");
 printf("Enter elements of second matrix\n");
Fill_Matrix(&mtx2);


printf("\n");
//


mtx5[0][0]=1;
mtx5[0][1]=2;
mtx5[0][2]=3;
mtx5[1][0]=4;
mtx5[1][1]=5;
mtx5[1][2]=6;
mtx5[2][0]=7;
mtx5[2][1]=8;
mtx5[2][2]=9;

mtx6[0][0]=1;
mtx6[0][1]=2;
mtx6[0][2]=3;
mtx6[1][0]=4;
mtx6[1][1]=5;
mtx6[1][2]=6;
mtx6[2][0]=7;
mtx6[2][1]=8;
mtx6[2][2]=9;

mtx7[0][0]=1;
mtx7[0][1]=2;
mtx7[0][2]=3;
mtx7[1][0]=4;
mtx7[1][1]=5;
mtx7[1][2]=6;
mtx7[2][0]=7;
mtx7[2][1]=8;
mtx7[2][2]=9;


printf("\n");

printf("\n");

//Creating 2d Array(Shared Memory array)
key_t key2;
int shmBuf1id;

int (*resultMatrix)[MATRIX_DIM]  ;
key2 = ftok("/temp",'b');
shmBuf1id = shmget(key2,sizeof(int[MATRIX_DIM][MATRIX_DIM]),IPC_CREAT|0666);

if(shmBuf1id == -1 )
{
perror("shmget");
exit(1);
}
else
{
resultMatrix = shmat(shmBuf1id,0,0);
if(resultMatrix == (void*) -1 )
{
perror("shmat");
exit(1);
}
}
int _resultMatrix  [MATRIX_DIM][MATRIX_DIM];
//Creating Normal Array
/* declarations */
int shm_id;
key_t key ;
int *ptr;//The array which be shared using the shared memory.
size_t SHM_SIZE = sizeof(int)*MATRIX_DIM;
    /* ftok */
key=ftok("/temp",'y');
    /* shmget() create shm and retotalNumbersturn the shm id*/
if ((shm_id = shmget(key,sizeof(int[MATRIX_DIM]),IPC_CREAT|0666))==-1)
{ perror("failed to get memory...");exit(1);}
/* shmat() return pointer to shm */
if ((ptr = (int *)shmat(shm_id, NULL, 0)) <0)
{perror("failed to attach memory..."); exit(1);}

FillMatrixWithZeros(&resultMatrix);
for(int i=0; i<MATRIX_DIM; ++i)
        for(int j=0; j<MATRIX_DIM; ++j)
        {
            _resultMatrix[i][j] = 0;
        }
for(int i=0;i<MATRIX_DIM;i++)//Getting input  
{
ptr[i]=0;
}
//multI_J2(&mtx5,&mtx6,&resultMatrix,i);// works without son
for(int i=0; i<MATRIX_DIM; ++i)
        for(int j=0; j<MATRIX_DIM; ++j)
            for( int k=0; k<MATRIX_DIM; ++k)
            {
                _resultMatrix[i][j]+=mtx1[i][k]*mtx2[k][j];
            }

for(int i=0;i<MATRIX_DIM;i++) // loop will run MATRIX_DIM times 
{
if(fork() == 0)
{

multI_J2(&mtx5,&mtx6,&resultMatrix,i);
ptr[i]=1;

exit(0);
    }

        }

int x=0;
while(1)
{
usleep(10000);
if(ptr[x]==1)
x++;
if(x==MATRIX_DIM-1)
{
    break;
}
}


printf("Product of the matrices\n");
Print_Matrix(&_resultMatrix);
return 0;
}