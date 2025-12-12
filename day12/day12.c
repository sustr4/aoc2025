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
#define MAXY 6
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int maxx;
	int maxy;
	int req[6];
} TRegion;

typedef char** TMap;
typedef struct {
	TMap map;
	TMap rot[4];
} TShape;

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
void printMap (char **map, int maxx, int maxy) {
	int x,y;
	for(y=0; y<maxy; y++) {
		for(x=0; x<maxx; x++) {
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

char **cleanMap(int x, int y) {
	char **map=calloc(y,sizeof(char*));
	for(int iter=0; iter<y; iter++) map[iter]=calloc(x,sizeof(char));

	return map;
}

// Read input file line by line (e.g., into an array)
int readInput(TRegion *inst, TShape *shape) {
//int readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	int count = -1;
	int shapes = 0;
	int sl=0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;

	while (getline(&line, &len, input) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL
		
		if(shapes<6) {
			if(!shape[shapes].map) shape[shapes].map=cleanMap(3,3);
				
			if((line[0]=='.') || (line[0]=='#')) {
				for(int x=0; x<strlen(line); x++) shape[shapes].map[sl][x] = line[x];
				sl++;
			}
			if(strlen(line)<2) {
				printf("Shape %d read\n", shapes);
				shapes++;
				sl=0;
			}
		}
		else {
			if(count<0) count=0;

			char *token;
			token = strtok(line, " ");
			sscanf(token,"%dx%d",
				&(inst[count].maxx),
				&(inst[count].maxy));
			
			int r=0;
			while( 1 ) {
				if(!(token = strtok(NULL, " "))) break;
				inst[count].req[r++]=atoi(token);
			}
			count++;
		}

	}

	fclose(input);
	if (line) free(line);

	return 0;
}

int main(int argc, char *argv[]) {

	TRegion *reg=(TRegion*)calloc(MAXX, sizeof(TRegion));
	TShape *shape=(TShape*)calloc(MAXY, sizeof(TShape));

	readInput(reg, shape);

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(int i=0; reg[i].maxx; i++) {
		printf("%dx%d", reg[i].maxx, reg[i].maxy);
		for(int k=0; k<6; k++) printf(" %d", reg[i].req[k]);
		printf("\n");
	}

	for(int i=0; i<MAXY; i++) {
		printf("%d:\n", i);
		printMap(shape[i].map, 3, 3);
	}

	return 0;
}
