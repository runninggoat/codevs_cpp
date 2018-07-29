
#include <stdio.h>

using namespace std;

#define MAX 1000
#define MAX_N 10
#define MAX_M 10

int n, m;

int matrix[MAX_N][MAX_M];

int b[MAX_N];

int min;

bool found;

int ans[MAX_N];
int color[MAX_N];

void dfs(int index);

int main() {

	scanf("%d %d", &n, &m);

	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++) {
			scanf("%d", &matrix[i][j]);
		}
	}

	for (int i=0; i<n; i++) {
		scanf("%d", &b[i]);
		ans[i] = 0;
		color[i] = 0;
	}

	min = 1000;
	found = false;

	dfs(0);

	if (found) {
		printf("%d", min);
	} else {
		printf("%s", "alternative!");
	}

	return 0;
}

void dfs(int index) {
	if (index == n) {
		//找到一种组合
		for (int j=0; j<m; j++) {
			if (j < m-1 && color[j] != color[j+1]) {
				//某两种颜色的纸数量不同，这个组合不满足要求
				return;
			}
			if (0 == color[j]) {
				return;
			}
		}
		int total = 0;
		for (int j=0; j<m; j++) {
			total += color[j];
			if (total > MAX || total == 0) {
				//如果需要生产的数量超过最大值1000或者一只也没有（0），则直接抛弃这种结果
				return;
			}
		}
		found = true;
		if (total < min) {
			min = total;
		}
		//for (int i=0; i<n; i++) {
		//	printf("%d ", ans[i]);
		//}printf("--%d\n", total);
		return;
	}

	for (int i=0; i<=b[index]; i++) {
		ans[index] = i;
		int tmp = 0;
		for (int j=0; j<m; j++) {
			tmp += color[j] + matrix[index][j] * i;
		}
		if (tmp > min) {
			continue;
		}
		for (int j=0; j<m; j++) {
			color[j] += matrix[index][j] * i;
		}
		dfs(index+1);
		for (int j=0; j<m; j++) {
			color[j] -= matrix[index][j] * i;
		}
	}
}
