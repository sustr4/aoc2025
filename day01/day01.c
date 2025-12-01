#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 10000

// Point structure definition
typedef struct {
	char dir;
	int num;
} TInst;

// Read input file line by line (e.g., into an array)
TInst *readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	TInst *inst=(TInst*)calloc(MAXX, sizeof(TInst));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into array
		sscanf(line,"%c%d",
			&(inst[count].dir),
			&(inst[count].num));

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

int main(int argc, char *argv[]) {

	TInst *array;
	int i=0;	
	array = readInput();
	int cur = 50, count = 0;

	for(i=0; array[i].dir; i++) {
		printf("%c %2d\t", array[i].dir, array[i].num);
	
		for(int y=0;y<array[i].num;y++) {
			cur+=(array[i].dir=='R') ? 1 : -1;
			if(cur==100) cur=0;
			if(cur==-1) cur=99;
			if(cur==0) count++;
		}

		printf("%d\n", cur);
	}

	
	printf("%d\n", count);

	return 0;
}
