#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 100
#define MAXY 200
#define MAXBAT 12

// Read input file line by line (e.g., into an array)
char **readInput() {
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

	int i, x, b, maxpos[MAXBAT];
	char **bat = readInput();
	char max[MAXBAT];
	long long sum=0;

	for(i=0; i<MAXY; i++) {
		for(b=0; b<MAXBAT; b++) {
			max[b]='0';
			maxpos[b]=-1;
			int from = (b == 0) ? 0 : maxpos[b-1]+1;
			for(x=from; x<MAXX-MAXBAT+b+1; x++) {
				if(max[b]<bat[i][x]) {
					max[b]=bat[i][x];
					maxpos[b]=x;
				}
			}

		}
		long long line=0;
		for(b=0; b<MAXBAT; b++) line=line*10+(long long int)(max[b]-'0');
		sum+=line;
	}

	printf("%lld\n", sum);

	return 0;
}
