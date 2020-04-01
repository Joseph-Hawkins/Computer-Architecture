#include <stdio.h>
#include <stdlib.h>
char possible_entries[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
char ** make_matrix(FILE * file);
void print_matrix(char ** matrix);
int is_valid_sudoku(char ** matrix);
char ** get_subsudoku(char ** matrix, int i, int j);
void print_subsudoku(char ** matrix);
int invalid_row(char **matrix, int i, int j);
int invalid_col(char **matrix, int i, int j);
int invalid_subsudoku(char ** matrix);
int solved(char**matrix);
int * valid_move(char ** matrix);
int valid_insert(char ** matrix, int i, int j);
int * get_value_row(char ** martrix, int i, int j);
int * get_value_column(char ** matrix, int i, int j);
int * get_value_subsudoku(char ** matrix, int i, int j);
void print_found_list(int * list);
int compare_insert_values(int* row, int* col, int* subchar);

int solved(char**matrix){
  int i,j;
  for(i=0;i<16;i++){
    for(j=0;j<16;j++){
      if(matrix[i][j] == '-'){
	return 0;//puzzle not yet solved
      }
    }
  }
  return 1; //when puzzle is solved
}

void print_found_list(int * list){
  int i;
  for(i=0;i<16;i++){
    printf("%c ", possible_entries[i]);
  }
  printf("\n");
  for(i=0;i<16;i++){
    printf("%d ", list[i]);
  }
  printf("\n");
}

int compare_insert_values(int*row, int*col, int*subchar){
  int*temp=malloc(16*sizeof(int));
  int i;
  int countRow=0;
  int countCol=0;
  int countSub=0;
  int countUnion=0;

  for(i=0;i<16;i++){
    temp[i] = 0;
  }
  for(i=0;i<16;i++){//count # of empty spots in rows
    if(row[i]==1){
      temp[i]=1;
      countRow++;
    }
  }
  for(i=0;i<16;i++){//count # of empty spots in subsudoku
    if(subchar[i]==1){
      temp[i]=1;
      countSub++;
    }   
  }
  for(i=0;i<16;i++){//count # of empty spots in columns
    if(col[i]==1){
      temp[i]=1;
      countCol++;
    }   
  }

  for(i=0;i<16;i++){
    if(temp[i]==0){
      countUnion++;
    }
  }  
 
  if(countCol==15){
    for(i=0;i<16;i++){
      if(col[i]==0){
	return i;
      }
    }
  }
  if(countSub==15){
    for(i=0;i<16;i++){
      if(subchar[i]==0){
	return i;
      }
    }
  }   
  if(countRow==15){
    for(i=0;i<16;i++){
      if(row[i]==0){
        return i;
      }
    }
  }
  
  if(countUnion==1){
    for(i=0;i<16;i++){
      if(temp[i]==0){
        return i;
      }
    }
  }
  return -1;
}

int * get_value_subsudoku(char ** matrix, int i, int j){  
  char ** subsudoku = get_subsudoku(matrix, i, j);
  int n, m, k;
  int count = 0;
  int * found=malloc(16*sizeof(int));
  for(n=0;n<4;n++){
    for(m=0;m<4;m++){
      for(k=0;k<16;k++){
	if(subsudoku[n][m] == possible_entries[k]){
	  found[k]=1;
	  count++;
	}
      }
    }
  }
  return found;
}

int * get_value_column(char ** matrix, int i, int j){
  int n, m;
  int*found = malloc(16*sizeof(int));
  int count = 0;
  for(n=0;n<16;n++){//check column                     
    for(m=0;m<16;m++){
      if(matrix[n][j] == possible_entries[m]){
        found[m]=1;
	count++;
      }
    }
  }
  return found;
}

int *get_value_row(char ** matrix, int i, int j){
  int n, m, count;
  count=0;
  int*found=malloc(16*sizeof(int));
  for(n=0;n<16;n++){//check row
    for(m=0;m<16;m++){
      if(matrix[i][n]==possible_entries[m]){
	found[m] = 1;
	count++;
      }
    }
  }
  return found;
}

int valid_insert(char ** matrix, int i, int j){
  int numDashRow = 0;
  int numDashCol = 0;
  int numDashSub = 0;
  int r;  //search row
  for(r=0;r<16;r++){
    if(matrix[i][r] == '-'){
      numDashRow++;
    }
  }
  int c; //search column
  for(c=0;c<16;c++){
    if(matrix[c][j] =='-'){
      numDashCol++;
    }
  }
  char ** subsudoku;//search subsudoku
  subsudoku = get_subsudoku(matrix, i, j);
  for(r=0;r<4;r++){
    for(c=0;c<4;c++){
      if(subsudoku[r][c] == '-'){
	numDashSub++;
      }
    }
  }
  if(numDashRow == 1 || numDashCol == 1 || numDashSub == 1)
    return 1;

  int z =  compare_insert_values(get_value_row(matrix,i,j), get_value_column(matrix,i,j), get_value_subsudoku(matrix,i,j));
  if(z != -1){
    return 1;
  }//combined found array has 1 zero value

  return 0;
}

int * valid_move(char ** matrix){
  int * index;
  index = malloc(2*sizeof(int));
  int i,j;
  for(i=0; i<16; i++){
    for(j=0; j<16; j++){
      if(matrix[i][j] == '-' && compare_insert_values(get_value_row(matrix,i,j), get_value_column(matrix,i,j), get_value_subsudoku(matrix,i,j)) > -1){
	if(valid_insert(matrix, i, j)==1){
	  index[0]=i;
	  index[1]=j;
	  return index;
	}
      }
    }
  }
  index[0]=-1;
  index[1]=-1;
  return index;
}

char ** get_subsudoku(char ** matrix, int i, int j){
  char ** subsudoku;
  int k;
  subsudoku = malloc(4 * sizeof(char*));
  for(k=0; k<4; k++){
    subsudoku[k] = malloc(4*sizeof(char));
  }
  if(j>3)
    j=j-(j%4);
  else j=0;

  if(i>3)
    i=i-(i%4);
  else i=0;
  int n,m;
  int temp = j;
  for(n=0;n<4;n++){
    for(m=0;m<4;m++){
      subsudoku[n][m] = matrix[i][j];//fill subsudoku
      j++;
    }
    i++;
    j = temp;
  }
  return subsudoku;
}

int invalid_row(char ** matrix, int i, int j){
  int x;
  for(x=0; x<16; x++){
    if(x!=j && matrix[i][j] == matrix[i][x])
      return 1;
  }
  return 0;
}
int invalid_col(char ** matrix, int i, int j){
  int x;
  for(x=0; x<16; x++){
    if(x!=i && matrix[i][j] == matrix[x][j])
      return 1;
  }
  return 0;
}

int invalid_subsudoku(char ** subsudoku){
  int i,j;
  int x,y;
  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      for(x=0; x<4; x++){
	for(y=0; y<4; y++){
	  if(subsudoku[i][j] != '-' && x!=i && y!=j && subsudoku[i][j] == subsudoku[x][y]){
	    return 1;
	  }
	}
      }
    }
  }
  return 0;
}

int is_valid_sudoku(char ** matrix){
  int num = 1;
  int i,j, dash;
  for(i=0;i<16;i++){
    for(j=0;j<16;j++){
      dash=0;
      if(matrix[i][j] == '-'){
	dash=1;
      }

      if(dash == 0){//current element is a dash
	if(invalid_row(matrix, i, j) == 1){//if row is invalid
	  return 0;
	}
	if(invalid_col(matrix, i, j) == 1){//if column is invalid
	  return 0;
	}
	char ** subsudoku = get_subsudoku(matrix, i, j);
	if(invalid_subsudoku(subsudoku)== 1){//if subsudoku is invalid
	  return 0;
	}
      }  
    }
  }
  return num;
}

char ** make_matrix(FILE * file){
  char ** matrix = malloc(16 * sizeof(char *));
  int i;
  for(i=0; i<16; i++){
    matrix[i] = malloc(16 * sizeof(char));
  } 
  int j;
  for(i=0; i<16; i++){
    for(j=0; j<16; j++){
      char temp;
      fscanf(file, "%c ", &temp);
      matrix[i][j] = temp;
    }
  }
  return matrix;		 
}

void print_matrix(char ** matrix){
  int i,j;
  for(i=0; i<16; i++){
    for(j=0; j<16; j++){
      printf("%c\t", matrix[i][j]);
    }
    printf("\n");
  }
}

void print_subsudoku(char ** matrix){
  int i,j;
  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      printf("%c\t", matrix[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char ** argv){
  FILE *file;
  file = fopen(argv[1], "r");
  char ** matrix;
  matrix = make_matrix(file);

  while(is_valid_sudoku(matrix) && solved(matrix)!=1){
    // print_matrix(matrix);
    //printf("\n\n");
    int * index;
    index = valid_move(matrix);
    int i,j;
    i=index[0];
    j=index[1];
    //printf("\nattempted insert at A[%d] [%d]\n", i, j);
    if(valid_insert(matrix, i, j)){
      int *row = get_value_row(matrix, i, j);
      int *col = get_value_column(matrix, i, j);
      int *subchar = get_value_subsudoku(matrix, i, j);
      int charIndex = compare_insert_values( row, col, subchar);
      matrix[i][j] = possible_entries[charIndex]; 
    }
  }

  if(is_valid_sudoku(matrix) == 0)
    printf("no-solution\n");
  else print_matrix(matrix); 

return 0;
}
