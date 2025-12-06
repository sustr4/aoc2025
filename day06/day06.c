#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 4000
#define MAXY 5
#define DIG 4
//#define INPUT "unit1.txt"
//#define MAXX 15
//#define MAXY 4
//#define DIG 3

// Read input file line by line (e.g., into an array)
char **readInput() {
//int readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0;
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x];
		y++;
		count++;
	}

	fclose(input);
	if (line) free(line);

	return map;
}

int main(int argc, char *argv[]) {

	char **map=readInput();

	int *num=(int*)calloc(MAXX,sizeof(int));

	long long sum=0, pres=0, tres=1;

	for(int y=MAXX;y>=0;y--) {
		for(int i=0;i<DIG;i++) {
			if((map[i][y]>='0') && (map[i][y]<='9')) {
				num[y]=num[y]*10+(int)(map[i][y]-'0');
			}
		}
		if(num[y]) {
			pres+=num[y];
			tres*=num[y];
		} else {
			pres=0;
			tres=1;
		}
		if(map[DIG][y]=='+') sum+=pres;
		if(map[DIG][y]=='*') sum+=tres;
		
	}

	printf("%lld\n",sum);

	return 0;
}
