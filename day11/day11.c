#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 645
#define MAXY 26

// Point structure definition
typedef struct {
	char *name;
	int *conn;
	long long paths[2][2];
	int considered;
} TDev;

int id(TDev* dev, char *name) {
	int i;
	for(i = 1; dev[i].name; i++) if(!strcmp(name,dev[i].name)) return i;

	dev[i].name=strdup(name);
	dev[i].conn = (int*)calloc(MAXY,sizeof(int));
	for(int y=0; y<2; y++) 
		for(int x=0; x<2; x++)
			dev[i].paths[y][x]=-1;
	return i;
}

// Read input file line by line (e.g., into an array)
TDev *readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	TDev *inst=(TDev*)calloc(MAXX, sizeof(TDev));

	while (getline(&line, &len, input) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL
		// Read tokens from single line
		char *token = strtok(line, " ");
		token[3]=0;
		int i = id(inst, token);
		for(int y=0; 1; y++) {
			if(!(token = strtok(NULL, " "))) break;
			inst[i].conn[y]=id(inst, token);
		}
	}

	fclose(input);
	if (line) free(line);

	return inst;
}

long long step(TDev *dev, int i, int O, int D, int F, int dacsat, int fftsat) {

	if(dev[i].paths[dacsat][fftsat]>=0) return dev[i].paths[dacsat][fftsat];

	if((i==O) && dacsat && fftsat) return 1;
	if(i==O) return 0;

	int ds = (i==D) || dacsat;
	int fs = (i==F) || fftsat;

	long long retval=0;
	dev[i].considered=1;
	for(int y=0; dev[i].conn[y]; y++) {
		if(dev[dev[i].conn[y]].considered) continue;
		retval+=step(dev, dev[i].conn[y], O, D, F, ds, fs);
	}
	dev[i].considered=0;
	dev[i].paths[ds][fs]=retval;

	return retval;
}

int main(int argc, char *argv[]) {

	TDev *array = readInput();
	
	printf("Found %lld paths\n", step(array,id(array, "svr"), id(array, "out"), id(array, "dac"), id(array, "fft"), 0, 0));
	return 0;
}
