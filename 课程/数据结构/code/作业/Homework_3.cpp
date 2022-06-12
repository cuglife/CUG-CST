#include <iostream>
#include <cmath>
using namespace std;

//斐波那契数列的递归算法，复杂度为O(2^n)
long Fibonacci(long& times,long n) {
	++times;
	if (n <= 1) 
		return n;
	else {
		return Fibonacci(times, n - 1) + Fibonacci(times, n - 2);
	}
}

//使用用斐波那契数列的通项公式，复杂度为O(1)
double FibonacciOptimization(double n){
	return (pow((1 + sqrt(5.0)) / 2, n) - pow((1 - sqrt(5.0)) / 2, n)) / sqrt(5.0);
}


int main()
{
		while (1) {
		long n, times = 0;
		cout << "Please input a number" << endl;
		cin >> n;
		cout << "Fibonacci result: "<<Fibonacci(times, n) << endl;
		cout <<"Times: "<< times << endl << endl;

		cout << "FibonacciOptimization result: " << FibonacciOptimization(n) << endl;
	}
}
