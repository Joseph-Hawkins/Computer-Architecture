#include <stdio.h>
#include <stdlib.h>

char possible_entries[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
char ** make_matrix(FILE * file);
void print_matrix(char ** matrix);
int solve_sudoku(char ** sudoku);
int solve_cell(char ** sudoku, int row, int col);
int * get_value_row(char ** martrix, int i, int j);
int * get_value_column(char ** matrix, int i, int j);
int * get_value_subsudoku(char ** matrix, int i, int j);
char ** get_subsudoku(char ** matrix, int i, int j);
int can_insert(char ** matrix, int i, int j, int charIndex);
int solved(char **matrix);


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

int * get_value_column(char ** matrix, int i, int j){
  int n, m;
  int*found = malloc(16*sizeof(int));
  for(n=0;n<16;n++){//check column                                                                  
    for(m=0;m<16;m++){
      if(matrix[n][j] == possible_entries[m])
        found[m]=1;
    }
  }
  return found;
}

int * get_value_row(char ** matrix, int i, int j){
  int n, m;
  int*found=malloc(16*sizeof(int));
  for(n=0;n<16;n++){//check row                                                      
    for(m=0;m<16;m++){
      if(matrix[i][n]==possible_entries[m])
        found[m] = 1;
    }
  }
  return found;
}

int * get_value_subsudoku(char ** matrix, int i, int j){
  char ** subsudoku = get_subsudoku(matrix, i, j);
  int n, m, k;
  int * found=malloc(16*sizeof(int));
  for(n=0;n<4;n++){
    for(m=0;m<4;m++){
      for(k=0;k<16;k++){
        if(subsudoku[n][m] == possible_entries[k])
          found[k]=1;
      }
    }
  }
  return found;
}

char ** get_subsudoku(char ** matrix, int i, int j){
  char ** subsudoku = malloc(4*sizeof(char*));
  int n,m;
  for(n=0; n<4; n++){
    subsudoku[n] = malloc(4*sizeof(char));
  }
  if(j>3)
    j=j-(j%4);
  else j=0;

  if(i>3)
    i=i-(i%4);
  else i=0;
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


char ** make_matrix(FILE * file){
  char ** matrix = malloc(16 * sizeof(char *));
  int i,j;
  for(i=0; i<16; i++){
    matrix[i] = malloc(16 * sizeof(char));
  }
  for(i=0; i<16; i++){
    for(j=0; j<16; j++){
      fscanf(file, "%c ", &matrix[i][j]);
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

int solve_sudoku(char ** sudoku){
  return solve_cell(sudoku, 0, 0);
}

int solve_cell(char ** sudoku, int row, int col){ 

  if(col==16){
    col=0;
    row++;
    if(row>15){
      if(solved(sudoku)){
	print_matrix(sudoku);
      }
      return 1;
    }
  }

  if(sudoku[row][col] != '-'){
    return solve_cell(sudoku, row, col+1);
  }

  int i;
  for(i=0;i<16;i++){
    if(can_insert(sudoku, row, col, i)){
      sudoku[row][col] = possible_entries[i];
      if(solve_cell(sudoku,row, col+1)){
	return 1;
      }
    }
  }
  sudoku[row][col] = '-';
  return 0;
  //if unsolvable from current position return 1
}

int can_insert(char ** sudoku, int i, int j, int charIndex){
  int * row = get_value_row(sudoku, i, j);
  int * col = get_value_column(sudoku, i, j);
  int * sub = get_value_subsudoku(sudoku, i, j);

  if(row[charIndex]==0 && col[charIndex]==0 && sub[charIndex]==0){
    return 1;
  }

  return 0; // if inserting this character causes an invalid matrix
}


int main(int argc, char ** argv){
  FILE *file;
  file = fopen(argv[1], "r");
  char ** sudoku = make_matrix(file);
  if(solve_sudoku(sudoku))
    return 0;

  else{
    printf("no-solution\n");
  }
  return 0;
}
