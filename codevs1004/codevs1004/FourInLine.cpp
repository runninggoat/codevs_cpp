
#include <stdio.h>

using namespace std;

int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}; //Ԥ������������4������

static void squarecp(char src[4][4], char des[4][4]); //����4*4����

static void blankcp(int src[2][2], int des[2][2]); //����2*2����

static char nextAllow(char cur); //���õ�ǰ��������ɫ�����һ�����е�������ɫ

static bool isEnd(char matrix[4][4]); //�ж�б�ߡ���ᡢ�����Ƿ��������״̬

class State { //״̬�࣬��Ϊ�����еĽڵ�
public:
	char square[4][4]; //��¼����λ�õ����ӻ��λ
	int blank[2][2]; //��¼������λ�����꣬blank[0][0]Ϊ��һ����λ��y��x
	State* next; //��һ��״̬��ָ��
	char allow; //��¼��������һ������������ɫ�߶�
	State* clone(); //��¡��ǰ״̬�����
	State();
	~State();
};

State::State() {}
State::~State() {
	delete next;
}

State* State::clone() {
	State* s = new State();
	squarecp(square, s->square); //��������
	blankcp(blank, s->blank); //������λ������
	return s;
}

class Queue {
public:
	State* HEAD;
	State* TAIL;
	State* cur; //ָ���β�����һ��Ԫ�أ�TAIL֮ǰ���Ǹ���
	void enqueue(State* s); //���һ��״̬
	State* dequeue(); //����һ��״̬
	bool isEmpty(); //�ж϶����Ƿ�Ϊ��
	int l; //���еĳ���
	Queue();
	~Queue();
};

Queue::Queue() { //��ʼ�����У�����ͷβ����������
	HEAD = new State();
	TAIL = new State();
	HEAD->next = TAIL;
	cur = HEAD;
	l = 0;
}
Queue::~Queue() {
	delete HEAD;
}

void Queue::enqueue(State* s) { //���һ��״̬�ڵ�
	cur->next = s;
	s->next = TAIL;
	cur = s;
	++l;
}

/*
����һ��״̬�ڵ�
���ô˷���ǰ��Ҫ�ȱ�֤���в���
*/
State* Queue::dequeue() {
	State* ret = HEAD->next;
	if (cur == ret) { //��������Ƕ���ֻʣ1���ڵ㣬�൱������curָ��
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

int step = 0; //��¼����

int main() {
	
	//�����ʼ��״̬����
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
	//��ʼ״̬��ȡ���
	
	//�������⣬��һ�������ǰ�������Ҳ���Ժ������ߣ������½�2��״̬��һ���ǰ�������ߣ���һ���Ǻ��������
	State* s1 = new State();
	squarecp(initial, s1->square);
	blankcp(b, s1->blank);
	State* s2 = s1->clone();
	s1->allow = 'B';
	s2->allow = 'W';

	//�����2��״̬
	Queue* q = new Queue(); //q���б�������һ�غϿ��Գ��ֵ�����״̬
	q->enqueue(s1);
	q->enqueue(s2);
	Queue* buffer = new Queue(); //buffer�������ڱ�������һ�غϵ�״̬��������״̬����һ�غ�ʱ�������Щ��״̬����q��������һ�غϵĴ���

	bool end = false;
	if (isEnd(initial)) { //���￼�ǳ�ʼ״̬��Ϊ����״̬�����������������
		end = true;
	}
	// BFS
	while(!q->isEmpty() && !end) { //q��Ϊ�գ����п����ߵ�״̬
		while(!q->isEmpty()) { //q��Ϊ�գ����п����ߵ�״̬
			State* curS = q->dequeue(); //����һ��״̬
			for (int i=0; i<4; i++) {
				//��һ����λ��ѡ��
				int y = curS->blank[0][0] + directions[i][0];
				int x = curS->blank[0][1] + directions[i][1];
				if (x >=0 && x <= 3 && y >= 0 && y <= 3) { //Խ��Ĳ�����
					if (curS->square[y][x] == curS->allow) {
						State* s = new State();
						squarecp(curS->square, s->square);
						blankcp(curS->blank, s->blank);
						s->square[y][x] = 'O'; //������λ�Ͷ�Ӧ��ɫ������
						s->square[curS->blank[0][0]][curS->blank[0][1]] = curS->allow;
						s->blank[0][0] = y; //���¿�λ������
						s->blank[0][1] = x;
						s->allow = nextAllow(curS->allow); //�ƶ���һ���ƶ������ӵ���ɫ
						buffer->enqueue(s); //��״̬���
						if (isEnd(s->square)) { //����ﵽ����״̬����������
							end = true;
							break;
						}
					}
				}

				//�ڶ�����λ��ѡ��
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

		//����q��buffer��׼��������һ�غ�
		Queue* tmp = q;
		q = buffer;
		buffer = tmp;

		++step;

		if (end) {
			break;
		}
	}

	//��ɣ���ӡ���
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
	else ret = 'B'; //����
	return ret;
}

static bool isEnd(char matrix[4][4]) {
	//б��
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

	//���
	for (int i=0; i<4; i++) {
		if (matrix[i][0] == matrix[i][1]
		&& matrix[i][1] == matrix[i][2]
		&& matrix[i][2] == matrix[i][3]) {
			return true;
		}
	}

	//����
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
�ڰ��������ߣ��ڰ�˫�������ƶ�������һ�������ӽ���
ÿһ���غ�
��һ��4*4�����¼Ŀǰ���ӵ�״̬
��һ��2*2�����¼������λ��λ��
ѡȡһ����λ��ѡȡ��λ��������������һ�����ߵ����ӣ���¡һ��Ŀǰ�ľ����ƶ���Ӧ���ӣ���¼��λ�仯������һ���µ�״̬�����������
ÿһ��״̬���������4*4���󣬿�λ���2*2����
*/
