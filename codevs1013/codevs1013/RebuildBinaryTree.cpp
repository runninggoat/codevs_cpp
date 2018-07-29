
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
parent：父节点
midSeq：中序遍历字符串
postSeq：后序遍历字符串
len：字符串长度，不包含'\0'
tag：标签，只能是LEFT（1）或RIGHT（2）
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

	char r = postSeq[len - 1]; //后序遍历中最后一个字符为根
	int midCur = 0; //在中序遍历中的游标，用于记录找到根的下标
	for (midCur=0; midCur<len; midCur++) {
		if (r == midSeq[midCur]) {
			break;
		}
	}

	//为找到的这个根创建节点
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
		//左子节点递归
		char* subLeftMidStr = new char[midCur + 1];
		strcp(midSeq, subLeftMidStr, 0, midCur);

		char* subLeftPostStr = new char[midCur + 1];
		strcp(postSeq, subLeftPostStr, 0, midCur);

		iterate(n, subLeftMidStr, subLeftPostStr, midCur, LEFT);
	}

	//右子节点递归
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
先序遍历：先根后左右
中序遍历：先左而根后右
后序遍历：先左右后根

若存在中序和后序两种遍历的结果，可以推导出完整的二叉树（中序遍历的结果是关键）
递归重建树法
大问题：通过先序遍历或后序遍历知道根是哪个，然后在中序遍历中分出左右枝
子问题：利用分出来的左右枝，重复大问题的分法
结束条件：分出来的枝长度只有1个字符
*/
