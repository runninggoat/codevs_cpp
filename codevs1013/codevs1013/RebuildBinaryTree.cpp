
#include <stdio.h>

using namespace std;

#define MAX 11

#define LEFT 1

#define RIGHT 2

class Node {
public:
	char* val;
	Node* left;
	Node* right;
	void printNodePre();
	Node();
	~Node();
};

Node* null = new Node();

Node::Node() {
	left = null;
	right = null;
}
Node::~Node() {
	delete left;
	delete right;
	delete val;
}

void Node::printNodePre() {
	printf("%s", val);
	if (left != null) left->printNodePre();
	if (right != null) right->printNodePre();
}

static void strcp(char src[], char target[]);

static void strcp(char src[], char target[], int start, int end); //end not include

static int length(char str[]); //end not include

/*
parent�����ڵ�
midSeq����������ַ���
postSeq����������ַ���
len���ַ������ȣ�������'\0'
tag����ǩ��ֻ����LEFT��1����RIGHT��2��
*/
void iterate(Node* parent, char midSeq[], char postSeq[], int len, int tag) {
	if (len == 1) {
		Node* n = new Node();
		char* v = new char[len + 1];
		strcp(midSeq, v);
		n->val = v;
		if (LEFT == tag) {
			parent->left = n;
		} else if (RIGHT == tag) {
			parent->right = n;
		}
		return;
	}

	char r = postSeq[len - 1]; //������������һ���ַ�Ϊ��
	int midCur = 0; //����������е��α꣬���ڼ�¼�ҵ������±�
	for (midCur=0; midCur<len; midCur++) {
		if (r == midSeq[midCur]) {
			break;
		}
	}

	//Ϊ�ҵ�������������ڵ�
	Node* n = new Node();
	char* v = new char[2];
	strcp(midSeq, v, midCur, midCur + 1);
	n->val = v;
	if (LEFT == tag) {
		parent->left = n;
	} else if (RIGHT == tag) {
		parent->right = n;
	}

	if (midCur > 0) {
		//���ӽڵ�ݹ�
		char* subLeftMidStr = new char[midCur + 1];
		strcp(midSeq, subLeftMidStr, 0, midCur);

		char* subLeftPostStr = new char[midCur + 1];
		strcp(postSeq, subLeftPostStr, 0, midCur);

		iterate(n, subLeftMidStr, subLeftPostStr, midCur, LEFT);
	}

	//���ӽڵ�ݹ�
	if (midCur < len - 1) {
		char* subRightMidStr = new char[len - midCur];
		strcp(midSeq, subRightMidStr, midCur + 1, len);

		char* subRightPostStr = new char[len - midCur];
		strcp(postSeq, subRightPostStr, midCur, len - 1);

		iterate(n, subRightMidStr, subRightPostStr, len - midCur - 1, RIGHT);
	}
}

int main() {
	
	char midSeq[MAX];
	char postSeq[MAX];

	scanf("%s", &midSeq);
	scanf("%s", &postSeq);

	int l = length(midSeq);

	iterate(null, midSeq, postSeq, l, LEFT);

	null->left->printNodePre();

	return 0;
}

static void strcp(char src[], char target[]) {
	int i = 0;
	while (src[i] != '\0') {
		target[i] = src[i];
		++i;
	}
	target[i] = '\0';
}

static void strcp(char src[], char target[], int start, int end) {
	int i = start;
	int j = 0;
	while (src[i] != '\0' && i < end) {
		target[j] = src[i];
		++i;
		++j;
	}
	target[j] = '\0';
}

static int length(char str[]) {
	int i = 0;
	while (str[i] != '\0') {
		++i;
	}
	return i;
}

/*
����������ȸ�������
��������������������
��������������Һ��

����������ͺ������ֱ����Ľ���������Ƶ��������Ķ���������������Ľ���ǹؼ���
�ݹ��ؽ�����
�����⣺ͨ�����������������֪�������ĸ���Ȼ������������зֳ�����֦
�����⣺���÷ֳ���������֦���ظ�������ķַ�
�����������ֳ�����֦����ֻ��1���ַ�
*/
