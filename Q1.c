#include <stdio.h>
#include <stdlib.h>

int mat[36] = {1, 0,0,0,2,0,3,9,0,0,0,3,0,7,8,7,0,0,3,0,8,7,5,0,0,8,0,9,9,3,0,4,0,0,2,1};

int* val;
int* col_indx;
int* row_ptr;

int main(){
    int n = 6,i;
    int col, row;
    int value;
    val = (int*) calloc(n * n, sizeof(int));
    col_indx = (int*) calloc(n * n, sizeof(int));
    row_ptr = (int*) malloc((n+1) * sizeof(int));
    Q1_e(n);  
    while(1){
		print_Q1_e(n);
		printf("Enter i to read (1st row is 1) ");
		scanf("%d",&row);
		printf("Enter j to read (1st column is 1) ");
		scanf("%d",&col);
		value = Q1_f(col,row); 
		printf("Value at (%d,%d): %d\n",row, col,value);
	
		printf("Enter i to set (1st row is 1) ");
		scanf("%d",&row);
		printf("Enter j to set (1st column is 1) ");
		scanf("%d",&col);
		printf("Enter new value ");
		scanf("%d",&value);
		Q1_g(col,row,value);
    }	
}

/*int Q1_d_createRandomArray(int n){
    int rnd;
    int size = n*n;
    int i;
    for(i = 0; i < size; i++){
        rnd = rand()%100 + 1;
        if(rnd > 90){
            mat[i] = randon_Number_gen();
        }else{
            mat[i] = 0;
        }
    }
    return 0;
}
*/
int Q1_e(int n){
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

int print_Q1_e(int n){
    int i;
    printf("Value    : ");
    for(i=0; ;i++){
		if(val[i] != 0)
		        printf("%d ",val[i]);
		else
			break;        
		}
    printf("\nCol Index: ");
    for(i=0; ;i++){
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

int Q1_f(int col, int row){
    int i;
    int nnz = row_ptr[row - 1];
    int nnz2 = row_ptr[row];
    for(i = 0; ;i++){
     	if(col_indx[i] == col){
			if(i+1 >= nnz && i+1 < nnz2){
	  		return val[i];
			}
		}else if(col_indx[i] == 0){
	    	return 0;
		}
    }
}

int Q1_g(int col, int row, int new_val){
	int i;
    int cur_val = Q1_f(col,row);
    int nnz = row_ptr[row - 1];
    int nnz2 = row_ptr[row];
    int index,next_val,temp_val,next_col_index,temp_col_index;
    for(i = 0; ;i++){
	   	if(col_indx[i] == col){
			if(i+1 >= nnz && i+1 < nnz2){
			val[i] = new_val;
			return 1;
			}
		}else if(col_indx[i] == 0){
			break;
		}	
    }
    for(i = nnz; i < nnz2; i++){
		if(col < col_indx[i]+1){
			index = i; 
			break;
		}
    }
    for(i=index; ;i++){
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
	for(i=row; ;i++){
		if(row_ptr[i] != 0)
			row_ptr[i]++;
		else
			break;
	}
}
/*
int random_Number_gen(){
    srand(time(NULL));
    return (rand()%20);
}*/
