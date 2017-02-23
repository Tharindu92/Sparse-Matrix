#include <stdio.h>

int* mat;
int* val;
int* col_indx;
int* row_ptr;

int main(){
    //int n = argv[0];
    int n = 6,i;
    mat = (int*) malloc(n * n * sizeof(int));
    //Q1_d_createRandomArray(n);
    mat = [10, 0,0,0,-2,0,3,9,0,0,0,3,0,7,8,7,0,0,3,0,8,7,5,0,0,8,0,9,9,13,0,4,0,0,2,-1];
    Q1_e(n);
    for(i=0; ;i++){
        if(*(row_ptr+i) != -1){
            printf("%d, ",*(row_ptr+i));
        }
        else{
            break;
        }
    }
    printf("\nDone");
}

int Q1_d_createRandomArray(int n){
    int rnd;
    int size = n*n;
    int i;
    for(int i = 0; i < size; i++){
        rnd = rand()%100 + 1;
        if(rnd > 90){
            mat[i] = randon_Number_gen();
        }else{
            mat[i] = 0;
        }
    }
    return 0;
}

int Q1_e(int n){
    int i,j;
    int nnz = 0;
    for(i = 0; i < n; i++){
        *(row_ptr + i) = nnz + 1;
        for(j = 0; j < n; j++){
            if(mat[i*n + j] != 0){
                *(val+nnz) = mat[i*n + j];
                *(col_indx + nnz) = j+1;
                nnz++;
            }
        }
    }
    *(row_ptr + n) = -1;
}

int random_Number_gen(){
    srand(time(NULL));
    return (rand()%20);
}
