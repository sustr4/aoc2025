#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
// Max. 13 buttons, max. 9 lights operated by each
// Max. 56 joltage counts 
#define MAXBUT 14
#define MAXLIGHT 10
#define MAXJOLT 57
#define INPUT "input.txt"
#define MAXX 164
#define MAXY 26
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	char *light;
	char **butstring;
	int **button;
	char *joltstring;
	int *joltage;
} TMach;

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
TMach *readInput() {
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
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	TMach *inst=(TMach*)calloc(MAXX, sizeof(TMach));

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

		// Max. 13 buttons, max. 9 lights operated by each
		// Max. 56 joltage counts 

		// Read tokens from single line
		char *token;
		token = strtok(line, " ");
		inst[count].light=strdup(token+1);
		inst[count].light[strlen(inst[count].light)-1]=0;
		inst[count].button=(int**)calloc(MAXBUT, sizeof(int*));
		inst[count].butstring=(char**)calloc(MAXBUT, sizeof(char*));
		int bcount=0;
		while( 1 ) {
			if(!(token = strtok(NULL, " "))) break;
			if(token[0]=='(') {
				token[strlen(token)-1]=0;
				inst[count].butstring[bcount++] = strdup(token+1);
			}
			else if (token[0]=='{') {
				token[strlen(token)-1]=0;
				inst[count].joltstring=strdup(token+1);
			}
		}

		for(int i=0; inst[count].butstring[i]; i++) {
			bcount=0;
			inst[count].button[i]=(int*)malloc(MAXLIGHT*sizeof(int));
			for(int n=0; n<MAXLIGHT; n++) inst[count].button[i][n]=-1;
			token = strtok(inst[count].butstring[i], ",");
			inst[count].button[i][bcount++]=atoi(token);
			while( 1 ) {
				if(!(token = strtok(NULL, ","))) break;
				inst[count].button[i][bcount++]=atoi(token);
			}
		}

		// Read Joltages
		bcount=0;
		inst[count].joltage=(int*)malloc(MAXJOLT*sizeof(int));
		for(int n=0; n<MAXJOLT; n++) inst[count].joltage[n]=-1;
		token = strtok(inst[count].joltstring, ",");
		inst[count].joltage[bcount++]=atoi(token);
		while( 1 ) {
			if(!(token = strtok(NULL, ","))) break;
			inst[count].joltage[bcount++]=atoi(token);
		}

		count++;
	}

	fclose(input);
	if (line) free(line);

//	printMap(map);

	return inst;
//	return map;
}

int main(int argc, char *argv[]) {

	TMach *array = readInput();

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(int i=0; array[i].light; i++) {
		printf("M%d: %s\n", i, array[i].light);
		for(int y=0; array[i].butstring[y]; y++) {
			printf("\tB%d: ", y);
			for(int l=0; (l<MAXLIGHT) && array[i].button[y][l]>=0; l++) printf("%d,", array[i].button[y][l]);
			printf("\n");
		}
		printf("\tJ: ");
		for(int l=0; (l<MAXJOLT) && array[i].joltage[l]>=0; l++) printf("%d,", array[i].joltage[l]);
		printf("\n");
	}

	return 0;
}
