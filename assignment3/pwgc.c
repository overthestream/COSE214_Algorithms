#include <stdio.h>
#include <stdlib.h>

#define PEASANT 0x08
#define WOLF	0x04
#define GOAT	0x02
#define CABBAGE	0x01

// 주어진 상태 state의 이름(마지막 4비트)을 화면에 출력
// 예) state가 7(0111)일 때, "<0111>"을 출력
static void print_statename(FILE* fp, int state) {
	// fin
	char bit[5];
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);
	bit[4] = 0;
	bit[0] = '0' + p;
	bit[1] = '0' + w;
	bit[2] = '0' + g;
	bit[3] = '0' + c;
	fprintf(fp, "<%s>", bit);
}

// 주어진 상태 state에서 농부, 늑대, 염소, 양배추의 상태를 각각 추출하여 p, w, g, c에 저장
// 예) state가 7(0111)일 때, p = 0, w = 1, g = 1, c = 1
static void get_pwgc(int state, int* p, int* w, int* g, int* c) {
	//fin 
	*p = (state & 8) >> 3;
	*w = (state & 4) >> 2;
	*g = (state & 2) >> 1;
	*c = state & 1;
}

// 허용되지 않는 상태인지 검사
// 예) 농부없이 늑대와 염소가 같이 있는 경우 / 농부없이 염소와 양배추가 같이 있는 경우
// return value: 1 허용되지 않는 상태인 경우, 0 허용되는 상태인 경우
static int is_dead_end(int state) {
	//fin
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);
	if (p != g && (g == w || g == c)) return 1;
	else return 0;
		
}

// state1 상태에서 state2 상태로의 전이 가능성 점검
// 농부 또는 농부와 다른 하나의 아이템이 강 반대편으로 이동할 수 있는 상태만 허용
// 허용되지 않는 상태(dead-end)로의 전이인지 검사
// return value: 1 전이 가능한 경우, 0 전이 불이가능한 경우 
static int is_possible_transition(int state1, int state2) {

	// fin
	int p1, p2, w1, w2, g1, g2, c1, c2;
	get_pwgc(state1, &p1, &w1, &g1, &c1);
	get_pwgc(state2, &p2, &w2, &g2, &c2);

	//농부가 안 움직였는가
	if (p1 == p2) return 0;

	//둘 이상이 움직였나
	if (w1 != w2 && g1 != g2) return 0;
	if (g1 != g2 && c1 != c2) return 0;
	if (w1 != w2 && c1 != c2) return 0;

	// 옮기려는 아이템과 농부가 다른 위치였는가 
	if (w1 != w2 && p1 != w1) return 0;
	if (g1 != g2 && p1 != g1) return 0;
	if (c1 != c2 && p1 != c1) return 0;

	if (is_dead_end(state2)) return 0;
	return 1;


}

// 상태 변경: 농부 이동
// return value : 새로운 상태
static int changeP(int state) {
	//fin
	return state & 8 ? state & 7 : state | 8;
}

// 상태 변경: 농부, 늑대 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePW(int state) {
	//fin
	int state2;
	state2 = state & 8 ? state & 7 : state | 8;
	state2 = state2 & WOLF ? state2 & ~WOLF : state2 | WOLF;
	if (is_possible_transition(state, state2)) return state2;
	else return -1;
}

// 상태 변경: 농부, 염소 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePG(int state) {
	//fin
	int state2;
	state2 = state & 8 ? state & 7 : state | 8;
	state2 = state2 & GOAT ? state2 & ~GOAT : state2 | GOAT;
	if (is_possible_transition(state, state2)) return state2;
	else return -1;
}

// 상태 변경: 농부, 양배추 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1 
static int changePC(int state) {
	//fin
	int state2;
	state2 = state & 8 ? state & 7 : state | 8;
	state2 = state2 & CABBAGE ? state2 & ~CABBAGE : state2 | CABBAGE;
	if (is_possible_transition(state, state2)) return state2;
	else return -1;
}

// 주어진 state가 이미 방문한 상태인지 검사
// return value : 1 visited, 0 not visited
static int is_visited(int visited[], int level, int state) {
	//fin
	if (0 < visited[state] && visited[state] < level) return 1;
	else return 0;
}

// 방문한 상태들을 차례로 화면에 출력
static void print_states(int visited[], int count) {
	//fin
	print_statename(stdout, 0);
	fprintf(stdout, "\n");
	for (int i = 1; i <= count; ++i) {
		for (int j = 0; j < 16; ++j) {
			if (visited[j] == i) {
				print_statename(stdout, j);
				fprintf(stdout, "\n");
			}
		}
	}
}

// recursive function
static void dfs_main(int state, int goal_state, int level, int visited[]) {
	//fin
	printf("cur state is ");
	print_statename(stdout, state);
	printf(" (level %d)\n", level);
	visited[state] = level;
	if (state == goal_state) {
		printf("Goal-state found!\n");
		print_states(visited, level);
		visited[state] = 0;
		return;
	}
	int next;
	next = changeP(state);
	if (changeP(state) != -1) {
		if (is_dead_end(next) {
			printf("\t\tnext state ");
			print_statename(stdout,next);
			printf(" is dead-end\n");
		}
		else if (is_visited(visited, level, next)) {
			printf("\t\tnext state ");
			print_statename(stdout, next);
			printf(" has been visited\n");
		}
		else {
			dfs_main(next, goal_state, level + 1, visited);
			printf("back to ");
			print_statename(stdout, state);
			printf(" (level %d)\n", level);
		}
	}
	next = changePW(state);
	if (next == -1) {
		if (is_dead_end(next)) {
			printf("\t\tnext state ");
			print_statename(stdout, next);
			printf(" is dead-end\n");
		}
	}
	else {
		if (is_visited(visited, level, next)) {
			printf("\t\tnext state ");
			print_statename(stdout, next);
			printf(" has been visited\n");
		}
		else {
			dfs_main(next, goal_state, level + 1, visited);
			printf("back to ");
			print_statename(stdout, state);
			printf(" (level %d)\n", level);
		}
	}

	next = changePG(state);
	if (next == -1) {
		if (is_dead_end(next)) {
			printf("\t\tnext state ");
			print_statename(stdout, next);
			printf(" is dead-end\n");
		}
	}
	else {
		if (is_visited(visited, level, next)) {
			printf("\t\tnext state ");
			print_statename(stdout, next);
			printf(" has been visited\n");
		}
		else {
			dfs_main(next, goal_state, level + 1, visited);
			printf("back to ");
			print_statename(stdout, state);
			printf(" (level %d)\n", level);
		}
	}

	next = changePC(state);
	if (next == -1) {
		if (is_dead_end(next)) {
			printf("\t\tnext state ");
			print_statename(stdout, next);
			printf(" is dead-end\n");
		}
	}
	else {
		if (is_visited(visited, level, next)) {
			printf("\t\tnext state ");
			print_statename(stdout, next);
			printf(" has been visited\n");
		}
		else {
			dfs_main(next, goal_state, level + 1, visited);
			printf("back to ");
			print_statename(stdout, state);
			printf(" (level %d)\n", level);
		}
	}

	visited[state] = 0;
	return;

}

////////////////////////////////////////////////////////////////////////////////
// 상태들의 인접 행렬을 구하여 graph에 저장
// 상태간 전이 가능성 점검
// 허용되지 않는 상태인지 점검 
void make_adjacency_matrix(int graph[][16]) {
	//fin
	int next1, next2, next3, next4;
	for(int cur=0; cur<16; ++cur) {
		if (is_dead_end(cur)) continue;
		next1 = changeP(cur);
		next2 = changePC(cur);
		next3 = changePG(cur);
		next4 = changePW(cur);
		
		if (!is_dead_end(next1) {
			graph[cur][next1] = 1;
			graph[next1][cur] = 1;
		}
		if (next2 != -1) {
			graph[cur][next2] = 1;
			graph[next2][cur] = 1;
		}
		if (next3 != -1) {
			graph[cur][next3] = 1;
			graph[next3][cur] = 1;
		}
		if (next4 != -1) {
			graph[cur][next4] = 1;
			graph[next4][cur] = 1;
		}
	}
}

// 인접행렬로 표현된 graph를 화면에 출력
void print_graph(int graph[][16], int num) {
	//fin
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			printf("%d ", graph[i][j]);
		}
		printf("\n");
	}
}

// 주어진 그래프(graph)를 .net 파일로 저장
// pgwc.net 참조
void save_graph(char* filename, int graph[][16], int num) {
	//fin
	FILE* fp = fopen(filename, "w");
	fprintf(fp, "*Vertices 16\n");
	for (int i = 1; i <= 16; ++i) {
		fprintf(fp, "%d \"");
		print_statename(fp, i - 1);
		fprintf(fp, "\"\n");
	}
	fprintf(fp, "*Edges\n");
	for (int i = 0; i < 16; ++i) {
		for (int j = i + 1; j < 16; ++j) {
			if (graph[i][j])
				fprintf(fp, "  %d  %d\n", i + 1, j + 1);
		}
	}
	fclose(fp);
}

////////////////////////////////////////////////////////////////////////////////
// 깊이 우선 탐색 (초기 상태 -> 목적 상태)
void depth_first_search( int init_state, int goal_state)
{
	int level = 0;
	int visited[16] = {0,}; // 방문한 정점을 저장
	
	dfs_main( init_state, goal_state, level, visited); 
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int graph[16][16] = {0,};
	
	// 인접 행렬 만들기
	make_adjacency_matrix( graph);

	// 인접 행렬 출력 (only for debugging)
	print_graph( graph, 16);
	
	// .net 파일 만들기
	save_graph( "pwgc.net", graph, 16);

	// 깊이 우선 탐색
	depth_first_search( 0, 15); // initial state, goal state
	
	return 0;
}


