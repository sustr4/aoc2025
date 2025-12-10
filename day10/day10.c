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
#define MAXJOLT 57
#define INPUT "input.txt"
#define MAXX 164
//#define INPUT "unit1.txt"
//#define MAXX 4
//#define INPUT "unit2.txt"
//#define MAXX 151
//#define INPUT "unit3.txt"
//#define MAXX 2


// Point structure definition
typedef struct {
	char *lightstring;
	unsigned int target;
	char **butstring;
	unsigned int *button;
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
		inst[count].lightstring=strdup(token+1);
		inst[count].lightstring[strlen(inst[count].lightstring)-1]=0;
		for(int n=0; n<strlen(inst[count].lightstring); n++) {
			if (inst[count].lightstring[n]=='#') inst[count].target|= 1 << n;
		}
		inst[count].button=(unsigned int*)calloc(MAXBUT, sizeof(unsigned int));
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
			token = strtok(inst[count].butstring[i], ",");
			inst[count].button[i]|=1 << atoi(token);
			while( 1 ) {
				if(!(token = strtok(NULL, ","))) break;
				inst[count].button[i]|=1 << atoi(token);
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

int press(TMach *mach, int i, unsigned int state, int depth, int maxdepth) {

	if(depth>maxdepth) return 0;

	if(state==mach[i].target) {
//		printf("M%d found at depth %d (%d == %d)\n", i, depth, state, mach[i].target);
		return 1;
	}

	// Try all buttons:
	for(int y=0; mach[i].butstring[y]; y++) {
		if(press(mach, i, state ^ mach[i].button[y], depth+1, maxdepth)) return 1;
	}
	

	return 0;
}

int jolt(TMach *mach, int i, unsigned int *state, int depth, int maxdepth) {

	if(depth>maxdepth) return 0;

	int zeroes=1;
	for(int y=0; y<strlen(mach[i].lightstring); y++) {
		if(state[y]<0) return 0;
		if(state[y]) {
			zeroes=0;
			break;
		}
	}

	if(zeroes) {
		printf("M%d found at depth %d\n", i, depth);
		return 1;
	}
		
	// Try all buttons:
	for(int y=0; mach[i].butstring[y]; y++) {
		for(int b=0; b<strlen(mach[i].lightstring); b++)
			if(mach[i].button[y] & (1 << b)) state[y]--;
		if(jolt(mach, i, state, depth+1, maxdepth)) return 1;
		for(int b=0; b<strlen(mach[i].lightstring); b++)
			if(mach[i].button[y] & (1 << b)) state[y]++;
	}

	return 0;
}

void printMachine(TMach *array, int i) {
	printf("M%d: %s -- %d\n", i, array[i].lightstring, array[i].target);
	for(int y=0; array[i].butstring[y]; y++) {
		printf("\tB%d: %d\n", y, array[i].button[y]);
	}
	printf("\tJ: ");
	for(int l=0; (l<MAXJOLT) && array[i].joltage[l]>=0; l++) printf("%d,", array[i].joltage[l]);
	printf("\n");
}

int main(int argc, char *argv[]) {

	TMach *array = readInput();
	int sum=0;

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(int i=0; array[i].lightstring; i++) {

		printMachine(array, i);

		int d=1;
		while(1) {
			if(press(array, i, 0, 0, d)) break;
			d++;
		}
		sum+=d;

		int jolts=0;
		for(jolts = 0; array[i].joltage[jolts]>=0; jolts++);
		int buttons=0;
		for(buttons = 0; array[i].butstring[buttons]; buttons++);
		d=0;
		for(int j=0; j<jolts; j++) if(d<array[i].joltage[j]) d=array[i].joltage[j];

		printf("M%d: %d buttons x %d joltage meters, start at %d\n", i, buttons, jolts, d);

		char *fname;
		asprintf(&fname, "%03d.sm2", i);
		FILE *f = fopen(fname, "wt");
		if (f == NULL) {
			perror("Failed to open file");
			return 1;
		}

		fprintf(f, "(set-logic QF_LIA)\n; Declare variables\n");
		for(int b=0; b<buttons; b++) {
			fprintf(f, "(declare-const %c Int)\n",'a'+b);
		}

		fprintf(f, "\n; Define system of equations\n");
		for(int b=0; b<buttons; b++) {
			fprintf(f, "(assert (>= %c 0))\n",'a'+b);
		}
		for(int j=0; j<jolts; j++) {
			fprintf(f,"(assert (= %d (+ ", array[i].joltage[j]);
			for(int b=0; b<buttons; b++) {
				if(array[i].button[b] & (1 << j)) {
					fprintf(f, "%c ", 'a'+b);

				}
			}
			fprintf(f,")))\n");
		}

//		fprintf(f,"(assert (= 12 (+ ");
//		for(int b=0; b<buttons; b++) {
//			fprintf(f, "%c ", 'a'+b);
//		}
//		fprintf(f,")))\n");

		fprintf(f,";Create an optimizer\n(minimize (+ ");
		for(int b=0; b<buttons; b++) {
			fprintf(f, "%c ", 'a'+b);
		}
		fprintf(f,"))\n(set-option :opt.priority pareto)\n");

		fprintf(f, "\n; Check satisfiability and get the model\n(check-sat)\n(get-model)\n(get-objectives)\n");
//		fprintf(f, "\n; Check satisfiability and get the model\n(check-sat)\n(get-objectives)\n");

		fclose(f);
		free(fname);
		
	
	}

	printf("Total presses for lights : %d\n", sum);
	printf("Don't forget to run ./day10-wrap.sh for Task 2\n");
	return 0;
}
