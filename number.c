#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FILE * fp;
int check;

void printSubset(int subSet[], int size, int subAdd[], int index, int check) {
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

void subsetSum(int set[], int subSet[], int n, int subSize, int total, int count ,int sum, int subAdd[], int index, int check) {
    if(total == sum) { // corrext case
       printSubset(subSet, subSize, subAdd, index, check);     // print the subset
       return;
    }
    else if(total > sum) // skip case
      return;
    else {
      for(int i = count; i < n; i++ ) {
        subSet[subSize] = set[i];
        subAdd[subSize] = i;
        subsetSum(set, subSet, n, subSize+1, total+set[i], i+1, sum, subAdd, index, check);     // search next subset
       }
    }
}

void findSubset(int set[], int size, int sum, int index, int check) {
    int *subSet = (int*)malloc(size * sizeof(int));     //create subset array to pass parameter of subsetSum
    int *subAdd = (int*)malloc(size * sizeof(int));     //create subaddress array to pass parameter of subsetSum
    subsetSum(set, subSet, size, 0, 0, 0, sum, subAdd, index, check);
    free(subSet);
}

int main(int argc, char *argv[]) {
  char str[1024], *token;
  int row = 0, col = 0;
  int **a = NULL;
  char buffer[1024];
  char c;
  int k = 0, l = 0;
  int *temp_row = NULL;
  int *temp_col = NULL;
  int count = 0;
  int col_count = 0;
  int label_num = 0;
  int *label_row = NULL;
  int *label_col = NULL;
  int check_num = 0;
  int already = 0;

  fp = fopen("formula","w");

  FILE * fpc = NULL;
  if(argc>1)
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
        //printf("%d\n",col);
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
        //printf("%d\n",col);
        token = strtok(NULL, " ");
      }
      //printf("\n");
    }
  }
  //printf("%d %d",row,col);
  col += 1;
  col = (col - row) / row;
  //printf("%d %d\n",row,col);
  fclose(fpc);

  count = 0;
  label_row = (int*)malloc(label_num * sizeof (int));
  label_col = (int*)malloc(row * sizeof (int));

  a = (int**)malloc(row * sizeof (*a));
  for (int j = 0; j < row; j++)
	 a[j] = (int*)malloc(col * sizeof(**a));

  for(int i = 0; i < row; i++)
   	for(int j = 0; j < col; j++)
   		 a[i][j] = 0;
  for(int i = 0; i < label_num; i++)
    label_row[i] = 0;
  for(int j = 0; j < row; j++)
    label_col[j] = 0;

  FILE * fpd = fopen(argv[1],"r");

  while(1)
  {
    c = fgetc(fpd);
    if(feof(fpd)) break;
  //  printf("%c\n",c);
    if(count < col)
    {
      if(c == ' ') {check_num++; count++; continue;}
      else if(c == '\n') {count++; continue;}
      else
      {
        label_row[check_num]*=10;
        label_row[check_num]+=atoi(&c);
        //printf("%d\n",label_row[l]);
      }
    }
    else
    {
      if(c == ' ') {l++; col_count++; continue;}
      else if(c == '\n') {k++; l = 0; already++; col_count = 0; continue;}
      else
      {
        if(col_count >= col)
        {
          label_col[already]*=10;
          label_col[already]+=atoi(&c);
        }
        else
        {
          a[k][l]*=10;
          a[k][l]+=atoi(&c);
        //  printf("%d\n",a[k][l]);
        }
      }
    }
  }
//  for(int i = 0; i < label_num; i++)
//    printf("%d\n",label_row[i]);
//  for(int i = 0; i < row; i++)
//    printf("%d\n",label_col[i]);
  fclose(fpd);
  //행렬 테스트
/*for(int i = 0; i < 9; i++){
  	for(int j = 0; j < 9; j++)
  		printf("%d ",a[i][j]);
    printf("\n");
  }*/
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
    {
      temp_row[j] = a[j][i];
      //printf("%d ",temp_row[j]);
    }
    //printf("\n");
    fprintf(fp,"(assert(or ");
    findSubset(temp_row, row, label_row[i], i+1, 1);
    fprintf(fp,"))\n");
  }
  free(temp_row);
  free(label_row);
  // row case
  for (int i = 0; i < col; i++)
  {
    for (int j = 0; j < col; j++)
    {
      temp_col[j] = a[i][j];
      //printf("%d ",temp_row[j]);
    }
    //printf("\n");
    fprintf(fp,"(assert(or ");
    findSubset(temp_col, col, label_col[i], i+1, 0);
    fprintf(fp,"))\n");
  }
  free(temp_col);
  free(label_col);

  fprintf(fp,"(check-sat)\n(get-model)\n");
  fclose(fp);

  for (int i = 0; i < row; i++) // free memory for each row
    free(a[i]);
  free(a); // free memory for row pointers

  /*FILE * fin = popen("z3 formula","r");

  char board[row + 1][col + 1];

  int i, j, z;

  char b[128] ;

  char s[128] ;

  char n[128] ;

  char q[128] ;

  char o,p;

  scanf("%s %s", q, b) ;

  if(strcmp(q,"unsat") == 0)
  {
    printf("No solution!\n");
    exit(-1);
  }
	for (z = 0 ; z < row*col ; z++) {
		scanf("%s %s %s %s %s", b, s, b, b, n) ;
		sscanf(s,"%c%d%c%d",&o,&i,&p,&j);

		if (strcmp(n, "0)") != 0) {
			board[i][j] = 1 ;
		}
  	else board[i][j] = 0;
	}

	for (i = 1 ; i <= row ; i++) {
		for (j = 1 ; j <= col ; j++) {
			printf("%d ", board[i][j]) ;
		}
		printf("\n") ;
	}
  pclose(fin); */
}
