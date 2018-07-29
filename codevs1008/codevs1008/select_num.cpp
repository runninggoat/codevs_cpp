
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
	//�˳�����
	if (n-level+selected < k || (level == n && selected < k)) { //ʣ�µĲ��������Ѿ�ѡ��Ĳ�������С��k������ѡ��̫�ٵ����
		return;
	}
	if (selected == k) { //ѡ��k�����ж��Ƿ�����
		if (isSushu(sum)) {
			++result;
		}
		return;
	}

	//��ѡ���
	dfs(level+1, selected, sum);

	//ѡ���
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
