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
	int size;
} TShape;

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

char **cleanMap(int x, int y) {
	char **map=calloc(y,sizeof(char*));
	for(int iter=0; iter<y; iter++) map[iter]=calloc(x,sizeof(char));

	return map;
}

int fillRotations(TShape *shape, int i) {
	shape[i].rot[0]=shape[i].map;

	for(int r=1; r<4; r++) {
		shape[i].rot[r]=cleanMap(3,3);
		for(int y=0; y<3; y++)
			for(int x=0; x<3; x++)
				shape[i].rot[r][y][x]=shape[i].rot[r-1][x][2-y];
			
	}

	return 0;
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
				for(int x=0; x<strlen(line); x++) {
					shape[shapes].map[sl][x] = line[x];
					if(line[x]=='#') shape[shapes].size++;
				}
				sl++;
			}
			if(strlen(line)<2) {
				fillRotations(shape, shapes);
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
	int fit=0;
	int never=0;
	int easy=0;

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(int i=0; reg[i].maxx; i++) {
		printf("%dx%d", reg[i].maxx, reg[i].maxy);
		for(int k=0; k<6; k++) printf(" %d", reg[i].req[k]);
		printf("\n");
	}

	for(int i=0; i<MAXY; i++) {
		printf("%d (%d boxes):\n", i, shape[i].size);
		printMap(shape[i].rot[1], 3, 3);
	}

	for(int i=0; reg[i].maxx; i++) {
		//Sanity check:
		int sum=0;
		int totboxes=0;
		for(int y=0; y<6; y++) {
			sum+=reg[i].req[y]*shape[y].size;
			totboxes+=reg[i].req[y];
		}

		if((reg[i].maxx*reg[i].maxy)<sum) {
			never++;
			continue;
		}
		
		//Easy cases:
		if(((reg[i].maxx/3) * (reg[i].maxy/3))>=totboxes) easy++;
		fit++;
	}
	
	fprintf(stderr,"%d cases will never fit\n", never);
	fprintf(stderr,"%d cases will always fit easily\n", easy);
	printf("%d can fit\n",fit);

	return 0;
}
