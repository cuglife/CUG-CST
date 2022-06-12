#include <iostream>
#include <chrono>
#include <conio.h>
#include <fstream>
#include <string> 
#include <algorithm> 
#include <cmath>
#include <ctime>
#include <windows.h>
#include "myAlgorithm.h"


/*
**
**	infor :test
**	author: acooz
**	data  : 2019.12.11
**
**/

using namespace std;
using namespace chrono;

//用于输出排序名
const string sortNameArr[12] = { "","冒泡排序","选择排序" ,"插入排序" ,"希尔排序" ,"归并排序" ,"快速排序","堆排序" ,"计数排序" ,"桶排序" ,"基数排序" ,"快排非递归" };

int getRange()
{
	cout << "请输入生成数据规模......" << endl;
	int range;
	while (cin >> range) {
		if (range < 20)
			cout << "数据范围太小了,请重试" << endl;
		else
			break;
	}
	return range;
}

int* initData(int range)
{
//随机数生成范围
	const int MAX = range;
	const int MIN = 0;

	int* data = new int[range];
	srand((int)time(0));
	for (int i = 0; i < range; i++) {
		int rand1 = rand() % 10000;
		int rand2 = rand() % 10000;
		data[i] = ((rand1 * 10000 + rand2) % (MAX - MIN + 1) + MIN);
	}
	return data;
}

int* arrayCopy(int oldArray[], int length) {
	if (oldArray == nullptr)
		return nullptr;

	int* newArray = new int[length];
	for (int i = 0; i < length; i++)
		newArray[i] = oldArray[i];
	return newArray;
}

void output(int arr[], int len,string textName)
{
	//把数据输出到文件

	cout << "正在保存...\r";		Sleep(300);
	ofstream output(textName+".txt");
	if (output.is_open()) {
		for (int i = 0; i < len; i++)
			output << arr[i] << "\n";
		output.close();
	}
	cout << "保存完毕...\r";	Sleep(300);

	//输出前20个数据

	//cout << "排序结果（选前20个" << endl;
	//int max=len-1;
	//int n = 10;
	//for (int i = 0; i < n; i++)
	//	cout << arr[max--] << "\n";
}


void output(int arr[], int len, int n,int way)
{
	if (way == 1) {
		int max = len - 1;
		for (int i = 0; i < n; i++)
			cout << arr[max--] << "\n";
	}
	else{
		for (int i = 0; i < n; i++)
			cout << arr[i] << "\n";
	}
	
}

void showMenu()
{
	cout << "——————请输入序号选择排序方式——————" << endl;
	for (int i = 1; i < 12; i++)
		cout << "\t\t" << i << "." << sortNameArr[i] << endl;
	cout << "———————————————————————" << endl;
}

void showTime(void(*pfunction)(int arr[], int length), int arr[], int length,string sortName)
{
	auto start = system_clock::now();
	pfunction(arr, length);
	auto duration = duration_cast<microseconds>(system_clock::now() - start);
	cout << sortName<<"耗时:\t" << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s" << endl;
	//output(arr, length, sortName);
}

void mySort(int sortWay, int arr[], int length)
{
	switch (sortWay) {
	case 1:
		showTime(&Solution::bubbleSort, arr, length, sortNameArr[sortWay]);		break;
	case 2:
		showTime(&Solution::selectSort, arr, length, sortNameArr[sortWay]);		break;
	case 3:
		showTime(&Solution::insertSort, arr, length, sortNameArr[sortWay]);		break;
	case 4:
		showTime(&Solution::shellSort, arr, length, sortNameArr[sortWay]);		break;
	case 5:
		showTime(&Solution::mergeSortN, arr, length, sortNameArr[sortWay]);		break;
	case 6:
		showTime(&Solution::quickSort, arr, length, sortNameArr[sortWay]);		break;
	case 7:
		showTime(&Solution::heapSort, arr, length, sortNameArr[sortWay]);		break;
	case 8:
		showTime(&Solution::countingSort, arr, length, sortNameArr[sortWay]);	break;
	case 9:
		showTime(&Solution::bucketSort, arr, length, sortNameArr[sortWay]);		break;
	case 10:
		showTime(&Solution::radixSort, arr, length, sortNameArr[sortWay]);		break;
	case 11:
		showTime(&Solution::quickSortNM, arr, length, sortNameArr[sortWay]);	break;
	default:
		cout << "输入序号错误" << endl;											break;
	}
}

void mySortTime(int arr[], int length) {
	int* testCopy = new int[length]();
	//不对复杂度为O(n^2)的排序算法进行统计
	//testCopy = arrayCopy(arr, length);	showTime(&Solution::bubbleSort, testCopy, length, sortNameArr[1]);		delete[] testCopy;
	//testCopy = arrayCopy(arr, length);	showTime(&Solution::selectSort, testCopy, length, sortNameArr[2]);		delete[] testCopy;
	//testCopy = arrayCopy(arr, length);	showTime(&Solution::insertSort, testCopy, length, sortNameArr[3]);		delete[] testCopy;
	//testCopy = arrayCopy(arr, length);	showTime(&Solution::shellSort, testCopy, length, sortNameArr[4]);		delete[] testCopy;
	testCopy = arrayCopy(arr, length);	showTime(&Solution::mergeSort, testCopy, length, sortNameArr[5]);		delete[] testCopy;
	testCopy = arrayCopy(arr, length);	showTime(&Solution::quickSort, testCopy, length, sortNameArr[6]);		delete[] testCopy;
	testCopy = arrayCopy(arr, length);	showTime(&Solution::heapSort, testCopy, length, sortNameArr[7]);		delete[] testCopy;
	testCopy = arrayCopy(arr, length);	showTime(&Solution::countingSort, testCopy, length, sortNameArr[8]);	delete[] testCopy;
	testCopy = arrayCopy(arr, length);	showTime(&Solution::bucketSort, testCopy, length, sortNameArr[9]);		delete[] testCopy;
	testCopy = arrayCopy(arr, length);	showTime(&Solution::radixSort, testCopy, length, sortNameArr[10]);		delete[] testCopy;
	testCopy = arrayCopy(arr, length);	showTime(&Solution::quickSortNM, testCopy, length, sortNameArr[11]);	delete[] testCopy;
	testCopy = arrayCopy(arr, length);	showTime(&Solution::noSort, testCopy, length, "构建堆");					delete[] testCopy;

	//testCopy = arrayCopy(arr, length);	showTime(&Solution::quickSortN, testCopy, length, "快排非递归STL栈");		delete[] testCopy;

	testCopy = nullptr;
	cout << "                       "<<endl;
}

void myTest()
{
	int range = getRange();
	cout << "生成数据中......\r";			Sleep(200);

		int* randomData = initData(range);

		int* positiveData = new int[range];
		for (int i = 0; i < range; i++)
			positiveData[i] = i;

		int* ReverseData = new int[range];
		for (int i = 0; i < range; i++)
			ReverseData[range - i - 1] = i;

	cout << "数据生成完毕......\r";			Sleep(300);

	system("cls");

	cout << "请选择排序数据类型......" << endl;
	cout << "01.随机数" << endl;
	cout << "02.正序" << endl;
	cout << "03.逆序" << endl;

	int dataType;
	while (true) {
		cin >> dataType;
		if (dataType != 1 && dataType != 2 && dataType != 3)
			cout << "输入序号错误,请重新输入！" << endl;
		else
			break;
	}
	
	system("cls");

	while (true){

		int* testCopy = new int[range]();
		if (dataType == 1)
			testCopy = arrayCopy(randomData, range);
		else if (dataType == 2)
			testCopy = arrayCopy(positiveData, range);
		else if (dataType == 3)
			testCopy = arrayCopy(ReverseData, range);

		showMenu();
		int sortWay;		cin >> sortWay;
		system("cls");
		cout << "排序中......";
		mySort(sortWay, testCopy, range);
		cout << "请输入n和最大(1)最小（其他）" << endl;
		int n; int way;
		cin >> n >> way;
		output(testCopy, range, n,way);

		delete[] testCopy;
		_getch();
		system("cls");
	}
}

void allMyShow()
{
	int range=getRange();		//当数据量大于10000时,复杂度为n^2的排序算法会需要较长时间
	//int range = 10000;
		int* randomData = initData(range);

		int* positiveData = new int[range];
		for (int i = 0; i < range; i++)
			positiveData[i] = i;

		int* ReverseData = new int[range];
		for (int i = 0; i < range; i++)
			ReverseData[range - i - 1] = i;
	cout <<range<< "数据量：" << endl << endl;
	cout << "随机排序情况：" << endl;		mySortTime(randomData, range);
	cout << "正序排序情况：" << endl;		mySortTime(positiveData, range);
	cout << "逆序排序情况：" << endl;		mySortTime(ReverseData, range);

}

void zeo()
{
	char words[3] = { '1','0',' ' };
	int randNum1 = rand() % 3;
	cout << words[randNum1];
}

void ttry(int n)
{
	for (int i = 0; i < n; i++)
	{
		zeo();
	}
}

int main()
{
	//system("explorer.exe     C:\\Users\\acooz\\source\\repos\\SortingAlgorithm\\SortingAlgorithm");
	//myTest();
	//allMyShow();

	//auto start = system_clock::now();
	//
	//int* ReverseData = new int[10000000];
	//for (int i = 0; i < 10000000; i++)
	//	ReverseData[10000000 - i - 1] = i;
	//int* Data = initData(10000000);
	//Solution m;
	//m.noSort(Data, 10000000);
	//cout << endl;
	//m.quickSort(Data, 10000000);
	//int len = 10000000;
	//for (int i = 0; i < 100; i++) {
	//	cout << Data[--len] << " ";
	//}

	//auto duration = duration_cast<microseconds>(system_clock::now() - start);
	//cout << "不排序" << "耗时:\t" << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s" << endl;
	while (true){
		ttry(32);
		Sleep(2);
	}
}

	


	

