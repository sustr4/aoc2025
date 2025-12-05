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
	const int *da = (const int *) a;
	const int *db = (const int *) b;
	return (*da > *db) - (*da < *db);
}

// Example for calling qsort()
//qsort(array,count,sizeof(),comp);


// Print a two-dimensional array
void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █ and border elements ┃━┗┛┏┓
// Color printf("\033[1;31mR \033[1;32mG \033[1;34mB \033[0moff\n");

// Retrieve nth neighbor from a map, diagonals are odd, side neighbors even
int dy[] = { -1, -1, -1, 0, 1, 1,  1,  0};
int dx[] = { -1,  0,  1, 1, 1, 0, -1, -1};
char mapnb(char **map, int y, int x, int n) {
	assert((n>=0) && (n<8));
	if((y+dy[n]<0) || (y+dy[n]>=MAXY) ||
	   (x+dx[n]<0) || (x+dx[n]>=MAXX)) return 0;
	return(map[y+dy[n]][x+dx[n]]);
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

	// Allocate one-dimensional array of strings

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
	// char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	int ingr=1;

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<2) {
			ingr=0;
			count=0;
			continue;
		}

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string

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

		// Read tokens from single line
		//char *token;
		//token = strtok(line, ",");
		//while( 1 ) {
		//	if(!(token = strtok(NULL, ","))) break;
		//}

		count++;
	}

	fclose(input);
	if (line) free(line);

//	printMap(map);

	return 0;
//	return inst;
//	return map;
}

int main(int argc, char *argv[]) {

	TFresh *fresh=(TFresh*)calloc(MAXX, sizeof(TFresh));
	long long *ing=(long long*)calloc(MAXY, sizeof(TFresh));
//	TFresh *array;
	int i=0;	
	long long int count=0;
	readInput(fresh, ing);

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	int change=1;
	while(change) {
		change=0;
		for(i=0; i<MAXX-1; i++) {
			if((fresh[i].from==0)||(fresh[i].to==0)) continue;
	//		printf("%lld-%lld\n", fresh[i].from, fresh[i].to);
			for(int y=i+1; y<MAXX; y++) {
				if((fresh[y].from==0)||(fresh[y].to==0)) continue;
				if((fresh[i].from<=fresh[y].from) && (fresh[i].to>=fresh[y].from)) { // y Start falls into i
					if(fresh[y].to<=fresh[i].to) { // y end also falls into i
						fresh[y].from=0; fresh[y].to=0;
					}
					else {
						fresh[i].to=fresh[y].to;
						fresh[y].from=0; fresh[y].to=0;
						change++;
					}
				}
				if((fresh[i].from<=fresh[y].to) && (fresh[i].to>=fresh[y].to)) { // y end falls into i
					if(fresh[y].from>=fresh[i].from) { // y start also falls into i
						fresh[y].from=0; fresh[y].to=0;
					}
					else {
						fresh[i].from=fresh[y].from;
						fresh[y].from=0; fresh[y].to=0;
						change++;
					}
				}
	//			printf("\t%lld-%lld\n", fresh[y].from, fresh[y].to);
			}
		}
	}

	for(i=0; i<MAXX-1; i++) {
		if((fresh[i].from==0)||(fresh[i].to==0)) continue;
		printf("%lld-%lld\n", fresh[i].from, fresh[i].to);
		count+=fresh[i].to-fresh[i].from+1;
	}
	printf("358972987438930 too high\n");
	printf("%lld fresh\n", count);

	return 0;
}
