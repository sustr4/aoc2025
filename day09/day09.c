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
#define MAXT 496
int MAXX;
int MAXY;
//#define INPUT "unit1.txt"
//#define MAXT 8

// Point structure definition
typedef struct {
	int x;
	int y;
	int scalex;
	int scaley;
	int orig;
} TPoint;

// Comparator function example
int comp(const void *a, const void *b, void *arg)
{
	const TPoint *da = (const TPoint *) a;
	const TPoint *db = (const TPoint *) b;
	int member = *(const int *)arg;

	switch(member) {
		case 1: return (da->x > db->x) - (da->x < db->x);
		case 2: return (da->y > db->y) - (da->y < db->y);
		case 3: return (da->orig > db->orig) - (da->orig < db->orig);
	}
	return 0;
}

// Print a two-dimensional array
void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			switch(map[y][x]) {
			case 1:
				printf("\033[1;31m█");
				break;
			case 2:
				printf("\033[1;32m█");
				break;
			case 3:
				printf("\033[1;32mX");
				break;
			case 4:
				printf("\033[1;34m█");
				break;
			default:
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("\033[0moff\n");
}

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

	TPoint *inst=(TPoint*)calloc(MAXT, sizeof(TPoint));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into array
		sscanf(line,"%d,%d",
			&(inst[count].x),
			&(inst[count].y));
		inst[count].orig=count;

		count++;
	}

	fclose(input);
	if (line) free(line);

	return inst;
}

int check(char **map, TPoint a, TPoint b) {
	// Check if rectangle from a to b
	int dy=0, dx=0;
	if(a.scalex > b.scalex) dx=-1;
	if(a.scalex < b.scalex) dx=1;
	if(a.scaley > b.scaley) dy=-1;
	if(a.scaley < b.scaley) dy=1;
	
	for(int y=a.scaley; y!=b.scaley; y+=dy) {
		for(int x=a.scalex; x!=b.scalex; x+=dx) {
			if(!map[y][x]) return 0;
		}
	}

	return 1;
}

int main(int argc, char *argv[]) {

	int i=0;
	TPoint *array = readInput();
	readInput();
	long long maxarea=0;

	// Sort by x
	int sort_key=1; qsort_r(array,MAXT,sizeof(TPoint),comp,&sort_key);
	int scaled=0;
	for(i=1; i<MAXT; i++) {
		if(array[i].x==array[i-1].x) array[i].scalex = array[i-1].scalex;
		else {
			scaled+=2;
			MAXX=scaled+1;
			array[i].scalex=scaled;
		}
	}
	printf("\n");
	// Sort by y
	sort_key=2; qsort_r(array,MAXT,sizeof(TPoint),comp,&sort_key);
	scaled=0;
	for(i=1; i<MAXT; i++) {
		if(array[i].y==array[i-1].y) array[i].scaley = array[i-1].scaley;
		else {
			scaled+=2;
			MAXY=scaled+1;
			array[i].scaley=scaled;
		}
	}

	// Sort back
	sort_key=3; qsort_r(array,MAXT,sizeof(TPoint),comp,&sort_key);

	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	// Fill in edges
	for(i=0; i<MAXT; i++) {
		map[array[i].scaley][array[i].scalex]=1;
		int inx=array[i].scalex;
		int iny=array[i].scaley;
		int next=(i+1) % MAXT;
		while(1) {
			if((inx==array[next].scalex)&&(iny==array[next].scaley)) break;
			if(inx<array[next].scalex) inx++;
			if(inx>array[next].scalex) inx--;
			if(iny<array[next].scaley) iny++;
			if(iny>array[next].scaley) iny--;
			if(!map[iny][inx]) map[iny][inx]=2;
		}
	}

	// Fill inside
	// First consider all in, except map edges
	for(int y=1; y<MAXY-1; y++) {
		for(int x=1; x<MAXX-1; x++) {
			if(!map[y][x]) map[y][x]=3;
		}
	}
	// Grow void:
	int change=1;
	while(change) {
		change=0;
		for(int y=0; y<MAXY; y++) 
			for(int x=0; x<MAXX; x++) 
				for(int n=0; n<8; n++) {
					if(map[y][x]) continue;
					if(mapnb(map, y, x, n)==3) {
						map[y+dy[n]][x+dx[n]]=0;
						change=1;
					}
				}
	}


	TPoint max[2];

	for(i=0; i<MAXT; i++) {
		for(int y=0; y<MAXT; y++) {
			long long wide = (long long)array[i].x-(long)(long)array[y].x;
			long long high = (long long)array[i].y-(long)(long)array[y].y;
			wide = (wide > 0 ? wide : 0 - wide) + 1;
			high = (high > 0 ? high : 0 - high) + 1;
			long long area= wide * high;
			if(area > maxarea) {
				// Inside needs checking
				if(!check(map, array[i], array[y])) continue;

				maxarea=area;
				printf("[%d,%d] & [%d,%d]: %lld\n", array[i].x, array[i].y, array[y].x, array[y].y, area);
				max[0]=array[i];
				max[1]=array[y];
			}
		}

	}

	for(int y = max[0].scaley < max[1].scaley ? max[0].scaley : max[1].scaley;
		y <= (max[0].scaley < max[1].scaley ? max[1].scaley : max[0].scaley); y++)
		for(int x = max[0].scalex < max[1].scalex ? max[0].scalex : max[1].scalex;
			x <= (max[0].scalex < max[1].scalex ? max[1].scalex : max[0].scalex); x++)
			map[y][x]=4;

//	printMap(map);
	return 0;
}
