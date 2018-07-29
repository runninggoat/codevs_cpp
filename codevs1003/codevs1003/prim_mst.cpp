
#include <stdio.h>

using namespace std;

#define INFINITY 0xFFFFFF

class Node {
public:
	int v1;
	int v2;
	Node* prev;
	Node* next;
	Node();
	~Node();
};

Node::Node() {}

Node::~Node() {
	delete prev;
	delete next;
}

class Queue {
public:
	Node* HEAD;
	Node* TAIL;
	Node* cur;
	int length;
	Queue();
	~Queue();
	void enqueue(Node* node);
};

Queue::Queue() {
	HEAD = new Node();
	HEAD->v1 = -1;
	HEAD->v2 = -1;
	TAIL = new Node();
	TAIL->v1 = -1;
	TAIL->v2 = -1;
	HEAD->next = TAIL;
	TAIL->prev = HEAD;
	cur = HEAD;
	length = 0;
}

Queue::~Queue() {
	delete HEAD;
	delete TAIL;
}

void Queue::enqueue(Node* node) {
	cur->next = node;
	node->prev = cur;
	node->next = TAIL;
	TAIL->prev = node;
	cur = node;
	++length;
}

bool isEND();

int Graph[100][100];

int n;

int distance[100];
int V[100];
int U[100];

Queue* outputQ;

int main() {

	outputQ = new Queue();

	scanf("%d", &n);

	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			scanf("%d", &Graph[i][j]);
		}
		distance[i] = INFINITY;
		V[i] = 0;
		U[i] = 1;
	}
	distance[0] = 0;
	V[0] = 1;
	U[0] = 0;

	while (!isEND()) {
		int candidate1 = -1;
		int candidate2 = -1;
		int minDis = 0xFFFFFF; //变量用于记录本回合中出现的最小权值
		for (int i=0; i<n; i++) { //循环U中未加入到生成树中的点
			if (U[i] == 0) { //已经加入到生成树，跳过
				continue;
			}
			for (int j=0; j<n; j++) { //循环V中的点，更新某未加入生成树的点到生成树的最小权重（距离）
				if (i == j) { //i==j即同一个点，不考虑
					continue;
				}
				if (V[j] == 0) { //此点未加入生成树，不能作为更新最小权值的依据，跳过
					continue;
				}
				if (Graph[j][i] <= distance[i]) {
					//如果生成树中j点到未加入生成树的i点的距离比已知的i点到生成树的距离（distance）更小，则更新最小距离
					distance[i] = Graph[j][i];
					//下面看更新了的最小距离是否比已知的本回合最小距离（minDis）要小
					if (distance[i] < minDis) {
						minDis = distance[i];
						//如果i点到生成树的距离在当前回合中更小，则这条权重边的前点和后点记为候选的点
						candidate1 = j; //更新候选的前点（生成树上的点）
						candidate2 = i; //更新候选的后点（未加入树的点）
					}
				}
			}
		}
		//一回合的探索结束，选择候选点加入生成树，距离变为0
		U[candidate2] = 0;
		V[candidate2] = 1;
		distance[candidate2] = 0;
		//另外，在本题中，已经存在的电话线权值为0，不加入输出队列中
		if (Graph[candidate1][candidate2] > 0) {
			Node* node = new Node();
			node->v1 = candidate1;
			node->v2 = candidate2;
			outputQ->enqueue(node);
		}
	}

	printf("%d\n", outputQ->length);

	int cost = 0;
	Node* node = outputQ->HEAD->next;
	for (int i=0; i<outputQ->length; i++) {
		cost += Graph[node->v1][node->v2];
		if (node->v1 < node->v2) {
			printf("%d %d\n", node->v1 + 1, node->v2 + 1);
		} else {
			printf("%d %d\n", node->v2 + 1, node->v1 + 1);
		}
		node = node->next;
	}

	printf("%d", cost);

	return 0;
}

bool isEND () {
	for (int i=0; i<n; i++) {
		if (V[i] == 0 || U[i] == 1) {
			return false;
		}
	}
	return true;
}


/*
寻找最小生成树
Prim算法
本题从1点开始作为生成树，每次迭代更新其它未加入树的点到树的距离，选最小距离的点加入生成树
Graph[n][n]保存输入中的图
distance[n]保存各个点到生成树的距离，初始化distance[0] = 0，distance[n] = INFINITY，#define INFINITY 0xFFFFFFFF
准备2个数组，V[n]=1记录已经加入生成树的点，U[n]=1表示未加入生成树的点
迭代过程中不断更新各个点到生成树的距离，某点v加入生成树后，v到生成树距离变为0
准备一个队列，用于记录连接边的前一个点和后一个点，迭代过程中，发现边权为0的，是已经存在的连接，不记录
*/
