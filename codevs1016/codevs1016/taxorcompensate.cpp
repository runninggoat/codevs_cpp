
#include <stdio.h>

using namespace std;

#define MAX 10000 //最多可以保存1w个售价和销量数据对

int price[MAX]; //保存售价
int sale[MAX]; //保存销量

int len; //售价和销量数据对的长度
int C; //保存超过最高价后固定的销量递减常量
int expect;
int expect_index;

int measure; //政府调控的绝对值，减之为收税，加之为补贴

int main() {

	len = 0; //初始化，还没读入数据对

	measure = 0;

	scanf("%d", &expect); //读入政府预期价格

	scanf("%d %d", &price[len], &sale[len]); //读入成本价及其销量
	++len;

	int p, s;
	while (1) {
		scanf("%d %d", &p, &s); //读入成本价及其销量
		if (-1 == p && -1 == s) break;

		//下面将两个相邻价格之间的价格和销量数据对计算出来
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

	scanf("%d", &C); //读入销量递减常量

	//下面将最高价之后的价格和销量数据对计算出来，直到销量为0为止
	while (1) {
		int tmp = sale[len-1] - C;
		if (tmp <= 0) break;
		price[len] = price[len-1] + 1;
		if (expect == price[len]) expect_index = len;
		sale[len] = tmp;
		++len;
	}

	//先找出政府调控为0的时候最大总利润的价格
	int max_gain = 0;
	int max_index = 0;
	for (int i=1; i<len; i++) {
		int gain = (price[i] - price[0]) * sale[i];
		//注意！当政府预期价格的总利润和最大的一样大时，也算政府价达到最大总利润
		if (max_gain <= gain) {
			max_gain = gain;
			max_index = i;
		}
	}

	/*
		1、如果最大总利润的价格与政府期望价一致，则找到答案
		2、如果最大总利润的价格小于政府期望价，则通过收税使较低价格的最大总利润下降速度更快
		如果在收税达到政府期望价之前能够使期望价获得最大总利润，则成功，否则找不到解
		3、如果最大总利润的价格大于政府期望价，则通过补贴使期望价格的总利润增速快过较高的价格
		直到得到结果为止（补贴没有上限，收税有下限），但是也有可能在某一个调控价位之后发生跳跃，
		最大总利润对应的价格又小于政府期望价格，这种也无解
	*/
	if (max_index == expect_index) { //情况1
		printf("%d", measure);
		return 0;
	} else if (max_index < expect_index) { //情况2
		while (1) {
			--measure; //增加收税
			int mg = 0;
			int mi = 0;
			for (int i=0; i<len; i++) {
				int gain = (price[i] - price[0] + measure) * sale[i];
				if (gain <= 0 && expect_index == i) {
					//政府预期价在高收税情况下依然未能达到最高利润，无解
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
	} else if (max_index > expect_index) { //情况3
		while (1) {
			++measure; //增加补贴
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
				//在高补贴情况下最大总利润的价格跳跃到比政府预期价更小，无解
				printf("%s", "NO SOLUTION");
				return 0;
			}
		}
	}

	return 0;
}

/**
1、未干预情况下，逐个计算出从成本到最高销售利润的所有销售利润，找出最高销售利润
2、政府的+1或-1都会导致所有价位的销售利润增加或减少一个销售量大小
3、如果最高销售利润的单价在预期价之前，则应实行税收；否则实行补贴。
利用减少或增加的销售量平衡预期价位的总利润
4、如果预期利润太高或太低都会导致无解

例子：
售价  销量  0     1       2       3       4
28    130   0     130     260     390     520
29    125   125   250     375     500     625
30    120   240   360     480     600     720
31    110   330   440     550     660     770--OK
32    95    380   475     570     665     760
33    80    400   480     560     640     720
34    65    390   455
......

所以输出的答案是4

另一个例子：
政府预期 34
售价  销量  0     -1      -2      -3      -4
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
所以输出的答案是 -1

expect 4011
售价    销量     0    -1
4000    40000    
4010    39890
4011    39880
*/
