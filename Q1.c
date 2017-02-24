#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int mat[36] = {10, 0,0,0,-2,0,3,9,0,0,0,3,0,7,8,7,0,0,3,0,8,7,5,0,0,8,0,9,9,3,0,4,0,0,2,-1};
int mat2[36] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int* ans_mat;

int* val;           int* val2;
int* col_indx;      int* col_indx2;
int* row_ptr;       int* row_ptr2;

int* row_indx;
int* col_ptr;
int* val_csc;

int vector[6] = {1,1,1,1,1,1};
int *ans_vector;
/*
 * Method declaration
 */
int Q1_d_createRandomArray(int n);
int Q1_e(int n, int* mat, int* val, int* col_indx, int* row_ptr);
int print_Q1_e(int n, int* val, int* col_indx, int* row_ptr);
int Q1_f(int n, int col, int row, int* val, int* col_indx, int* row_ptr);
int Q1_g(int n, int col, int row, int new_val, int* val, int* col_indx, int* row_ptr);
int Q1_h(int n, int* val, int* col_indx, int* row_ptr);
int print_Q1_h(int n, int* val, int* col_indx, int* row_ptr);
int Q1_j(int n, int* val, int* col_indx, int* row_ptr);
int random_Number_gen();

int main(){
    int n = 6,i;
    int col, row;
    int value;
    /*
     * memory allocation
     */
    val = (int*) calloc(n * n, sizeof(int));
    col_indx = (int*) calloc(n * n, sizeof(int));
    row_ptr = (int*) malloc((n+1) * sizeof(int));

    val2 = (int*) calloc(n * n, sizeof(int));
    col_indx2 = (int*) calloc(n * n, sizeof(int));
    row_ptr2 = (int*) malloc((n+1) * sizeof(int));

    val_csc = (int*) calloc(n * n, sizeof(int));
    row_indx = (int*) calloc(n * n, sizeof(int));
    col_ptr = (int*) malloc((n+1) * sizeof(int));

    ans_vector = (int*) calloc(n,  sizeof(int));
    ans_mat = (int*) malloc(n*n*sizeof(int));
    memcpy(ans_mat,mat,n*n*sizeof(int));
    /*
     * Convert dense matrix to sparse matrix (Q1. e)
     */
    Q1_e(n,mat,val,col_indx,row_ptr);
    Q1_e(n,mat2,val2,col_indx2,row_ptr2);

    while(1){
		print_Q1_e(n,val,col_indx,row_ptr);
        //print_Q1_e(n,val2,col_indx2,row_ptr2);
		/*printf("Enter i to read (1st row is 1) ");
		scanf("%d",&row);
		printf("Enter j to read (1st column is 1) ");
		scanf("%d",&col);

		//Get value of given index

		value = Q1_f(col,row);
		printf("Value at (%d,%d): %d\n",row, col,value);

		printf("Enter i to set (1st row is 1) ");
		scanf("%d",&row);
		printf("Enter j to set (1st column is 1) ");
		scanf("%d",&col);
		printf("Enter new value ");
		scanf("%d",&value);
		//set new value to given index
		Q1_g(col,row,value); */
        //Q1_h(n);
        //print_Q1_h(n);
        //Q1_j(n,val,col_indx,row_ptr);
        //print_Q1_j(n, ans_vector);
        //break;
        Q1_k(n,val,col_indx,row_ptr,val2,col_indx2,row_ptr2);
        scanf("%d",&value);
    }
}

int Q1_d_createRandomArray(int n){
    int rnd;
    int size = n*n;
    int i;
    for(i = 0; i < size; i++){
        rnd = rand()%100 + 1;
        if(rnd > 90){
            mat[i] = random_Number_gen();
        }else{
            mat[i] = 0;
        }
    }
    return 0;
}

int Q1_e(int n,int* mat, int* val, int* col_indx, int* row_ptr){
    int i,j;
    int nnz = 0;
    for(i = 0; i < n; i++){
        row_ptr[i] = nnz + 1;
        for(j = 0; j < n; j++){
            if(mat[i*n + j] != 0){
                *(val+nnz) = mat[i*n + j];
                *(col_indx + nnz) = j+1;
                nnz++;
            }
        }
    }
    row_ptr[n] = nnz + 1;
}

int print_Q1_e(int n, int* val, int* col_indx, int* row_ptr){
    int i;
    printf("Value    : ");
    for(i=0;i<n*n;i++){
		if(val[i] != 0)
		        printf("%d ",val[i]);
		else
			break;
		}
    printf("\nCol Index: ");
    for(i=0;i<n*n;i++){
		if(col_indx[i] != 0)
            printf("%d ",col_indx[i]);
	else
	    break;
    }
    printf("\nPtr Index: ");
    for(i=0; i<=n ;i++){
        printf("%d ",row_ptr[i]);
    }
    printf("\nDone\n");
}

int Q1_f(int n, int col, int row, int* val, int* col_indx, int* row_ptr){
    int i;
    int nnz = row_ptr[row - 1];
    int nnz2 = row_ptr[row];
    for(i = 0;i<n*n;i++){
     	if(col_indx[i] == col){
			if(i+1 >= nnz && i+1 < nnz2){
	  		return val[i];
			}
		}else if(col_indx[i] == 0){
	    	return 0;
		}
    }
}

int Q1_g(int n, int col, int row, int new_val, int* val, int* col_indx, int* row_ptr){
	int i;
    int cur_val = Q1_f(n,col,row,val,col_indx,row_ptr);
    int nnz = row_ptr[row - 1];
    int nnz2 = row_ptr[row];
    int index = -1,next_val,temp_val,next_col_index,temp_col_index;
    if(new_val != 0){
        for(i = 0;i<n*n;i++){
            if(col_indx[i] == col){
                if(i+1 >= nnz && i+1 < nnz2){
                    val[i] = new_val;
                    return 1;
                }
            }else if(col_indx[i] == 0){
                break;
            }
        }

        for(i = nnz-1; i < nnz2-1; i++){
            if(col < col_indx[i]+1){
                index = i;
                break;
            }
        }
        if(index == -1){
            index = i;
        }
        for(i=index;i<n*n;i++){
            if(i == index){
                next_val = val[i];
                next_col_index = col_indx[i];
                val[i] = new_val;
                col_indx[i] = col;
            }else{
                temp_val = val[i];
                temp_col_index = col_indx[i];
                if(temp_val == 0 && next_val == 0){
                    break;
                }
                val[i] = next_val;
                col_indx[i] = next_col_index;
                next_val = temp_val;
                next_col_index = temp_col_index;
            }
        }
        for(i=row;i<n*n;i++){
            if(row_ptr[i] != 0)
                row_ptr[i]++;
            else
                break;
        }
    }else{
        for(i = 0;i<n*n;i++){
            if(col_indx[i] == col){
                if(i+1 >= nnz && i+1 < nnz2){
                    index = i;
                    break;
                }
            }else if(col_indx[i] == 0){
                return 2;
            }
        }
        for(i=index;i<n*n;i++){
            val[i] = val[i+1];
            col_indx[i] = col_indx[i+1];
            if(val[i] == 0)
                break;
        }
        for(i=row;i<n*n;i++){
            if(row_ptr[i] != 0)
                row_ptr[i]--;
            else
                break;
        }
    }
}

int Q1_h(int n, int* val, int* col_indx, int* row_ptr){
    int nnz=0,i,j,k;
    for(i = 0; i<n ;i++){
        col_ptr[i] = nnz+1;
        for(j = 0;j<n*n;j++){
            if(col_indx[j] == i+1){
                val_csc[nnz] = val[j];
                for(k=0; k<n+1;k++){
                    if(row_ptr[k] <= j+1 && row_ptr[k+1] > j+1) {
                        row_indx[nnz] = k + 1;
                        break;
                    }
                }
                nnz++;
            }else if(col_indx[j] == 0)
                break;
        }
    }
    col_ptr[i] = nnz + 1;
}

int print_Q1_h(int n, int* val, int* col_indx, int* row_ptr){
    int i;
    printf("CSR Value   : ");
    for(i=0;i<n*n;i++){
        if(val[i] != 0)
            printf("%d ",val_csc[i]);
        else
            break;
    }
    printf("\nRow Index   : ");
    for(i=0;i<n*n;i++){
        if(col_indx[i] != 0)
            printf("%d ",row_indx[i]);
        else
            break;
    }
    printf("\nCol Ptr Index: ");
    for(i=0; i<=n ;i++){
        printf("%d ",col_ptr[i]);
    }
    printf("\nDone\n");
}

int Q1_j(int n, int* val, int* col_indx, int* row_ptr){
    int i,j,nnz=0;
    for(i=0; i<n; i++){
        for(j = row_ptr[i]; j < row_ptr[i+1]; j++){
            ans_vector[i] += val[nnz] * vector[col_indx[nnz] - 1];
            nnz++;
        }
    }
}

int print_Q1_j(int n, int* ans_vector) {
    int i;
    printf("\nMatrix Vector Multiplication : ");
    for(i=0; i<n ;i++){
        printf("%d ",ans_vector[i]);
    }
    printf("\nDone\n");
}

int Q1_k(int n, int* val, int* col_indx, int* row_ptr, int* val2, int* col_indx2, int* row_ptr2){
    int i, j, row1, col, value2,sum;
    for(i=0;i<n*n;i++){
        if(i == n*n || val[i] == 0)
            break;
        row1 = get_row_index(n,i,row_ptr);
        col = col_indx[i];
        value2 = Q1_f(n,col,row1,val2,col_indx2,row_ptr2);
        sum = value2 + val[i];
        Q1_g(n,col,row1,value2+val[i],val,col_indx,row_ptr);
        Q1_g(n,col,row1,0,val2,col_indx2,row_ptr2);

    }
    print_Q1_e(n,val,col_indx,row_ptr);

    if(val2[0] != 0){
        for(i=0;i<n*n;i++){
            if(i == n*n || val2[i] == 0)
                break;
            row1 = get_row_index(n,i,row_ptr2);
            Q1_g(n,col_indx2[i],row1,val2[i],val,col_indx,row_ptr);
            //Q1_g(col_indx2[i],row1,0,val2,col_indx2,row_ptr2);
        }
        for(i=0;i<n*n;i++){
            if(i == n*n || val2[i] == 0)
                break;
            row1 = get_row_index(n,i,row_ptr2);
            Q1_g(n,col_indx2[i],row1,0,val2,col_indx2,row_ptr2);
        }
    }
}

int get_row_index(int n, int val_index, int* row_ptr){
    int i;
    for(i=0; i<n; i++){
        if(val_index >= row_ptr[i]-1 && val_index < row_ptr[i+1]-1 )
            return i+1;
    }
}

int random_Number_gen(){
    srand(time(NULL));
    return (rand()%20);
}
