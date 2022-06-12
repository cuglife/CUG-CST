#include <iostream>
#include "bankersAlgorithm.h"
#include "dynamicPartition.h"
#include "multipleFeedback.h"

using namespace std;



int main()
{
	cout << "请选择要执行的程序" << endl;
	cout << "1. 多级队列调度算法	 " << endl;
	cout << "2. 银行家算法	 " << endl;
	cout << "3. 动态分区式存贮区管理	 " << endl;
	int which;
	cin >> which;

	if (which == 1)
		multipleFeedback();
	else if (which == 2)
		bankersAlgorithm();
	else if (which == 3)
		dynamicPartition();
	else
		cout << "输入错误，请重新选择" << endl;

}

