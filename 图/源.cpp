#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<iostream>
using namespace std;
#define maxsize 20
#define max 0x3f
typedef char vertextype[20];
typedef int edgetype;
#define max 100
typedef struct queue
{
	int data[max];
	int front;
	int rear;
}queue;
queue* initqueue()
{
	queue* q = (queue*)malloc(sizeof(queue));
	q->rear = 0;
	q->front = 0;
	return q;
}
int pushqueue(queue* q, int num)
{
	if ((q->rear + 1) % max == q->front) return 0;
	q->data[q->rear] = num;
	q->rear = (q->rear + 1) % max;
	return 1;
}
int popqueue(queue* q)
{
	if (q->rear == q->front) return 0;
	int temp = q->data[q->front];
	q->front = (q->front + 1) % max;
	return temp;
}
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
	int arc[maxsize][maxsize];
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
	int weight;
	struct egdenode* next;
}egdenode;//边表 带权边
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
void addedge(adjgraph* g, int from, int to, int weight)
{
	g->egdenumber++;
	egdenode* newg = (egdenode*)malloc(sizeof(egdenode));
	egdenode* newgg = (egdenode*)malloc(sizeof(egdenode));
	newg->adjvex = from;
	newgg->adjvex = to;
	newg->weight = weight;
	newgg->weight = weight;
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
//以下为自己写的dfs 居然无意间在弄非递归遍历 呵呵呵，看到标准的递归遍历开怀大笑，原来这么简单，明明早就想到树的遍历了，哈哈。
void dfs(adjgraph* g, stack* s)
{
	int visited[maxvex] = { 0 };
	int now;
	egdenode* tempnode = g->Adjlist[0].next;
	push(0, s);
	while (isempty(s) == 0)
	{
		now = pop(s);
		while (visited[now] == 1)
		{
			if (isempty(s) == 1) return;
			now = pop(s);
		}
		printf("%d ", now);
		visited[now] = 1;
		tempnode = g->Adjlist[now].next;
		while (tempnode != NULL)
		{
			push(tempnode->adjvex, s);
			tempnode = tempnode->next;
		}
	}
}
//标准dfs:
void dfs2(adjgraph* g, int now, int visited[])
{
	printf("%d ", now);
	visited[now] = 1;
	egdenode* tempnode = g->Adjlist[now].next;
	while (tempnode != NULL)
	{
		int next = tempnode->adjvex;
		if (visited[next] == 0)
		{
			dfs2(g, next, visited);
		}
		tempnode = tempnode->next;
	}
}
void dfsme(adjgraph* g, int now, int visited[])
{
	printf("%d", now);
	visited[now] = 1;
	egdenode* tempnode = g->Adjlist[now].next;
	while (tempnode != NULL)
	{
		int next = tempnode->adjvex;
		if (visited[next] == 0)
		{
			dfsme(g, next, visited);
		}
		tempnode = tempnode->next;
	}
}
void bfs(adjgraph* g, queue* q)
{
	int visited[max] = { 0 };
	pushqueue(q, 0);
	while (q->front != q->rear)
	{
		int temp = popqueue(q);
		egdenode* tempnode = g->Adjlist[temp].next;
		if (visited[temp] == 0)
		{
			printf("%d ", temp);
			visited[temp] = 1;
		}
		while (tempnode != NULL)
		{
			pushqueue(q, tempnode->adjvex);
			tempnode = tempnode->next;
		}
	}
}
#define inf 0x3f3f3f3f
void primalg(adjgraph* g)
{
	int sumweight = 0;
	int dist[maxvex];
	memset(dist, inf, sizeof(dist));
	bool intree[maxvex] = { false };
	dist[0] = 0;
	for (int now = 0; now < g->vertexnumber; now++)
	{
		int minvertex;
		int minweight = inf;
		//找出账本最小权值
		for (int a = 0; a < g->vertexnumber; a++)
		{
			if (!intree[a] && dist[a] < minweight)
			{
				minweight = dist[a];
				minvertex = a;
			}
		}
		intree[minvertex] = true;
		sumweight += minweight;
		//维护并更新账本
		egdenode* tempnode = g->Adjlist[minvertex].next;
		while (tempnode != NULL)
		{
			int thisweight = tempnode->weight;
			int thisvertex = tempnode->adjvex;
			if (intree[thisvertex] == false && thisweight < dist[thisvertex])
			{
				dist[thisvertex] = thisweight;
			}
			tempnode = tempnode->next;
		}
	}
	printf("%d", sumweight);
}
//邻接矩阵版本 写这个目的是破坏性改写 增强我对prim的记忆和理解
int vertexprim(mat_gragh* g)
{
	//初始化
	int dist[maxvex];
	memset(dist, inf, sizeof(dist));
	bool intree[maxvex] = { false };
	dist[0] = 0;
	int sumweight = 0;
	//一次一次找到账本minweight直到循环结束
	for (int now = 0; now < g->vertexnumber; now++)
	{
		int minvertex = -1;
		int minweight = inf;
		//找到单次账本minweight
		for (int a = 0; a < g->vertexnumber; a++)
		{
			if (!intree[a] && dist[a] < minweight)
			{
				minweight = dist[a];
				minvertex = a;
			}
		}
		if (minvertex == -1 || minweight == inf) return -1;
		sumweight += minweight;
		//更新并维护dist和intree
		intree[minvertex] = true;
		for (int b = 0; b < g->vertexnumber; b++)
		{
			if (!intree[b] && dist[b] > g->arc[minvertex][b])
			{
				dist[b] = g->arc[minvertex][b];
			}
		}
	}
	return sumweight;
}
typedef struct edge
{
	int weight;
	int parent;
	int son;
}edge;
void swap(edge* a, edge* b) {

	edge temp = *a;
	*a = *b;
	*b = temp;
}
void quick_sort(edge arr[], int left, int right) {
	if (left >= right) return;

	int pivot = arr[left].weight; // 选基准值
	int i = left, j = right;

	while (i < j) {
		// 从右往左找第一个小于基准的
		while (i < j && arr[j].weight >= pivot) j--;
		// 从左往右找第一个大于基准的
		while (i < j && arr[i].weight <= pivot) i++;
		if (i < j) {
			swap(&arr[i], &arr[j]);
		}
	}
	// 将基准归位
	swap(&arr[left], &arr[i]);

	// 递归左右两边
	quick_sort(arr, left, i - 1);
	quick_sort(arr, i + 1, right);
}
//并查集
//这里kruskal核心的改进在于超前的递归寻祖 只需要寻到一次 以后树中所有子孙的祖都将为真正的根节点而非未初始化的向上一个一个追溯 
//这无疑是超前且极具智慧的 我感到深深的敬佩 我自己写kruskal的时候很难很难想到这种聪明绝顶的东西
//这里是溯源并重定向一步到祖的find
int find_ancestor(int u[], int v)
{
	if (u[v] == v) return v;
	return u[v] = find_ancestor(u, u[v]);
}
//判别是否同祖
bool union_set(int parent[], int u, int v)
{
	int root_u = find_ancestor(parent, u);
	int root_v = find_ancestor(parent, v);
	if (root_u == root_v) return false;
	parent[root_u] = root_v;
	return true;
}
//找到rootu/v 然后看是否相等 如果相等就说明成环 不相等就并入 并入的方式很特别 是直接parent[root]级别的
//其实就是把祖节点转换成了子节点
void kruskal(adjgraph* g)
{
	//初始化 找到所有边
	int sumweight = 0;
	int size = 0;
	int num = 0;
	egdenode* tempnode;
	int parent[maxvex] = { 0 };
	//每个顶点的祖初始化为自己
	for (int a = 0; a < g->vertexnumber; a++)
	{
		parent[a] = a;
	}
	// 预估边数，分配空间 这里借助copilot写了一下预估的边数 我忘记了本质上是个拉手问题
	int max_edges = g->vertexnumber * (g->vertexnumber - 1);
	edge* edgetest = (edge*)malloc(sizeof(edge) * max_edges);
	for (int now = 0; now < g->vertexnumber; now++)
	{
		tempnode = g->Adjlist[now].next;
		while (tempnode != NULL)
		{
			edgetest[size].weight = tempnode->weight;
			edgetest[size].parent = now;
			edgetest[size].son = tempnode->adjvex;
			tempnode = tempnode->next;
			size++;
		}
	}
	//排序边表 此时边表变为按权值从小到大排列
	quick_sort(edgetest, 0, size - 1);
	//开始kruskal核心思想的全局连线
	for (int i = 0; i < size; i++)
	{
		bool istrue = union_set(parent, edgetest[i].parent, edgetest[i].son);
		if (istrue)
		{
			num++;
			sumweight += edgetest[i].weight;
		}
		if (num == g->vertexnumber - 1) break;
	}
	if (num != g->vertexnumber - 1)
	{
		printf("非连通图 错误！");
		free(edgetest);
		return;
	}
	printf("%d", sumweight);
	free(edgetest);
}
int djikstra(adjgraph* g)
{
	int visited[maxvex] = { false };
	int dist[maxvex];
	memset(dist, 0x3f, sizeof(dist));
	dist[0] = 0;
	//重复找
	for (int i = 0; i < g->vertexnumber; i++)
	{
		egdenode* tempnode = NULL;
		int minweight = inf;
		int minvertex = -1;
		int thisweight = 0;
		int thisvertex = 0;
		//同样找账本最小
		//这里敢这么写 也就是最短的直接入visited锁定是因为没有负边权 
		//所以dist内最小的一定就是通向这个点的最短路径
		//其他的去其他点都比这里大了更别说从那个终点再过去这个点了
		for (int a = 0; a < g->vertexnumber; a++)
		{
			if (!visited[a] && dist[a] < minweight)
			{
				minweight = dist[a];
				minvertex = a;
			}
		}
		//这里找到了
		if (minvertex == -1) return -1;
		visited[minvertex] = true;
		//更新并维护
		tempnode = g->Adjlist[minvertex].next;
		while (tempnode != NULL)
		{
			thisweight = tempnode->weight;
			thisvertex = tempnode->adjvex;
			if (!visited[thisvertex] && dist[thisvertex] > dist[minvertex] + thisweight)
			{
				//确认单点最短叠加路径 这里倒是有点mst的味道
				//其实就是每个点都算一边经过它到这个thisvertex的最小权 找到绝对的最小权
				//不需要担心等会如果thisvertex被选了是否还会有其他的可能性 也就是在后面或许weight会更小
				//因为这里dist越靠后其实dist内的值都越大 这是因为这里我们计算的是总路径长 那当然越靠后路径越长
				//所以越早出来的 代表总长度越小 这个角度看 反而应该尽快出来 这是一个单向递增的循环
				//而这也就是djikstra的核心 也是不能出现负权边的根本原因
				dist[thisvertex] = dist[minvertex] + thisweight;
			}
			tempnode = tempnode->next;
		}
	}
}
//多源最短路 floyd算法
//其核心其实就是通过中转点的情况每个情况都把任意两点组合遍历一遍
//一次次更新得到最终的结果
int floyd(adjgraph* g)
{
	int dist[maxvex][maxvex];
	memset(dist, 0x3f, sizeof(dist));
	for (int i = 0; i < maxvex; i++)
	{
		dist[i][i] = 0;
	}
	for (int i = 0; i < maxvex; i++)
	{
		egdenode* tempnode = g->Adjlist[i].next;
		while (tempnode != NULL)
		{
			int tempvertex = tempnode->adjvex;
			dist[i][tempvertex] = tempnode->weight;
			tempnode = tempnode->next;
		}
	}
	//Floyd核心
	for (int k = 0; k < maxvex; k++)
	{
		for (int i = 0; i < maxvex; i++)
		{
			for (int j = 0; j < maxvex; j++)
			{
				if (dist[i][j] > dist[i][k] + dist[k][j])
				{
					dist[i][j] = dist[i][k] + dist[k][j];
				}
			}
		}
	}
	return 1;
}
//拓补排序
queue* kanh(adjgraph* g, queue* q)
{
	int count = 0;
	//找到所有点的入度
	int rudu[maxvex] = { 0 };
	for (int i = 0; i < g->vertexnumber; i++)
	{
		egdenode* tempnode = g->Adjlist[i].next;
		while (tempnode != NULL)
		{
			rudu[tempnode->adjvex]++;
			tempnode = tempnode->next;
		}
	}
	//找到初始入度为0
	for (int i = 0; i < g->vertexnumber; i++)
	{
		if (rudu[i] == 0) pushqueue(q, i);
	}
	//从初始队列中出队入队反复寻找入度为0的新点构成拓补序
	while (q->front != q->rear)
	{
		int temp = popqueue(q);
		count++;
		printf("%d ", temp);
		egdenode* tempnode = g->Adjlist[temp].next;
		while (tempnode != NULL)
		{
			rudu[tempnode->adjvex]--;
			if (rudu[tempnode->adjvex] == 0) pushqueue(q, tempnode->adjvex);
			tempnode = tempnode->next;
		}
	}
	if (count != g->vertexnumber) printf("出现环 无完整拓补结构");
	return q;
}
int criticalpath(adjgraph* g, stack* s)
{
	queue* temp;
	queue* q = kanh(g, temp);
	int* ve = (int*)malloc(sizeof(g->vertexnumber));
	int* vl = (int*)malloc(sizeof(g->vertexnumber));
	int e, l;
	memset(ve, 0, sizeof(ve));
	int maxweight = 0;
	//求ve
	while (q->front != q->rear)
	{
		int poptemp = popqueue(q);
		push(poptemp, s);
		egdenode* tempnode = g->Adjlist[poptemp].next;
		while (tempnode != NULL)
		{
			if (ve[tempnode->adjvex] < ve[poptemp] + tempnode->weight)
			{
				ve[tempnode->adjvex] = ve[poptemp] + tempnode->weight;
			}
			if (ve[tempnode->adjvex] > maxweight) maxweight = ve[tempnode->adjvex];
		}
	}
	for (int i = 0; i < g->egdenumber; i++)
	{
		vl[i] = maxweight;
	}
	//求vl
	while (isempty(s) != 1)
	{
		int temppop = pop(s);
		egdenode* tempnode = g->Adjlist[temppop].next;
		while (tempnode != NULL)
		{
			l = vl[tempnode->adjvex] - tempnode->weight;
			printf("l of %d to %d is %d ", temppop, tempnode->adjvex, l);
			if (vl[tempnode->adjvex] - tempnode->weight < vl[temppop])
			{
				vl[temppop] = vl[tempnode->adjvex] - tempnode->weight;
			}
		}
	}
}
int main()
{
	adjgraph* g = initadjgraph();
	mat_gragh* gg = initgraph();
	queue* q = initqueue();
	addedge(g, 0, 1, 2);
	addedge(g, 0, 2, 3);
	addedge(g, 1, 3, 9);
	g->vertexnumber = 4;
	kanh(g, q);
}