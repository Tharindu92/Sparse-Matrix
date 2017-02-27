#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int* mat;
int* mat2;
int* ans_mat;

int* val;           int* val2;          int* ans_val;
int* col_indx;      int* col_indx2;     int* ans_col_indx;
int* row_ptr;       int* row_ptr2;      int* ans_row_ptr;

int* row_indx;
int* col_ptr;
int* val_csc;

int* vector;
int* ans_vector_sparse;
int* ans_vector_dense;

int sample_sizes[6] = {10, 50, 100, 500, 1000, 5000};
/*
 * Method declaration
 */
int createRandomArray(int n, int percentage, int* mat);
void dense_to_csr_convertion(int n, int* mat, int* val, int* col_indx, int* row_ptr);
void print_dense_to_csr_convertion(int n, int* val, int* col_indx, int* row_ptr);
int get_value(int n, int col, int row, int* val, int* col_indx, int* row_ptr);
int set_value(int n, int col, int row, int new_val, int* val, int* col_indx, int* row_ptr);
int csr_to_csc(int n, int* val, int* col_indx, int* row_ptr);
void print_csr_to_csc(int n, int* val, int* col_indx, int* row_ptr);
int csr_vector_mul(int n, int* val, int* col_indx, int* row_ptr);
void print_csr_vector_mul(int n, int* ans_vector_sparse);
int csr_csr_addition(int n, int* val, int* col_indx, int* row_ptr, int* val2, int* col_indx2, int* row_ptr2);
int random_Number_gen(int limit);
int get_row_index(int n, int val_index, int* row_ptr);
void allocate_memory(int n);
void free_memory();
void pre_processing(int n, int percentage);
void generate_vector(int n, int* vector);
int mat_mat_addition(int n, int* mat, int* mat2);
int mat_vector_mul(int n, int* mat, int* vec);

int main(){
    int n = 6,row,col,percentage = 90;
    int value, choise,samples;
    clock_t begin, end;
    /*
     * Create random array with 90
     */

    /*
     * Convert dense matrix to sparse matrix (Q1. e)
     */

    printf("To run Q1. j) press 1\n");
    printf("To run Q1. k) press 2\n");
    printf("Press any other to run Q1. i)\n");

    printf("\nEnter your preference");
    scanf("%d", &choise);

    if(choise == 1) {
        int i, j;
        double tot_time_sparse = 0, tot_time_dense = 0;
        printf("******Run Q1. j)******\n");
        printf("Enter number of samples ");
        scanf("%d", &samples);
        for (i = 0; i < 6; i++) {
            tot_time_sparse = 0;
            tot_time_dense = 0;
            for (j = 0; j < samples; j++) {
                n = sample_sizes[i];
                /*
                 * memory allocation
                 */
                allocate_memory(n);
                pre_processing(n, percentage);
                generate_vector(n, vector);


                begin = clock();
                csr_vector_mul(n, val, col_indx, row_ptr);
                end = clock();
                tot_time_sparse += (double) (end - begin) / CLOCKS_PER_SEC;
                begin = clock();
                mat_vector_mul(n, mat, vector);
                end = clock();
                tot_time_dense += (double) (end - begin) / CLOCKS_PER_SEC;

                free_memory();
            }
            printf("n = %d\t", n);
            printf("sparse time = %.10lf\t",tot_time_sparse/samples);

            printf("n = %d\t", n);
            printf("dense time = %.10lf\n",tot_time_dense/samples);

        }
    }else if(choise == 2){
        int i, j;
        double tot_time_sparse = 0, tot_time_dense = 0;
        printf("******Run Q1. k)******\n");
        printf("Enter number of samples ");
        scanf("%d", &samples);
        for (i = 0; i < 6; i++) {
            tot_time_sparse = 0;
            tot_time_dense = 0;
            for (j = 0; j < samples; j++) {
                n = sample_sizes[i];
                /*
                 * memory allocation
                 */
                allocate_memory(n);
                pre_processing(n, percentage);
                generate_vector(n, vector);


                begin = clock();
                csr_csr_addition(n,ans_val,ans_col_indx,ans_row_ptr,val2,col_indx2,row_ptr2);
                csr_vector_mul(n, val, col_indx, row_ptr);
                end = clock();
                tot_time_sparse += (double) (end - begin) / CLOCKS_PER_SEC;
                begin = clock();
                mat_mat_addition(n,ans_mat,mat2);
                end = clock();
                tot_time_dense += (double) (end - begin) / CLOCKS_PER_SEC;

                free_memory();
            }
            printf("n = %d\t", n);
            printf("sparse time = %.10lf\t", tot_time_sparse / samples);

            printf("n = %d\t", n);
            printf("dense time = %.10lf\n", tot_time_dense / samples);
        }
    }else{
        while(1){
            int i;
            printf("******Q1) i******\n");
            printf("Enter the size of the Matrix ");
            scanf("%d",&n);

            mat = (int*) calloc(n*n, sizeof(int));

            val = (int*) calloc(n * n, sizeof(int));
            col_indx = (int*) calloc(n * n, sizeof(int));
            row_ptr = (int*) malloc((n+1) * sizeof(int));

            val_csc = (int*) calloc(n * n, sizeof(int));
            row_indx = (int*) calloc(n * n, sizeof(int));
            col_ptr = (int*) malloc((n+1) * sizeof(int));

            createRandomArray(n,percentage,mat);
            printf("Convert to CSR format\n");
            dense_to_csr_convertion(n,mat,val,col_indx,row_ptr);
            print_dense_to_csr_convertion(n,val,col_indx,row_ptr);

            for(i = 0; i < n; i++){
                set_value(n,i+1,i+1,2016,val,col_indx,row_ptr);
            }
            printf("\nConvert CSR to CSC format\n");
            csr_to_csc(n,val,col_indx,row_ptr);
            print_csr_to_csc(n,val,col_indx,row_ptr);
        }
    }
}

/*
 * Answer for Q1) d.
 */
int createRandomArray(int n, int percentage, int* mat){
    int rnd;
    int size = n*n;
    int i = 0;
    int nnz = n*n * (100 - percentage) / 100;
    int row, col;
    srand(time(NULL));
    while(i<nnz){
        row = rand()%n;
        col = rand()%n;
        if(mat[row*n + col] == 0){
            rnd = rand()%99 + 1;
            mat[row*n + col] = rnd;
            i++;

        }
    }
    return 0;
}
/*
 * Answer for Q1) e
 */
void dense_to_csr_convertion(int n,int* mat, int* val, int* col_indx, int* row_ptr){
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

void print_dense_to_csr_convertion(int n, int* val, int* col_indx, int* row_ptr){
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

/*
 * Answer to Q1) f
 */
int get_value(int n, int col, int row, int* val, int* col_indx, int* row_ptr){
    int i;
    int nnz = row_ptr[row - 1];
    int nnz2 = row_ptr[row];
    for(i = nnz-1;i<nnz2-1;i++){
     	if(col_indx[i] == col){
			return val[i];
		}else if(col_indx[i] == 0){
	    	return 0;
		}
    }
    return 0;
}

/*
 * Answer to Q1) g
 */
int set_value(int n, int col, int row, int new_val, int* val, int* col_indx, int* row_ptr){
	int i;
    int nnz = row_ptr[row - 1];
    int nnz2 = row_ptr[row];
    int index = -1,next_val=-1,temp_val,next_col_index,temp_col_index;
    if(new_val != 0){
        for(i = nnz-1;i<nnz2-1;i++){
            if(col_indx[i] == col){
                val[i] = new_val;
                return 1;
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
        for(i = nnz-1;i<nnz2-1;i++){
            if(col_indx[i] == col){
                index = i;
                break;
            }else {
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
/*
 * Answer to Q1) h
 */
int csr_to_csc(int n, int* val, int* col_indx, int* row_ptr){
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
    return 0;
}

void print_csr_to_csc(int n, int* val, int* col_indx, int* row_ptr){
    int i;
    printf("CSC Value   : ");
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

/*
 * Answer for Q1 j
 */
int csr_vector_mul(int n, int* val, int* col_indx, int* row_ptr){
    int i,j,nnz=0;
    for(i=0; i<n; i++){
        for(j = row_ptr[i]; j < row_ptr[i+1]; j++){
            ans_vector_sparse[i] += val[nnz] * vector[col_indx[nnz] - 1];
            nnz++;
        }
    }
    return 0;
}

void print_csr_vector_mul(int n, int* ans_vector_sparse) {
    int i;
    printf("\nMatrix Vector Multiplication : ");
    for(i=0; i<n ;i++){
        printf("%d ",ans_vector_sparse[i]);
    }
    printf("\nDone\n");
}

/*
 * Answer for Q1) k
 */
int csr_csr_addition(int n, int* val, int* col_indx, int* row_ptr, int* val2, int* col_indx2, int* row_ptr2){
    int i, col, col2;
    int nnz1,nnz2,tot_nnz=0;
    for(i = 0; i<n+1; i++){
        ans_row_ptr[i] = tot_nnz + 1;
        nnz1 = row_ptr[i+1] - 1;
        nnz2 = row_ptr2[i+1] - 1;
        col = row_ptr[i]-1;
        col2 = row_ptr2[i]-1;
        ans_row_ptr[i] = tot_nnz + 1;
        while(col < nnz1  || col2 < nnz2 ){

            if(col_indx[col] > col_indx2[col2] && col2 < row_ptr2[i+1]-1){
                ans_val[tot_nnz] = val2[col2];
                ans_col_indx[tot_nnz] = col_indx2[col2];
                tot_nnz++;
                col2++;
            }else if(col_indx2[col2] > col_indx[col] && col < row_ptr[i+1] -1){
                ans_val[tot_nnz] = val[col];
                ans_col_indx[tot_nnz] = col_indx[col];
                tot_nnz++;
                col++;
            }else if(col_indx2[col2] > col_indx[col]){
                ans_val[tot_nnz] = val2[col2];
                ans_col_indx[tot_nnz] = col_indx2[col2];
                tot_nnz++;
                col2++;
            }else if(col_indx2[col2] < col_indx[col]){
                ans_val[tot_nnz] = val[col];
                ans_col_indx[tot_nnz] = col_indx[col];
                tot_nnz++;
                col++;
            }else if(col_indx2[col2] == col_indx[col] && col2 < row_ptr2[i+1] - 1 || col_indx2[col2] == col_indx[col] && col < row_ptr[i+1] - 1){
                if(val[col] + val2[col2] != 0){
                    ans_val[tot_nnz] = val[col] + val2[col2];
                    ans_col_indx[tot_nnz] = col_indx[col];
                    tot_nnz++;
                }
                col++;
                col2++;
            }
        }
    }
}

int mat_vector_mul(int n, int* mat, int* vec){
    int i,j,temp;
    for(i=0; i<n; i++){
        temp = 0;
        for(j=0; j<n; j++){
            temp += mat[i*n + j] * vec[j];
        }
        ans_vector_dense[i] = temp;
    }
}

int mat_mat_addition(int n, int* mat, int* mat2){
    int i,j,temp;
    for(i=0; i<n*n; i++){
        mat[i] += mat2[i];
    }
}

int get_row_index(int n, int val_index, int* row_ptr){
    int i;
    for(i=0; i<n; i++){
        if(val_index >= row_ptr[i]-1 && val_index < row_ptr[i+1]-1 )
            return i+1;
    }
}

int random_Number_gen(int limit){
    srand(time(NULL));
    return (rand()%limit);
}

void allocate_memory(int n){
    mat = (int*) calloc(n*n, sizeof(int));
    mat2 = (int*) calloc(n*n, sizeof(int));

    val = (int*) calloc(n * n, sizeof(int));
    col_indx = (int*) calloc(n * n, sizeof(int));
    row_ptr = (int*) malloc((n+1) * sizeof(int));

    val2 = (int*) calloc(n * n, sizeof(int));
    col_indx2 = (int*) calloc(n * n, sizeof(int));
    row_ptr2 = (int*) malloc((n+1) * sizeof(int));

    val_csc = (int*) calloc(n * n, sizeof(int));
    row_indx = (int*) calloc(n * n, sizeof(int));
    col_ptr = (int*) malloc((n+1) * sizeof(int));

    ans_vector_sparse = (int*) calloc(n,  sizeof(int));
    ans_vector_dense = (int*) calloc(n,  sizeof(int));
    ans_mat = (int*) malloc(n*n * sizeof(int));

    ans_val = (int*) calloc(n * n, sizeof(int));
    ans_col_indx = (int*) calloc(n * n, sizeof(int));
    ans_row_ptr = (int*) malloc((n+1) * sizeof(int));

    vector = (int*) malloc(n*sizeof(int));
}

void free_memory(){
    free(mat);
    free(mat2);

    free(val);
    free(col_indx);
    free(row_ptr);

    free(val2);
    free(col_indx2);
    free(row_ptr2);

    free(val_csc);
    free(row_indx);
    free(col_ptr);

    free(ans_vector_sparse);
    free(ans_mat);
    free(ans_vector_dense);

    free(ans_val);
    free(ans_col_indx);
    free(ans_row_ptr);

}

void pre_processing(int n, int percentage){
    createRandomArray(n, percentage, mat);
    createRandomArray(n, percentage, mat2);
    memcpy(ans_mat,mat, n*n*sizeof(int));

    dense_to_csr_convertion(n,mat,val,col_indx,row_ptr);
    dense_to_csr_convertion(n,mat2,val2,col_indx2,row_ptr2);
    dense_to_csr_convertion(n,ans_mat,ans_val,ans_col_indx,ans_row_ptr);
}

void generate_vector(int n, int* vector){
    int i;
    for(i = 0; i<0; i++){
        vector[i] = random_Number_gen(100);
    }
}

