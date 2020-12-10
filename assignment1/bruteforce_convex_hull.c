#include <stdlib.h> // atoi, rand, malloc, realloc
#include <stdio.h>
#include <time.h> //time

#define RANGE 10000

typedef struct
{
	int x;
	int y;
} t_point;

typedef struct
{
	t_point from;
	t_point to;
} t_line;

////////////////////////////////////////////////////////////////////////////////
void print_header( char *filename)
{
	printf( "#! /usr/bin/env Rscript\n");
	printf( "png(\"%s\", width=700, height=700)\n", filename);
	
	printf( "plot(1:%d, 1:%d, type=\"n\")\n", RANGE, RANGE);
}
////////////////////////////////////////////////////////////////////////////////
void print_footer( void)
{
	printf( "dev.off()\n");
}

////////////////////////////////////////////////////////////////////////////////
/*
#points
points(2247,7459)
points(616,2904)
points(5976,6539)
points(1246,8191)
*/
void print_points(t_point* points, int num_point) { // TODO : 위의 주석처럼 생성된 points를 출력
	
	printf("#points\n");
	for (int i = 0; i < num_point; ++i)
		printf("points(%d,%d)\n", points[i].x, points[i].y);
	printf("\n");

}
/*
#line segments
segments(7107,2909,7107,2909)
segments(43,8,5,38)
segments(43,8,329,2)
segments(5047,8014,5047,8014)
*/
void print_line_segments(t_line* lines, int num_line) { //TODO : 위의 주석처럼 생성된 line(from, to) 출력
	
	printf("#line segments\n");
	for (int i = 0; i < num_line; ++i)
		printf("segments(%d,%d,%d,%d)\n", lines[i].from.x, lines[i].from.y, lines[i].to.x, lines[i].to.y);

}
// [input] points : set of points
// [input] num_point : number of points
// [output] num_line : number of line segments that forms the convex hull
// return value : set of line segments that forms the convex hull
t_line* convex_hull(t_point* points, int num_point, int* num_line) { //num line도 set 하세요. 

	*num_line = 0;
	t_line* lines = (t_line*)malloc(sizeof(t_line) * *num_line);
	int isConvexHull = 0, isOn=0;
	int a, b,c;
	for (int from = 0; from < num_point - 1; ++from) {
		for (int to = from + 1; to < num_point; ++to) {
			isConvexHull = 0;
			//ax+by-c에서 a,b,c 구하기
			a = points[to].y - points[from].y;
			b = points[from].x - points[to].x;
			c = points[from].x * points[to].y - points[to].x * points[from].y;
			// line(from, to)의 CCW 판별 
			for (int i = 0; i < num_point; ++i) {
				if (a * points[i].x + b * points[i].y - c > 0) {
					isOn = 1;
					break;
				}
				if (a * points[i].x + b * points[i].y - c < 0) {
					isOn = 0;
					break;
				}
			}
			// convex hull인지 판별 .
			if(isOn)
				for (int i = 0; i < num_point; ++i) {
					if (a * points[i].x + b * points[i].y - c < 0) break;
					if (i == num_point - 1) isConvexHull = 1;
				}
			else
				for (int i = 0; i < num_point; ++i) {
					if (a * points[i].x + b * points[i].y - c > 0) break;
					if (i == num_point - 1) isConvexHull = 1;
				}

			

			if (isConvexHull) { // Convex Hull이라면 . 
				lines = (t_line*)realloc(lines, sizeof(t_line) * (++(*num_line)));
				lines[*num_line - 1].from.x = points[from].x;
				lines[*num_line - 1].from.y = points[from].y;
				lines[*num_line - 1].to.x = points[to].x;
				lines[*num_line - 1].to.y = points[to].y;
			}


		}
	}
	return lines;


}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int x, y;
	int num_point; // number of points
	int num_line; // number of lines
	
	if (argc != 2)
	{
		printf( "%s number_of_points\n", argv[0]);
		return 0;
	}

	num_point = atoi( argv[1]);
	if (num_point <= 0)
	{
		printf( "The number of points should be a positive integer!\n");
		return 0;
	}

	t_point *points = (t_point *) malloc( num_point * sizeof( t_point));
		
	t_line *lines;

	// making n points
	srand( time(NULL));
	for (int i = 0; i < num_point; i++)
	{
		x = rand() % RANGE + 1; // 1 ~ RANGE random number
		y = rand() % RANGE + 1;
		
		points[i].x = x;
		points[i].y = y;
 	}

	fprintf( stderr, "%d points created!\n", num_point);

	print_header( "convex.png");
	
	print_points( points, num_point);
	
	lines = convex_hull( points, num_point, &num_line);

	fprintf( stderr, "%d lines created!\n", num_line);

	print_line_segments( lines, num_line);
		
	print_footer();
	
	free( points);
	free( lines);
	
	return 0;
}
