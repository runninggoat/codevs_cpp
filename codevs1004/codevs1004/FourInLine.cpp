
#include <stdio.h>

using namespace std;

int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}; //预定义上右下左4个方向

static void squarecp(char src[4][4], char des[4][4]); //复制4*4矩阵

static void blankcp(int src[2][2], int des[2][2]); //复制2*2矩阵

static char nextAllow(char cur); //利用当前的棋子颜色获得下一步可行的棋子颜色

static bool isEnd(char matrix[4][4]); //判断斜线、打横、打竖是否满足结束状态

class State { //状态类，作为队列中的节点
public:
	char square[4][4]; //记录各个位置的棋子或空位
	int blank[2][2]; //记录两个空位的坐标，blank[0][0]为第一个空位的y、x
	State* next; //下一个状态的指针
	char allow; //记录接下来这一步允许哪种颜色走动
	State* clone(); //克隆当前状态，深拷贝
	State();
	~State();
};

State::State() {}
State::~State() {
	delete next;
}

State* State::clone() {
	State* s = new State();
	squarecp(square, s->square); //拷贝棋盘
	blankcp(blank, s->blank); //拷贝空位的坐标
	return s;
}

class Queue {
public:
	State* HEAD;
	State* TAIL;
	State* cur; //指向队尾的最后一个元素（TAIL之前的那个）
	void enqueue(State* s); //入队一个状态
	State* dequeue(); //出队一个状态
	bool isEmpty(); //判断队列是否为空
	int l; //队列的长度
	Queue();
	~Queue();
};

Queue::Queue() { //初始化队列，产生头尾，并连起来
	HEAD = new State();
	TAIL = new State();
	HEAD->next = TAIL;
	cur = HEAD;
	l = 0;
}
Queue::~Queue() {
	delete HEAD;
}

void Queue::enqueue(State* s) { //入队一个状态节点
	cur->next = s;
	s->next = TAIL;
	cur = s;
	++l;
}

/*
出队一个状态节点
调用此方法前需要先保证队列不空
*/
State* Queue::dequeue() {
	State* ret = HEAD->next;
	if (cur == ret) { //此种情况是队列只剩1个节点，相当于重置cur指针
		cur = HEAD;
	}
	HEAD->next = HEAD->next->next;
	--l;
	return ret;
}

bool Queue::isEmpty() {
	if (HEAD->next == TAIL || l == 0) {
		cur = HEAD;
		return true;
	}
	return false;
}

int step = 0; //记录步数

int main() {
	
	//读入初始的状态数据
	char initial[4][4];
	int b[2][2];
	int bi = 0;

	char line[5];

	for (int i=0; i<4; i++) {
		scanf("%s", &line);
		for (int j=0; j<4; j++) {
			initial[i][j] = line[j];
			if (line[j] == 'O') {
				b[bi][0] = i;
				b[bi][1] = j;
				++bi;
			}
		}
	}
	//初始状态读取完成
	
	//根据题意，第一步可以是白棋先走也可以黑棋先走，所有新建2个状态，一个是白棋可以走，另一个是黑棋可以走
	State* s1 = new State();
	squarecp(initial, s1->square);
	blankcp(b, s1->blank);
	State* s2 = s1->clone();
	s1->allow = 'B';
	s2->allow = 'W';

	//入队这2个状态
	Queue* q = new Queue(); //q队列保存了这一回合可以出现的所有状态
	q->enqueue(s1);
	q->enqueue(s2);
	Queue* buffer = new Queue(); //buffer队列用于保存由这一回合的状态产生的新状态，下一回合时将会把这些新状态交给q，进行下一回合的处理

	bool end = false;
	if (isEnd(initial)) { //这里考虑初始状态即为结束状态（出现四连）的情况
		end = true;
	}
	// BFS
	while(!q->isEmpty() && !end) { //q不为空，即有可以走的状态
		while(!q->isEmpty()) { //q不为空，即有可以走的状态
			State* curS = q->dequeue(); //出队一个状态
			for (int i=0; i<4; i++) {
				//第一个空位的选择
				int y = curS->blank[0][0] + directions[i][0];
				int x = curS->blank[0][1] + directions[i][1];
				if (x >=0 && x <= 3 && y >= 0 && y <= 3) { //越界的不处理
					if (curS->square[y][x] == curS->allow) {
						State* s = new State();
						squarecp(curS->square, s->square);
						blankcp(curS->blank, s->blank);
						s->square[y][x] = 'O'; //交换空位和对应颜色的棋子
						s->square[curS->blank[0][0]][curS->blank[0][1]] = curS->allow;
						s->blank[0][0] = y; //更新空位的坐标
						s->blank[0][1] = x;
						s->allow = nextAllow(curS->allow); //推断下一步移动的棋子的颜色
						buffer->enqueue(s); //新状态入队
						if (isEnd(s->square)) { //如果达到结束状态，可以跳出
							end = true;
							break;
						}
					}
				}

				//第二个空位的选择
				y = curS->blank[1][0] + directions[i][0];
				x = curS->blank[1][1] + directions[i][1];
				if (x >=0 && x <= 3 && y >= 0 && y <= 3) {
					if (curS->square[y][x] == curS->allow) {
						State* s = new State();
						squarecp(curS->square, s->square);
						blankcp(curS->blank, s->blank);
						s->square[y][x] = 'O';
						s->square[curS->blank[1][0]][curS->blank[1][1]] = curS->allow;
						s->blank[1][0] = y;
						s->blank[1][1] = x;
						s->allow = nextAllow(curS->allow);
						buffer->enqueue(s);
						if (isEnd(s->square)) {
							end = true;
							break;
						}
					}
				}
			}
		}

		//交换q和buffer，准备进入下一回合
		Queue* tmp = q;
		q = buffer;
		buffer = tmp;

		++step;

		if (end) {
			break;
		}
	}

	//完成，打印结果
	printf("%d", step);

	return 0;
}


static void squarecp(char src[4][4], char des[4][4]) {
	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			des[i][j] = src[i][j];
		}
	}
}

static void blankcp(int src[2][2], int des[2][2]) {
	for (int i=0; i<2; i++) {
		for (int j=0; j<2; j++) {
			des[i][j] = src[i][j];
		}
	}
}

static char nextAllow(char cur) {
	char ret;
	if ('B' == cur) ret = 'W';
	else if ('W' == cur) ret = 'B';
	else ret = 'B'; //出错
	return ret;
}

static bool isEnd(char matrix[4][4]) {
	//斜线
	if (matrix[0][0] == matrix[1][1]
	&& matrix[1][1] == matrix[2][2]
	&& matrix[2][2] == matrix[3][3]) {
		return true;
	}

	if (matrix[0][3] == matrix[1][2]
	&& matrix[1][2] == matrix[2][1]
	&& matrix[2][1] == matrix[3][0]) {
		return true;
	}

	//打横
	for (int i=0; i<4; i++) {
		if (matrix[i][0] == matrix[i][1]
		&& matrix[i][1] == matrix[i][2]
		&& matrix[i][2] == matrix[i][3]) {
			return true;
		}
	}

	//打竖
	for (int i=0; i<4; i++) {
		if (matrix[0][i] == matrix[1][i]
		&& matrix[1][i] == matrix[2][i]
		&& matrix[2][i] == matrix[3][i]) {
			return true;
		}
	}

	return false;
}

/*
黑白任意先走，黑白双方交替移动，任意一方四连子结束
每一个回合
用一个4*4矩阵记录目前棋子的状态
用一个2*2矩阵记录两个空位的位置
选取一个空位，选取空位的上下左右其中一个可走的棋子，克隆一个目前的矩阵，移动相应棋子，记录空位变化，产生一个新的状态，放入队列中
每一个状态：棋盘情况4*4矩阵，空位情况2*2矩阵
*/
