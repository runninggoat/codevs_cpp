
#include <stdio.h>

using namespace std;

int n;
int nums[101];
int max;

void quick_sort(int arr[], int l, int r);

int main() {

	scanf("%d", &n);

	if (n == 1) {
		printf("%d", 1);
		return 0;
	}

	for (int i=0; i<n; i++) {
		scanf("%d", &nums[i]);
	}

	//先排个序
	quick_sort(nums, 0, n - 1);

	max = 0;

	for (int i=0; i<n; i++) {
		if (n - i < max) {
			//如果前一个指针后面的数字数目小于最大等差数列长度，就退出程序
			break;
		}
		for (int j=i+1; j<n; j++) {
			if (n-j < max) {
				//如果后一个指针后面的数字数目小于最大等差数列长度，就退出本轮搜索
				break;
			}
			int dis = nums[j] - nums[i];
			int l = 2;
			int cur = nums[j];
			for (int k=j+1; k<n; k++) {
				if (nums[k] - cur > dis) {
					//如果下一项与目前项差值大于公差，后面的肯定更大,退出本轮搜索
					break;
				}
				if (nums[k] - cur == dis) {
					++l;
					cur = nums[k];
				}
			}
			if (l > max) {
				max = l;
			}
		}
	}

	printf("%d", max);

	return 0;
}

void quick_sort(int arr[], int l, int r) {
	if (r - l < 1) {
		return;
	}
	int front = l;
	int last = r;
	int key = arr[front];
	while (front < last) {
		while (front < last && arr[last] >= key) {
			--last;
		}
		arr[front] = arr[last];
		while (front < last && arr[front] <= key) {
			++front;
		}
		arr[last] = arr[front];
	}
	arr[front] = key;
	quick_sort(arr, l, front - 1);
	quick_sort(arr, front + 1, r);
}
