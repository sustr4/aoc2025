#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 645
#define MAXY 26
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	char *name;
	int *conn;
	int paths;
} TDev;

int id(TDev* dev, char *name) {

	int i=1;
	while(dev[i].name) {
		if(!strcmp(name,dev[i].name)) return i;
		i++;
	}

	dev[i].name=strdup(name);
	dev[i].conn = (int*)calloc(MAXY,sizeof(int));
	dev[i].paths=-1;
	return i;
}

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
TDev *readInput() {
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
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	TDev *inst=(TDev*)calloc(MAXX, sizeof(TDev));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
	// char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));

		// Read tokens from single line
		char *token;
		token = strtok(line, " ");
		token[3]=0;
		int i = id(inst, token);
		int y = 0;
		while( 1 ) {
			if(!(token = strtok(NULL, " "))) break;
			inst[i].conn[y++]=id(inst, token);
		}

		count++;
	}

	fclose(input);
	if (line) free(line);

	return inst;
}

int step(TDev *dev, int i, int O, int deep) {
	if(deep>MAXX) return 0;

	if(dev[i].paths>=0) return dev[i].paths;

	if(i==O) return 1;

	int retval=0;
	for(int y=0; dev[i].conn[y]; y++) {
		retval+=step(dev, dev[i].conn[y], O, deep+1);
	}

	return retval;
}

int main(int argc, char *argv[]) {

	TDev *array = readInput();

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(int i=1; array[i].name; i++) { // XXX: Run from 1!
//		printf("%d: %s\t", i, array[i].name);
//		for(int y=0; array[i].conn[y]; y++) printf("%s ", array[array[i].conn[y]].name);
//		printf("\n");
//	}

	int S=id(array, "you");
	int O=id(array, "out");
	printf("Starting from %d to %d\n", S, O);

	int c=step(array, S, O, 0);

	printf("Fount %d paths\n", c);
	return 0;
}
