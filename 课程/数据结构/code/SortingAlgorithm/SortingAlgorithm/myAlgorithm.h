#pragma once
#pragma warning(disable:6031 6385 6386 26451)
#include <vector>
#include <iterator>
#include <functional>
#include <algorithm>
#include <stack>
#include <assert.h>

/*
**
**	infor :SortAlgorithm
**	author: acooz
**	data  : 2019.12.11
**
**/

using namespace std;

//static int IncrementSequenceHibbard[30] = { 30,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535,131071,262143,524287,1048575,2097151,4194303,8388607,16777215,33554431,67108863,134217727,268435455,536870911 };

//快速非递归优化，STL栈性能不高，简单实现一个栈
class myStack
{
public:
	myStack() { data = new int[100](); size = 100; length = 0; }
	bool isEmpty() { return length == 0; }
	bool isFull() { return length == size; }
	
	void push(int elem){
		if (isFull())
			expan();
		data[length++] = elem;
	}
	int  pop(){
		if (isEmpty())
			return -1;
		return data[--length];
	}
	
private:
	int* data;
	int size;
	int length;

	void expan() {
		int* newData = new int[size * 10]();
		for (int i = 0; i < size; i++)
			newData[i] = data[i];
		size = size * 10;
		delete data;
		data = newData;
	}
};


class Solution
{
	//为了方便传入函数指针，将成员函数改为static//
public:


	static void noSort(int arr[], int length)
	{
		if (length <= 1 || arr == nullptr)
			return;
		//构建堆
		for (int i = 0; i < length; i++)
			heapInsert(arr, i);

		for (int i = 0; i < 100; i++){
			cout << arr[i] << " ";
		}
	}


	static void bubbleSort(int arr[], int length)
	{
		if (length <= 1||arr==nullptr) 
			return;  
		for (int i = 0; i < length; i++) {
			bool flag = false;
			for (int j = 0; j < length - i - 1; j++) {
				if (arr[j] > arr[j+1]) {
					swap(arr, j, j+1);
					flag = true;		//通过标志减少循环次数
				}
			}
			if (!flag)
				break;
		}
	}

	static void selectSort(int arr[], int length)
	{
		if (length <= 1 || arr == nullptr)
			return;
		for (int i = 0; i < length - 1; i++){
			int minIndex = i;
			for (int j = i + 1; j < length; j++) {
				if (arr[minIndex] > arr[j])
					minIndex = j;		//遍历数组，选择最小的数
			}
			swap(arr, minIndex, i);
		}
	}

	static void insertSort(int arr[], int length)
	{
		if (length <= 1 || arr == nullptr)
			return; 
		for (int i = 1; i < length; i++) {
			int value = arr[i];
			int j = i - 1;
// 查找插入的位置
			for (; j >= 0; j--) {
				if (arr[j] > value)
					arr[j+1] = arr[j]; // 数据移动 
				else
					break; 
			}
			arr[j+1] = value; // 插入数据 
		}
	}

	//根据gap的选取， 复杂度O(n ^ (1.3—2))
	static void shellSort(int arr[], int length)
	{
		for (int gap = (length >> 1); gap > 0; gap >>= 1) {//xxxtag
			//魔改插入排序
			for (int i = gap; i < length; i++) {
				for (int j = i; j - gap >= 0 && arr[j - gap] > arr[j]; j -= gap)
					swap(arr, j - gap, j);
			}
		}
	}

	static void mergeSort(int arr[], int length)
	{
		if (arr == nullptr || length < 2)
			return;
		mergeSort(arr, 0, length - 1);
	}

	static void mergeSortN(int arr[], int length)
	{
		if (arr == nullptr || length < 2)
			return;
		mergeSortN(arr, 0, length - 1);
	}

	static void quickSort(int arr[], int length)
	{
		if (length <= 1 || arr == nullptr)
			return;
		quickSort(arr, 0, length - 1);
	}

	static void heapSort(int arr[], int length)
	{
		if (length <= 1 || arr == nullptr)
			return;
		//构建堆
		for (int i = 0; i < length; i++)
			heapInsert(arr, i);
		int size = length;
		while (size > 0) {
			swap(arr, 0, --size);
			heapify(arr, 0, size);
		}
	}

	static void countingSort(int arr[], int length)
	{
		if (length <= 1 || arr == nullptr)
			return;
		int max = arr[0];
		int lastIndex = 0;
		for (int i = 1; i < length; i++)
			max = arr[i] > max ? arr[i] : max;

		int* sortArr = new int[max + 1]();
		for (int j = 0; j < length; j++)
			sortArr[arr[j]]++;
		for (int k = 0; k < max + 1; k++) {
			while (sortArr[k] > 0) {
				arr[lastIndex++] = k;
				sortArr[k]--;
			}
		}
		delete[] sortArr;
	}

	static void bucketSort(int arr[], int length)
	{
		if (length < 2)
			return;
		
		int size=10;		//size 为一个桶的囊括的数的范围
		//拿到最大最小值
		int min = arr[0], max = arr[0];
		for (int i = 1; i < length; i++) {
			min = arr[i] < min ? arr[i] : min;
			max = arr[i] > max ? arr[i] : max;
		}

		if (min == max)
			return;

		//创建桶
		int bucketSize = (max - min) / size + 1;
		vector<vector<int>> bucket(bucketSize+1);		//申请vector时直接多申请一个，防止溢出
		int bid = 0;		

		//把数组中的数 扔进桶里
		for (int i = 0; i < length; i++) {
			bid = getGroupCount(arr[i], bucketSize, min, max);
			bucket[bid].push_back(arr[i]);
		}

		for (int i = 0; i < bucketSize; i++) {
		//对桶内进行插入排序。按照升序，保证稳定性
			int bucketLenth = int(bucket[i].size());

			if (bucketLenth <= 1)
				break;

			for (int j = 1; j < bucketLenth; j++) {
				int value = bucket[i][j];
				int k = j - 1;
				for (; k >= 0; k--) {
					if (bucket[i][k] > value)
						bucket[i][k + 1] = bucket[i][k];
					else
						break;
				}
				bucket[i][k+1] = value; // 插入数据
			}
		}

	//将排序好的桶内数据复制到数组上
		int index = 0;
		for (int i = 0; i < bucketSize+1; i++) {
			for (int j = 0; j < int(bucket[i].size()); j++) {
				arr[index++] = bucket[i][j];
			}
		}

		bucket.~vector();
	}

	static void radixSort(int arr[], int length)
	{
		if (length <= 1 || arr == nullptr)
			return;

		int d = maxbit(arr, length);
		int* tmp = new int[length];
		int* count = new int[10]; //计数器
		int i, j, k;
		int radix = 1;
		//进行d次排序
		for (i = 1; i <= d; i++) {

			//每次分配前清空计数器
			for (j = 0; j < 10; j++)
				count[j] = 0; 

			//统计每个桶中的记录数
			for (j = 0; j < length; j++){
				k = (arr[j] / radix) % 10; 
				count[k]++;
			}

			//将tmp中的位置依次分配给每个桶
			for (j = 1; j < 10; j++)
				count[j] = count[j - 1] + count[j]; 

			//将所有桶中记录依次收集到tmp中
			for (j = length - 1; j >= 0; j--) {
				k = (arr[j] / radix) % 10;
				tmp[count[k] - 1] = arr[j];
				count[k]--;
			}

			//将临时数组的内容复制到data中
			for (j = 0; j < length; j++) 
				arr[j] = tmp[j];
			radix = radix * 10;
		}

		delete[] tmp;
		delete[] count;
	}

	static void quickSortN(int arr[], int length)
	{
		//STL栈
		if (length <= 1 || arr == nullptr)
			return;
		quickSortN(arr, 0, length - 1);
	}

	static void quickSortNM(int arr[], int length)
	{
		//自定义栈
		if (length <= 1 || arr == nullptr)
			return;
		quickSortNM(arr, 0, length - 1);
	}

private:
	static void swap(int arr[], int a, int b){
		int tmp = arr[a];
		arr[a] = arr[b];
		arr[b] = tmp;
	}

	static void mergeSort(int arr[], int L, int R) {
		if (L == R)
			return;
		int mid = ((L + R) >> 1);
		mergeSort(arr, L, mid);
		mergeSort(arr, mid + 1, R);
		merge(arr, L, mid, R);
	}

	static void mergeSortN(int arr[], int L, int R) 
	{
		if (L == R)
			return;
		int length = R - L + 1;
		int gap = 1, left, mid, right;

		while (gap < length) {
			left = 0;
			while (left + gap < length) {
				mid = left + gap - 1;
				right = mid + gap;
				if (right > length - 1)
					right = length - 1;
				merge(arr, left, mid, right);
				left = right + 1;//下一次归并时第一关序列的下界 
			}
			gap *= 2;
		}
	}

	static void merge(int arr[], int L, int M, int R) {
		int* temp = new int[R - L + 1]();
		int i = 0;
		int pFirst = L, pSecond = M + 1;
		while (pFirst <= M && pSecond <= R)
			temp[i++] = arr[pFirst] < arr[pSecond] ? arr[pFirst++] : arr[pSecond++];

		while (pFirst <= M)
			temp[i++] = arr[pFirst++];

		while (pSecond <= R)
			temp[i++] = arr[pSecond++];

		//拷贝到arr
		for (int j = 0; j < (R - L + 1); j++)
			arr[L + j] = temp[j];
		delete[] temp;
	}

	static void quickSort(int arr[], int p, int r)
	{
		if (p >= r)
			return;
		//若每次partition取最后/最前的数，则在正序/逆序情况下会退化为冒泡排序，即复杂度为O(n^2)

		if (r - p + 1 < 15){
			int* start = arr + p;
			insertSort(start,r-p+1);
			return;
		}

		int q = partition(arr, p, r);
		quickSort(arr, p, q - 1);
		quickSort(arr, q + 1, r);
	}

	static void quickSortN(int arr[], int left, int right)
	{
		//手动利用栈来存储每次分块快排的起始点
		//栈非空时循环获取中轴入栈
		stack<int> s;
		if (left < right){
			int boundary = partition(arr, left, right);
			//确保左右分区存在
			if (boundary - 1 > left) {
				s.push(left);
				s.push(boundary - 1);
			}
			if (boundary + 1 < right) {
				s.push(boundary + 1);
				s.push(right);
			}

			while (!s.empty()){
				//得到某分区的左右边界 
				int r = s.top();
				s.pop();
				int l = s.top();
				s.pop();
				boundary = partition(arr, l, r);
				if (boundary - 1 > l) {
					s.push(l);
					s.push(boundary - 1);
				}
				if (boundary + 1 < r) {
					s.push(boundary + 1);
					s.push(r);
				}
			}
		}

	}

	static void quickSortNM(int arr[], int left, int right)
	{
		//手动利用栈来存储每次分块快排的起始点
		//栈非空时循环获取中轴入栈
		myStack s;
		int q = partition(arr, left, right);
		//确保左右分区存在
		if (q - 1 > left) {
			s.push(left);
			s.push(q - 1);
		}
		if (q + 1 < right) {
			s.push(q + 1);
			s.push(right);
		}
		while (!s.isEmpty()) {
			//得到某分区的左右边界 
			int r = s.pop();
			int l = s.pop();

			if (r - l + 1 < 15){
				insertSort(arr + l, r - l + 1);
			}
			else{
				q = partition(arr, l, r);
				if (q - 1 > l) {
					s.push(l);
					s.push(q - 1);
				}
				if (q + 1 < r) {
					s.push(q + 1);
					s.push(r);
				}
			}
		}
	}

	static void doPivot(int arr[],int left,int right)
	{
	//从数组中选择合适的pivot,并把其移至数组最右端,防止有序情况下退化
		
		int pivot = right;

		//每次取中间值
		pivot = left + ((right - left) >> 1);		

		//三数取中法简单实现
		//int mid = left + ((right - left) >> 1);
		//if (((arr[left] < arr[mid]) && (arr[right] > arr[mid])) || ((arr[left] > arr[mid]) && (arr[right] < arr[mid])))
		//	pivot = mid;
		//else if (((arr[left] < arr[right]) && (arr[mid] > arr[right])) || ((arr[left] > arr[right]) && (arr[mid] < arr[right])))
		//	pivot = right;
		//else
		//	pivot = left;

		//随机法
		//int rand1 = rand() % 10000;	int rand2 = rand() % 10000;
		//pivot= ((rand1 * 10000 + rand2) % (right - left) + left);
		 
		swap(arr, pivot, right);
	}

	static int	partition(int arr[], int left, int right)
	{
		doPivot(arr, left, right);
		int i = left;
		for (int j = left; j < right; j++) {
			if (arr[j] < arr[right]) {
				if (j != i)
					swap(arr, i, j);
				i++;
			}
		}
		swap(arr, i, right);
		return i;
	}

	static void heapInsert(int arr[], int index) {
		while (arr[index] > arr[(index - 1)>>1]) {
			swap(arr, index, ((index - 1)>>1));
			index = ((index - 1) >> 1);
		}
	}

	static void heapify(int arr[], int index, int size) {
		int leftChild = (index << 1) + 1;
		while (leftChild < size) {
			int maxChild = leftChild + 1 < size && arr[leftChild + 1] > arr[leftChild] ? leftChild + 1 : leftChild;
			int maxAll = arr[maxChild] > arr[index] ? maxChild : index;
			if (maxAll == index)
				break;
			swap(arr, maxAll, index);
			index = maxAll;
			leftChild = (index << 1) + 1;
		}
	}

	static int	getGroupCount(int value, int bucketSize, int min, int max) {
		float difference = float(max - min);
		float Vdifference = float(value - min);
		int count = int(Vdifference / difference * bucketSize);
		return count;
	}

	static int	maxbit(int arr[], int length) //求数据的最大位数
	{
		int maxData = arr[0];
		for (int i = 1; i < length; i++){
			if (arr[i]>maxData)
				maxData = arr[i];
		}

		int num = 1;
		while (maxData >= 10){
			maxData /= 10;
			++num;
		}

		return num;
	}
};