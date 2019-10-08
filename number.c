#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * fp;

void printSubset(int subSet[], int size, int subAdd[], int index, int check)
{
  if(size == 0)
    return;
  if(check == 1)
  {
    if(size == 1)
    {
      fprintf(fp,"(=");
      fprintf(fp," B%d_%d",subAdd[0]+1,index);
      fprintf(fp," %d) ",size);
    }
    else
    {
      fprintf(fp,"(=(+");
      for(int i = 0; i < size; i++) {
        fprintf(fp," B%d_%d",subAdd[i]+1,index);
      }
      fprintf(fp,")%d) ",size); // col case
    }

  }
  else
  {
    if(size == 1)
    {
      fprintf(fp,"(=");
      fprintf(fp," B%d_%d",index,subAdd[0]+1);
      fprintf(fp," %d) ",0);
    }
    else
    {
      fprintf(fp,"(=(+");
      for(int i = 0; i < size; i++) {
        fprintf(fp," B%d_%d",index,subAdd[i]+1);
      }
      fprintf(fp,")%d) ",0); // col case
    }

  }
}

void subsetcase(int set[], int subSet[], int n, int subSize, int total, int count ,int sum, int subAdd[], int index, int check)
{
    if(total == sum) { // find case
       printSubset(subSet, subSize, subAdd, index, check); // print the subset
       return;
    }
    else if(total > sum) // skip case
      return;
    else {
      for(int i = count; i < n; i++ ) {
        subSet[subSize] = set[i];
        subAdd[subSize] = i;
        subsetcase(set, subSet, n, subSize+1, total+set[i], i+1, sum, subAdd, index, check); // search next subset
       }
    }
}

void searchSubset(int set[], int size, int sum, int index, int check)
{
    int *subSet = (int*)malloc(size * sizeof(int)); //create subset array to pass parameter of subsetcase
    int *subAdd = (int*)malloc(size * sizeof(int)); //create subaddress array to pass parameter of subsetcase
    subsetcase(set, subSet, size, 0, 0, 0, sum, subAdd, index, check);
    free(subSet);
    free(subAdd);
}

int main(int argc, char *argv[])
{
  char str[1024], *token;
  int row = 0, col = 0;
  int **a = NULL;
  int *temp_row = NULL;
  int *temp_col = NULL;
  int *label_row = NULL;
  int *label_col = NULL;
  int count = 0; //
  int label_num = 0;
  FILE * fpc = NULL;

  fp = fopen("formula","w");

  if(argc > 1)
    fpc = fopen(argv[1],"r");
  else
  {
    printf("not a form\n");
    exit(-1);
  }

  while(fgets(str,1024,fpc) != NULL)
  {
    count++;

    if(count == 1)
    {
      str[strlen(str)-1] = 0; // delete ‘\n’
      token = strtok(str, " ");
      while(token) {
        label_num++;
        token = strtok(NULL, " ");
      }
    }

    else
    {
      str[strlen(str)-1] = 0; // delete ‘\n’
      if(str[0] == 0)
        break;
      row++;
      token = strtok(str, " ");
      while(token) {
        col++;
        token = strtok(NULL, " ");
      }
    }
  }
  col += 1;
  col = (col - row) / row;

  fclose(fpc);

  label_row = (int*)malloc(col * sizeof (int));
  label_col = (int*)malloc(row * sizeof (int));

  a = (int**)malloc(row * sizeof (*a));
  for (int j = 0; j < row; j++)
	 a[j] = (int*)malloc(col * sizeof(**a));

  FILE * fpd = fopen(argv[1],"r");

  for(int i = 0; i < col; i++)
    fscanf(fpd,"%d",&label_row[i]);

  for(int i = 0; i < row; i++)
    for(int j = 0; j < col + 1; j++)
    {
      if(j == 9)
        fscanf(fpd,"%d",&label_col[i]);
      else
        fscanf(fpd,"%d",&a[i][j]);
    }
  fclose(fpd);
/*
  Matrix and label value check
*/
/*
  for(int i = 0; i < col; i++)
    printf("%d\n",label_row[i]);
  for(int i = 0; i < row; i++)
    printf("%d\n",label_col[i]);
  for(int i = 0; i < 9; i++){
  	for(int j = 0; j < 9; j++)
  		printf("%d ",a[i][j]);
    printf("\n");}
*/
  temp_row = (int*)malloc(row * sizeof (int));
  temp_col = (int*)malloc(col * sizeof (int));
  // assert statement declare
  for (int i = 1 ; i <= row ; i++)
      for (int j = 1 ; j <= col ; j++)
        fprintf(fp,"(declare-const B%d_%d Int)\n", i, j) ;
  for (int i = 1; i <= row; i++)
      for(int j = 1; j <= col; j++)
  		  fprintf(fp,"(assert(and (<= B%d_%d 1) (<= 0 B%d_%d)))\n", i, j, i, j);
  // col case check
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < row; j++)
      temp_row[j] = a[j][i];

    fprintf(fp,"(assert(or ");
    searchSubset(temp_row, row, label_row[i], i+1, 1);
    fprintf(fp,"))\n");
  }
  free(temp_row);
  free(label_row);
  // row case check
  for (int i = 0; i < col; i++)
  {
    for (int j = 0; j < col; j++)
      temp_col[j] = a[i][j];

    fprintf(fp,"(assert(or ");
    searchSubset(temp_col, col, label_col[i], i+1, 0);
    fprintf(fp,"))\n");
  }
  free(temp_col);
  free(label_col);

  fprintf(fp,"(check-sat)\n(get-model)\n");
  fclose(fp);

  for (int i = 0; i < row; i++) // free memory for each row
    free(a[i]);
  free(a); // free memory for row pointers

  FILE * fin = popen("z3 formula","r");

  char board[row + 1][col + 1];
  int i, j, z;
  char b[128] ;
  char s[128] ;
  char n[128] ;
  char q[128] ;
  char o,p;

  fscanf(fin,"%s %s", q, b) ;

  if(strcmp(q,"sat") == 0)
  {
    for (z = 0 ; z < row*col ; z++)
    {
      fscanf(fin,"%s %s %s %s %s", b, s, b, b, n);
      sscanf(s,"%c%d%c%d",&o,&i,&p,&j);

      if (strcmp(n, "0)") != 0)
        board[i][j] = 1 ;
      else
        board[i][j] = 0;
    }

    for (i = 1 ; i <= row ; i++)
    {
      for (j = 1 ; j <= col ; j++)
        printf("%d ", board[i][j]) ;
      printf("\n") ;
     }
  }
  else
  {
    printf("No solution!\n");
    exit(-1);
  }

  pclose(fin);
}
