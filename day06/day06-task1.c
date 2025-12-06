#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 2000
#define MAXY 5
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TPoint;

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
TPoint *readInput(int **num, char*op) {
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

		int x=0;

		// Read tokens from single line
		char *token;
		token = strtok(line, " ");
		if((token[0]=='+') || (token[0]=='*'))
			op[x++]=token[0];
		else num[count][x++]=atoi(token);
		while( 1 ) {
			if(!(token = strtok(NULL, " "))) break;
			if((token[0]=='+') || (token[0]=='*'))
				op[x++]=token[0];
			else num[count][x++]=atoi(token);
		}

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

	char *op=(char*)calloc(MAXX, sizeof(char));
	int **num=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) num[iter]=calloc(MAXX,sizeof(int));

//	TPoint *array;
	int i=0;	
//	array = readInput();
	readInput(num, op);
	long long sum = 0;

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; num[0][i]; i++) {
		long long res=num[0][i];
		if(op[i]=='+') for(int y=1;num[y][i];y++) res+=num[y][i];
		if(op[i]=='*') for(int y=1;num[y][i];y++) res*=num[y][i];
		sum+=res;
	}

	printf("%lld\n", sum);
	return 0;
}
