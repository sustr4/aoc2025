#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 141
#define MAXY 142
//#define INPUT "unit1.txt"
//#define MAXX 15
//#define MAXY 16

// Read input file line by line (e.g., into an array)
char **readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

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
	}

	fclose(input);
	if (line) free(line);

	return map;
}

long long go(char **map, int y, int x, long long **mem) {
	long long res=0;

	if(y==MAXY-1) return 1;
	if(mem[y][x]) return mem[y][x];

	if(map[y][x]!='^') res+=go(map,y+1,x,mem);
	else {
		if(x>0) res+=go(map,y+1,x-1,mem);
		if(x<MAXX-1) res+=go(map,y+1,x+1,mem);
	}

	mem[y][x]=res;
	return res;
}

int main(int argc, char *argv[]) {
	char **map = readInput();
	int S=-1;

	long long **mem=calloc(MAXY,sizeof(long long*));
	for(int iter=0; iter<MAXY; iter++) mem[iter]=calloc(MAXX,sizeof(long long));

	for(int x=0; x<MAXY; x++) {
		if(map[0][x]=='S') {
			S=x;
			break;
		}
	}
	assert(S>=0);

	printf("%lld\n", go(map,0,S,mem));

	return 0;
}
