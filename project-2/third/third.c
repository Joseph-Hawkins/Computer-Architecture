#include <stdio.h>
#include <stdlib.h>
void print_matrix(double ** matrix, int rows, int columns);
double ** make_matrix(FILE * file, int rows, int columns);
double * strip_house_prices(double ** matrix, int rows, int columns);
double ** trim_x(double** matrix, int rows, int columns);
double ** transpose(double** matrix, int rows, int columns);
double ** allocate_transpose(double ** matrix, int rows, int columns);
double ** multiply(double** A, double** B, int rows, int columns);
double ** inverse(double** xTx, int rows, int columns);
double ** rowReduct(double ** aug, int rows, int columns);
double * make_leading_one(double * pivotRow, int i, int columns);
double ** subtract(double ** aug, double * pivotRow, int pivotIndex, int rows, int columns);
double ** multiply2(double ** A, double ** B, int rowsA, int columnsA, int columnsB);
double * multiply3(double ** A, double * B, int rowsA, int columnsA, int columnsB);
double * multiply4(double ** A, double* B, int rowsA, int columnsA, int rows );
void test(double * W, double ** testing, int rows, int testPoints, int testColumns);
double ** add_leading_one(double ** testData, int rows, int columns);
void free_matrix(double ** matrix, int rows);
void free_list(double * list, int rows);


void free_list(double * list, int rows){
    free(list);
}

void free_matrix(double ** matrix, int rows){
  int i;
  for(i=0;i<rows;i++){
    free(matrix[i]);
  }
  free(matrix);
}

double ** add_leading_one(double ** testData, int rows, int columns){
  double ** testNew = malloc(rows*sizeof(double *));
  int i,j;
  for(i=0;i<rows;i++){
    testNew[i] = malloc((columns+1) * sizeof(double));
    testNew[i][0] = 1;
  }
  for(i=0;i<rows;i++){
    for(j=0;j<columns;j++){
      testNew[i][j+1] = testData[i][j];
    }
  }
  return testNew;
}

void test(double * W, double ** testing, int rows, int testPoints, int testColumns){
  int i,j;
  double * prices = malloc(testPoints * sizeof(double));
  for(i=0;i<testPoints;i++){
    for(j=0;j<testColumns;j++){
      prices[i]=prices[i] + (testing[i][j]*W[j]);
    }
    printf("%0.0lf \n", prices[i]);
  }
  free(prices);
}

double ** inverse(double** xTx, int rows, int columns){
  double ** aug = malloc(rows*sizeof(double*));
  int i;
  for(i=0;i<rows;i++){
    aug[i] = malloc( (2*rows) * sizeof(double));
  }
  int j;
  for(i=0;i<rows;i++){//fill augmented matrix
    for(j=0;j<columns;j++){ //fill in xTx
      aug[i][j] = xTx[i][j];
    }
    for(j=columns;j<(2*columns);j++){//fill in identity matrix
      if(i==(j-columns)){
	aug[i][j] = 1.0;
      }
      else{
	aug[i][j] = 0.0;
      }
    }
  }
  aug = rowReduct(aug, rows, columns*2);

  //puts transformed identity matrix into a matrix by itself to return 
  double ** xTxInverse = malloc(rows*sizeof(double*));
  for(i=0;i<rows;i++){
    xTxInverse[i]=malloc(columns*sizeof(double));
  }

  for(i=0;i<rows;i++){
    for(j=0;j<columns;j++){
      xTxInverse[i][j] = aug[i][j+columns];
    }
  }
  free_matrix(aug,rows);
  return xTxInverse;
}

double ** rowReduct(double ** aug, int rows, int columns){
  int i;
  for(i=0;i<rows;i++){
    aug[i] = make_leading_one(aug[i], i, columns);//turn leading element into a 1    
    aug = subtract(aug, aug[i], i, rows, columns);
  }
  return aug;
}

//subtracts the pivot row from all other rows
//s.t. the column of the leading one contains 0's and itself
double ** subtract(double ** aug, double * pivotRow, int pivotIndex, int rows, int columns){
  int i;
  int j;
  for(i=0;i<pivotIndex;i++){//before pivot row
    for(j=columns-1;j>-1;j--){ 
      aug[i][j]=aug[i][j]-(pivotRow[j]*aug[i][pivotIndex]);
    }
  }

  for(i=pivotIndex+1;i<rows;i++){//after pivot row
    for(j=columns-1;j>-1;j--){
      aug[i][j]=aug[i][j]-(pivotRow[j]*aug[i][pivotIndex]);
    }
  }

  return aug;
}

double * make_leading_one(double * pivotRow, int i,  int columns){
  double temp = pivotRow[i];
  int j;

  for(j=0;j<columns;j++){//divide whole row to get  a leading one
    pivotRow[j] = pivotRow[j]/temp;
  }
  return pivotRow;
}

double ** multiply(double** A, double** B, int rowsA, int columnsA){
  double ** AB = malloc(rowsA*sizeof(double*));
  int i,j,m;
  for(i=0;i<rowsA;i++){
    AB[i] = malloc(rowsA*sizeof(double));
  }  
  for(i=0;i<rowsA;i++){
    for(j=0;j<columnsA;j++){
      for(m=0;m<rowsA;m++){
	AB[i][m] = ( AB[i][m] + (A[i][j] * B[j][m]));
      }
    }
  }
  return AB;
}

double ** multiply2(double ** A, double ** B, int rowsA, int columnsA, int columnsB){
  double ** AB = malloc(rowsA*sizeof(double*));
  int i,j,m;
  for(i=0;i<rowsA;i++){
    AB[i] = malloc(columnsB*sizeof(double));
  }
  for(i=0;i<rowsA;i++){
    for(j=0;j<columnsA;j++){
      for(m=0;m<columnsB;m++){
        AB[i][m] = ( AB[i][m] + (A[i][j] * B[j][m]));
      }
    }
  }
  return AB;
}

double * multiply3(double ** A, double* B, int rowsA,int columnsA, int columnsB){
  double * W = malloc(columnsB * sizeof(double));
  int i,j;

  for(i=0;i<rowsA;i++){
    for(j=0;j<columnsA;j++){
	W[i] = (W[i] + (A[i][j] *B[j]));
    }
  }
  return W;
}

double * multiply4(double ** A, double* B, int rowsA, int columnsA, int rows ){
  double  * w =malloc(rowsA*sizeof(double));
  int i,j;
  for(i=0;i<rowsA;i++){
    for(j=0;j<columnsA;j++){
      w[i] = w[i] + (A[i][j]*B[j]);
    }
  }
  return w;
}

double ** allocate_transpose(double ** matrix, int rows, int columns){
  int i;
  double ** transpose = malloc(columns*sizeof(double*));
  for(i=0;i<columns; i++){
    transpose[i] = malloc(rows*sizeof(double));
  }
  return transpose;
}

double ** transpose(double** matrix, int rows, int columns){
  double ** transposed = malloc(columns * sizeof(double*));
  int i,j;
  for(i=0;i<columns;i++){
    transposed[i] = malloc(rows * sizeof(double));
  }
  for(i=0;i<rows;i++){
    for(j=0;j<columns;j++){
      transposed[j][i] = matrix[i][j];
    }
  }
  return transposed;
}


double * strip_house_prices(double ** matrix, int rows, int columns){
  double * housePrices = malloc(rows * sizeof(double));
  int i;
  for(i=0;i<rows;i++){
    housePrices[i] = matrix[i][columns-1];
  }
  return housePrices;
}

double ** trim_x(double** matrix, int rows, int columns){
  double ** trimmed = malloc(rows*sizeof(double*));
  int i,j;
  for(i=0;i<rows;i++){
    trimmed[i]=malloc(columns * sizeof(double));
  }
  
  for(i=0;i<rows;i++){
    trimmed[i][0] = 1;
  }


  for(i=0;i<rows;i++){//copy over to trimmed
    for(j=1;j<columns;j++){
      trimmed[i][j] = matrix[i][j-1];
    }
  }
  return trimmed;
}

double ** make_matrix(FILE * file, int rows, int columns){
  int i, j;
  double ** matrix = malloc(rows * sizeof(double *));
  for(i=0; i<rows; i++){
    matrix[i] = malloc(columns * sizeof(double));
  } 
  for(i=0; i<rows; i++){
    for(j=0; j<columns; j++){
      double temp;
      char comma;
      fscanf(file, "%lf", &temp);
      matrix[i][j] = temp;
      if(j<(columns-1) ){
	fscanf(file, "%c", &comma);
      }
    }
  }
  return matrix;		 
}

void print_matrix(double ** matrix, int rows, int columns){
  int i,j;
  for(i=0; i<rows; i++){
    for(j=0; j<columns; j++){
      printf("%lf", matrix[i][j]);
      if(j< (columns-1)){
	printf(",");
      }
    }
    printf("\n");
  }
}









int main(int argc, char ** argv){
  FILE *trainData;
  FILE *testData;
  trainData = fopen(argv[1], "r");
  testData = fopen(argv[2], "r");

  int rows, columns;
  fscanf(trainData, "%d", &columns);
  fscanf(trainData, "%d", &rows);
  columns++;

  double** training;
  training = make_matrix(trainData, rows, columns);

  int columnsTest;
  columnsTest=columns-1;

  int testPoints;
  double** testing;
  fscanf(testData, "%d", &testPoints);
  testing = make_matrix(testData, testPoints, columnsTest);

  double* housePrices;
  housePrices = strip_house_prices(training, rows, columns);
  training = trim_x(training, rows, columns);

  double ** transposed;
  int rowsT, columnsT;
  rowsT = columns;
  columnsT = rows;
  transposed = allocate_transpose(training, rows, columns);
  transposed = transpose(training, rows, columns);


  int rowsXTX=columns;

  double ** xTx;
  xTx =  multiply(transposed, training, rowsT, columnsT);

  double ** inverseXTX;
  inverseXTX = inverse(xTx, rowsXTX, rowsXTX);

  double ** everythingButY;
  everythingButY = multiply2(inverseXTX, transposed, rowsXTX, rowsXTX,  columnsT);

  double * W; 
  W = multiply4(everythingButY, housePrices, rowsXTX, columnsT, rows);

  testing = add_leading_one( testing, testPoints, columnsTest);
  test(W, testing, columns , testPoints, columnsTest+1);


  free_matrix(training, rows);
  free_matrix(testing, testPoints);
  free_matrix(everythingButY,rowsXTX);
  free_matrix(inverseXTX, rowsXTX);
  free_matrix(transposed, rowsT);
  free_list(W,columns);
  free_list(housePrices,columns);
  free(trainData);
  free(testData);
  return 0;
}
