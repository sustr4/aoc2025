#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 2000
#define MAXY 5
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Read input file line by line (e.g., into an array)
int readInput(int **num, char*op) {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		int x=0;
		// Read tokens from single line
		char *token;
		token = strtok(line, " ");
		if((token[0]=='+') || (token[0]=='*'))
			op[x++]=token[0];
		else num[count][x++]=atoi(token);
		while( 1 ) {
			if(!(token = strtok(NULL, " "))) break;
			if((token[0]=='+') || (token[0]=='*'))
				op[x++]=token[0];
			else num[count][x++]=atoi(token);
		}

		count++;
	}

	fclose(input);
	if (line) free(line);

	return 0;
}

int main(int argc, char *argv[]) {

	char *op=(char*)calloc(MAXX, sizeof(char));
	int **num=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) num[iter]=calloc(MAXX,sizeof(int));

	readInput(num, op);
	long long sum = 0;

	for(int i=0; num[0][i]; i++) {
		long long res=num[0][i];
		if(op[i]=='+') for(int y=1;num[y][i];y++) res+=num[y][i];
		if(op[i]=='*') for(int y=1;num[y][i];y++) res*=num[y][i];
		sum+=res;
	}

	printf("%lld\n", sum);
	return 0;
}
