
#include <stdio.h>

using namespace std;

#define MAX 10000 //�����Ա���1w���ۼۺ��������ݶ�

int price[MAX]; //�����ۼ�
int sale[MAX]; //��������

int len; //�ۼۺ��������ݶԵĳ���
int C; //���泬����߼ۺ�̶��������ݼ�����
int expect;
int expect_index;

int measure; //�������صľ���ֵ����֮Ϊ��˰����֮Ϊ����

int main() {

	len = 0; //��ʼ������û�������ݶ�

	measure = 0;

	scanf("%d", &expect); //��������Ԥ�ڼ۸�

	scanf("%d %d", &price[len], &sale[len]); //����ɱ��ۼ�������
	++len;

	int p, s;
	while (1) {
		scanf("%d %d", &p, &s); //����ɱ��ۼ�������
		if (-1 == p && -1 == s) break;

		//���潫�������ڼ۸�֮��ļ۸���������ݶԼ������
		int last_p = price[len-1];
		int last_s = sale[len-1];
		int decrement = (last_s - s) / (p - last_p);
		while (price[len-1] != p-1) {
			price[len] = price[len-1] + 1;
			if (expect == price[len]) expect_index = len;
			sale[len] = sale[len-1] - decrement;
			++len;
		}
		price[len] = p;
		if (expect == price[len]) expect_index = len;
		sale[len] = s;
		++len;
	}

	scanf("%d", &C); //���������ݼ�����

	//���潫��߼�֮��ļ۸���������ݶԼ��������ֱ������Ϊ0Ϊֹ
	while (1) {
		int tmp = sale[len-1] - C;
		if (tmp <= 0) break;
		price[len] = price[len-1] + 1;
		if (expect == price[len]) expect_index = len;
		sale[len] = tmp;
		++len;
	}

	//���ҳ���������Ϊ0��ʱ�����������ļ۸�
	int max_gain = 0;
	int max_index = 0;
	for (int i=1; i<len; i++) {
		int gain = (price[i] - price[0]) * sale[i];
		//ע�⣡������Ԥ�ڼ۸�������������һ����ʱ��Ҳ�������۴ﵽ���������
		if (max_gain <= gain) {
			max_gain = gain;
			max_index = i;
		}
	}

	/*
		1��������������ļ۸�������������һ�£����ҵ���
		2��������������ļ۸�С�����������ۣ���ͨ����˰ʹ�ϵͼ۸������������½��ٶȸ���
		�������˰�ﵽ����������֮ǰ�ܹ�ʹ�����ۻ�������������ɹ��������Ҳ�����
		3��������������ļ۸�������������ۣ���ͨ������ʹ�����۸�����������ٿ���ϸߵļ۸�
		ֱ���õ����Ϊֹ������û�����ޣ���˰�����ޣ�������Ҳ�п�����ĳһ�����ؼ�λ֮������Ծ��
		����������Ӧ�ļ۸���С�����������۸�����Ҳ�޽�
	*/
	if (max_index == expect_index) { //���1
		printf("%d", measure);
		return 0;
	} else if (max_index < expect_index) { //���2
		while (1) {
			--measure; //������˰
			int mg = 0;
			int mi = 0;
			for (int i=0; i<len; i++) {
				int gain = (price[i] - price[0] + measure) * sale[i];
				if (gain <= 0 && expect_index == i) {
					//����Ԥ�ڼ��ڸ���˰�������Ȼδ�ܴﵽ��������޽�
					printf("%s", "NO SOLUTION");
					return 0;
				}
				if (gain <= 0) continue;
				if (mg <= gain) {
					mg = gain;
					mi = i;
				}
			}
			if (mi == expect_index) {
				printf("%d", measure);
				return 0;
			}
		}
	} else if (max_index > expect_index) { //���3
		while (1) {
			++measure; //���Ӳ���
			int mg = 0;
			int mi = 0;
			for (int i=0; i<len; i++) {
				int gain = (price[i] - price[0] + measure) * sale[i];
				if (mg <= gain) {
					mg = gain;
					mi = i;
				}
			}
			if (mi == expect_index) {
				printf("%d", measure);
				return 0;
			}
			if (mi < expect_index) {
				//�ڸ߲�����������������ļ۸���Ծ��������Ԥ�ڼ۸�С���޽�
				printf("%s", "NO SOLUTION");
				return 0;
			}
		}
	}

	return 0;
}

/**
1��δ��Ԥ����£����������ӳɱ����������������������������ҳ������������
2��������+1��-1���ᵼ�����м�λ�������������ӻ����һ����������С
3����������������ĵ�����Ԥ�ڼ�֮ǰ����Ӧʵ��˰�գ�����ʵ�в�����
���ü��ٻ����ӵ�������ƽ��Ԥ�ڼ�λ��������
4�����Ԥ������̫�߻�̫�Ͷ��ᵼ���޽�

���ӣ�
�ۼ�  ����  0     1       2       3       4
28    130   0     130     260     390     520
29    125   125   250     375     500     625
30    120   240   360     480     600     720
31    110   330   440     550     660     770--OK
32    95    380   475     570     665     760
33    80    400   480     560     640     720
34    65    390   455
......

��������Ĵ���4

��һ�����ӣ�
����Ԥ�� 34
�ۼ�  ����  0     -1      -2      -3      -4
28    130   0
29    125   125   0
30    120   240   120     0
31    110   330   220     110     0
32    95    380   285     190     95      0
33    80    400   320     240     160     80
34    65    390   325--OK     260     195     130
35    50    350   300
36    35    280   
37    20    180
38    5     50
��������Ĵ��� -1

expect 4011
�ۼ�    ����     0    -1
4000    40000    
4010    39890
4011    39880
*/
