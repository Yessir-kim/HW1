#include <stdio.h>
#include <string.h>

char board[10][10] ;

int main ()
{
	int i, j, k;

	char b[128] ;

  char s[128] ;

  char n[128] ;

  char q[128] ;

  char o,p;

	scanf("%s %s", b, q) ;
	for (k = 0 ; k < 81 ; k++) {
		scanf("%s %s %s %s %s", b, s, b, b, n) ;
		sscanf(s,"%c%d%c%d",&o,&i,&p,&j);

		if (strcmp(n, "0)") != 0) {
			board[i][j] = 1 ;
		}
  	else board[i][j] = 0;
	}

	for (i = 1 ; i <= 9 ; i++) {
		for (j = 1 ; j <= 9 ; j++) {
			printf("%d ", board[i][j]) ;
		}
		printf("\n") ;
	}
}
