#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>
#include<math.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 1000
#define FIRST 1000
//#define INPUT "unit1.txt"
//#define MAXX 20
//#define FIRST 10

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TPoint;

typedef struct {
	int a;
	int b;
	double dist;
	int conn;
} TPair;

double dist(TPoint a, TPoint b) {
	return sqrt(((double)a.x - (double)b.x)*((double)a.x - (double)b.x) + ((double)a.y - (double)b.y)*((double)a.y - (double)b.y) + ((double)a.z - (double)b.z)*((double)a.z - (double)b.z));
//	return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
}

// Comparator function example
int comp(const void *a, const void *b)
{
	const TPair *da = (const TPair *) a;
	const TPair *db = (const TPair *) b;
	return (da->dist > db->dist) - (da->dist < db->dist);
}

// Example for calling qsort()
//qsort(array,count,sizeof(),comp);


// Print a two-dimensional array
void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXX; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █ and border elements ┃━┗┛┏┓
// Color printf("\033[1;31mR \033[1;32mG \033[1;34mB \033[0moff\n");

// Read input file line by line (e.g., into an array)
TPoint *readInput() {
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
	TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

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
		sscanf(line,"%d,%d,%d",
			&(inst[count].x),
			&(inst[count].y),
			&(inst[count].z));

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

	return inst;
}

void interconnect(int isl, int *island, char **map) {
	int change=1;
	while(change) {
		change=0;
		for(int i=0; i<MAXX; i++) {
			if(island[i]!=isl) continue;
//			printf("no island for %d yet\n", i);
			for(int y=0; y<MAXX; y++) {
				if((map[y][i]) && (!island[y])) {
					island[y]=isl;
					printf("island[%d]=%d;\n", y, isl);
					change=1;
				}
			}
		}
	}
}

int main(int argc, char *argv[]) {

	TPoint *array;
	int i=0;	
	array = readInput();
	int count=0;

	TPair *d=(TPair*)calloc(MAXX*MAXX, sizeof(TPair));

	char **map=calloc(MAXX,sizeof(char*));
	for(int iter=0; iter<MAXX; iter++) map[iter]=calloc(MAXX,sizeof(char));

	int *island=(int*)calloc(MAXX, sizeof(int));

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; array[i].x; i++) {
//		printf("[%d,%d,%d]\n", array[i].x, array[i].y, array[i].z);
		for(int y=i; array[y].x; y++) {
			if(i==y) continue;
			d[count].a=i;
			d[count].b=y;
			d[count].dist=dist(array[i], array[y]);
			count++;
		}
	}

	qsort(d,count,sizeof(TPair),comp);

	for(i=0; i<FIRST; i++) {
		d[i].conn=1;
		map[d[i].a][d[i].b]=1;
		map[d[i].b][d[i].a]=1;
		printf("%3d--%3d \t%f \t%d,%d,%d \t%d,%d,%d\n", d[i].a, d[i].b, d[i].dist,
			array[d[i].a].x, array[d[i].a].y, array[d[i].b].z,
			array[d[i].b].x, array[d[i].b].y, array[d[i].b].z);
	}

	int last=1;
	for(i=0; i<MAXX; i++) {
		if(island[i]) continue;
		island[i]=last++;
		interconnect(island[i], island, map);
	}

	printf("%d islands\n", last-1);

	for(i=0; i<MAXX; i++) {
		printf("box %d in island %d\n", i, island[i]);
	}

	return 0;
}
