
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
		int minDis = 0xFFFFFF; //�������ڼ�¼���غ��г��ֵ���СȨֵ
		for (int i=0; i<n; i++) { //ѭ��U��δ���뵽�������еĵ�
			if (U[i] == 0) { //�Ѿ����뵽������������
				continue;
			}
			for (int j=0; j<n; j++) { //ѭ��V�еĵ㣬����ĳδ�����������ĵ㵽����������СȨ�أ����룩
				if (i == j) { //i==j��ͬһ���㣬������
					continue;
				}
				if (V[j] == 0) { //�˵�δ������������������Ϊ������СȨֵ�����ݣ�����
					continue;
				}
				if (Graph[j][i] <= distance[i]) {
					//�����������j�㵽δ������������i��ľ������֪��i�㵽�������ľ��루distance����С���������С����
					distance[i] = Graph[j][i];
					//���濴�����˵���С�����Ƿ����֪�ı��غ���С���루minDis��ҪС
					if (distance[i] < minDis) {
						minDis = distance[i];
						//���i�㵽�������ľ����ڵ�ǰ�غ��и�С��������Ȩ�رߵ�ǰ��ͺ���Ϊ��ѡ�ĵ�
						candidate1 = j; //���º�ѡ��ǰ�㣨�������ϵĵ㣩
						candidate2 = i; //���º�ѡ�ĺ�㣨δ�������ĵ㣩
					}
				}
			}
		}
		//һ�غϵ�̽��������ѡ���ѡ������������������Ϊ0
		U[candidate2] = 0;
		V[candidate2] = 1;
		distance[candidate2] = 0;
		//���⣬�ڱ����У��Ѿ����ڵĵ绰��ȨֵΪ0�����������������
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
Ѱ����С������
Prim�㷨
�����1�㿪ʼ��Ϊ��������ÿ�ε�����������δ�������ĵ㵽���ľ��룬ѡ��С����ĵ����������
Graph[n][n]���������е�ͼ
distance[n]��������㵽�������ľ��룬��ʼ��distance[0] = 0��distance[n] = INFINITY��#define INFINITY 0xFFFFFFFF
׼��2�����飬V[n]=1��¼�Ѿ������������ĵ㣬U[n]=1��ʾδ�����������ĵ�
���������в��ϸ��¸����㵽�������ľ��룬ĳ��v������������v�������������Ϊ0
׼��һ�����У����ڼ�¼���ӱߵ�ǰһ����ͺ�һ���㣬���������У����ֱ�ȨΪ0�ģ����Ѿ����ڵ����ӣ�����¼
*/
