#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 1000
#define MAXY 1000
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	long long int from;
	long long int to;
} TFresh;

// Comparator function example
int comp(const void *a, const void *b)
{
	const TFresh *da = (const TFresh *) a;
	const TFresh *db = (const TFresh *) b;
	return (da->from > db->from) - (da->from < db->from);
}

// Read input file line by line (e.g., into an array)
int readInput(TFresh *fresh, long long int *ing) {
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

	int ingr=1;

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<2) {
			ingr=0;
			count=0;
			continue;
		}

		if(ingr) {
			// Read into array
			sscanf(line,"%lld-%lld",
				&(fresh[count].from),
				&(fresh[count].to));
		}
		else {
			// Read into array
			sscanf(line,"%lld",
				&(ing[count]));
		}

		count++;
	}

	fclose(input);
	if (line) free(line);

	return 0;
}

int main(int argc, char *argv[]) {

	TFresh *fresh=(TFresh*)calloc(MAXX, sizeof(TFresh));
	long long *ing=(long long*)calloc(MAXY, sizeof(TFresh));
	int i=0;	
	long long int count=0;
	readInput(fresh, ing);

	for(i=0; fresh[i].from; i++);

	qsort(fresh,i,sizeof(TFresh),comp);
	int change=1;
	while(change) {
		change=0;
		for(i=0; i<MAXX-1; i++) {
			if((fresh[i].from==0)||(fresh[i].to==0)) continue;
			for(int y=i+1; y<MAXX; y++) {
				if((fresh[y].from==0)||(fresh[y].to==0)) continue;
				if((fresh[i].from<=fresh[y].from) && (fresh[i].to>=fresh[y].from)) { // y Start falls into i
					if(fresh[y].to>fresh[i].to) { // y end also falls into i
						fresh[i].to=fresh[y].to;
						change++;
					}
					fresh[y].from=0; fresh[y].to=0;
				}
				if((fresh[i].from<=fresh[y].to) && (fresh[i].to>=fresh[y].to)) { // y end falls into i
					if(fresh[y].from<fresh[i].from) { // y start also falls into i
						fresh[i].from=fresh[y].from;
						change++;
					}
					fresh[y].from=0; fresh[y].to=0;
				}
			}
		}
	}

	for(i=0; i<MAXX-1; i++) {
		if((fresh[i].from==0)||(fresh[i].to==0)) continue;
		printf("%lld-%lld\n", fresh[i].from, fresh[i].to);
		count+=fresh[i].to-fresh[i].from+1;
	}
	printf("%lld fresh\n", count);

	return 0;
}
