
#include <stdio.h>
#define MAX 20

using namespace std;

int n, k, result=0;
int x[MAX];

bool isSushu(int num) {
	for (int div=2; div*div<=num; div++) {
		if (num % div == 0) {
			return false;
		}
	}
	return true;
}

void dfs(int level, int selected, int sum) {
	//退出条件
	if (n-level+selected < k || (level == n && selected < k)) { //剩下的层数加上已经选择的层数总数小于k，属于选得太少的情况
		return;
	}
	if (selected == k) { //选够k个，判断是否素数
		if (isSushu(sum)) {
			++result;
		}
		return;
	}

	//不选这层
	dfs(level+1, selected, sum);

	//选这层
	dfs(level+1, selected+1, sum+x[level]);
}

int main() {
	
	scanf("%d %d", &n, &k);

	for (int i = 0; i < n; i++) {
		scanf("%d", &x[i]);
	}

	dfs(0, 0, 0);

	printf("%d", result);

	return 0;
}
