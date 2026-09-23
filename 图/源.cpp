#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#define maxsize 20
#define max 0x3f
typedef char vertextype[20];
typedef int edgetype;
#define max 100
typedef struct stack
{
	int data[max];
	int top;
}stack;
stack* initstack()
{
	stack* s = (stack*)malloc(sizeof(stack));
	s->top = -1;
	return s;
}
int isempty(stack* s)
{
	if (s->top == -1)
		return 1;
	else
		return 0;
}
int push(int num, stack* s)
{
	if (s->top == max - 1) return 0;
	s->top++;
	s->data[s->top] = num;
	return 1;
}
int pop(stack* s)
{
	if (isempty(s) == 1) return -1;
	int temp = s->data[s->top];
	s->top--;
	return temp;
}
//邻接矩阵
typedef struct
{
	int edgenumber;
	int vertexnumber;
	edgetype arc[maxsize][maxsize];
	vertextype vertex[maxsize];
}mat_gragh;
mat_gragh* initgraph()
{
	mat_gragh* g = (mat_gragh*)malloc(sizeof(mat_gragh));
	g->edgenumber = 0;
	g->vertexnumber = 0;
	for (int d = 0; d < maxsize; d++)
	{
		for (int e = 0; e < maxsize; e++)
		{
			g->arc[d][e] = 0;
		}
	}
	char round = 0;
	for (int a = 0; a < maxsize; a++)
	{
		sprintf(g->vertex[a], "%d", round);
		round++;
	}
	return g;
}
//有向图一样 唯一需要略加关心的是带权图了 其实也是一样不过就是要考虑带权图中的邻接矩阵在非对角线的时候是∞ 这里用0x7fffffff代替 
// 但是我手懒了 所以用0x3f -v-
//邻接表
#define maxvex 100
typedef struct egdenode
{
	int adjvex;
	struct egdenode* next;
}egdenode;//边表
typedef struct
{
	int vertex;
	egdenode* next;
}vertexnode, adjlist[maxvex];//顶点表 
//这里等于using adjlist=vertexnode[maxvex] C语言原生这样有点别扭
typedef struct
{
	adjlist Adjlist;
	int vertexnumber;
	int egdenumber;
}adjgraph;
adjgraph* initadjgraph()
{
	adjgraph* g = (adjgraph*)malloc(sizeof(adjgraph));
	g->egdenumber = 0;
	g->vertexnumber = 0;
	for (int a = 0; a < maxvex; a++)
	{
		g->Adjlist[a].next = NULL;
		g->Adjlist[a].vertex = a;
	}
	return g;
}
void addedge(adjgraph* g, int from, int to)
{
	g->egdenumber++;
	egdenode* newg = (egdenode*)malloc(sizeof(egdenode));
	egdenode* newgg = (egdenode*)malloc(sizeof(egdenode));
	newg->adjvex = from;
	newgg->adjvex = to;
	newgg->next = g->Adjlist[from].next;
	g->Adjlist[from].next = newgg;
	newg->next = g->Adjlist[to].next;
	g->Adjlist[to].next = newg;
}
void printgraph(adjgraph* g)
{
	for (int a = 0; a < 100; a++)
	{
		egdenode* tempnode = g->Adjlist[a].next;//NOTE: 此时next反而代表了第一个边表指针
		if (tempnode != NULL) 
		{
			printf("%d:  ", a);
			while (tempnode != NULL)
			{
				printf("%d ", tempnode->adjvex);
				tempnode = tempnode->next;
			}
			printf("\n");
		}
	}
}
void dfs(adjgraph* g,stack*s)
{
	int visited[maxvex-1];
	for (int a = 1; a < maxvex;a++)
	{
		visited[a] = 0;
	}
	int now;
	push(0, s);
	while (isempty(s) != 0)
	{
		now = pop(s);
		while (visited[now] == 1)
		{
			if (isempty == 0)
			{
				return;
			}
			now = pop(s);
		}
		printf("%d ", g->Adjlist[now].next->adjvex);
		visited[now] = 1;
	}
	while (g->Adjlist[now].next != NULL)
	{
		push(g->Adjlist[now].next->adjvex, s);
		g->Adjlist[now].next = g->Adjlist[now].next->next;
	}
}
//深度优先搜索
int main()
{
	adjgraph* g = initadjgraph();
	addedge(g, 0, 1);
	addedge(g, 0, 3);
	addedge(g, 1, 2);
	addedge(g, 5, 8);
	stack* s = initstack();
	dfs(g,s);
}