#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 1000
//#define INPUT "unit1.txt"

// Point structure definition
typedef struct {
	long int from;
	long int to;
} TRange;

// Read input file line by line (e.g., into an array)
TRange *readInput() {
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

	// Allocate one-dimensional array of strings
	TRange *inst=(TRange*)calloc(MAXX, sizeof(TRange));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read tokens from single line
		char *token;
		token = strtok(line, ",");
		sscanf(token,"%ld-%ld",
			&(inst[count].from),
			&(inst[count].to));
		count++;
		while( 1 ) {
			if(!(token = strtok(NULL, ","))) break;
			sscanf(token,"%ld-%ld",
				&(inst[count].from),
				&(inst[count].to));
			count++;
		}

	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

int main(int argc, char *argv[]) {

	TRange *array;
	int i=0;	
	array = readInput();
	long int count=0;
	char *s;

	for(i=0; array[i].from; i++) {
		//printf("%ld %ld\n", array[i].from, array[i].to);
		for(long int y=array[i].from; y<=array[i].to; y++) {
			asprintf(&s, "%ld", y);
			for(int parts=2; parts<=strlen(s); parts++) { // Try for n. consecutive parts
				if(strlen(s) % parts) continue;
				int fits=1;
				for(int p=0; p<parts-1; p++) {
					if(strncmp(s+p*strlen(s)/parts, s+(p+1)*strlen(s)/parts, strlen(s)/parts)) {
						fits=0;
						break;
					}
				}
				if(fits) {
					printf("%s\n", s);
					count+=y;
					break;
				}
			}
			free(s);
		}
	}

	printf("IDs: %ld\n", count);
	return 0;
}
