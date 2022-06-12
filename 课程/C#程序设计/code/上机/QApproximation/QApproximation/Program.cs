using System;

namespace QApproximation
{
    //创建控制台应用程序，利用下面的公式计算q的近似值，并运行出k=10时q的值。
    static class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("please input k value");
            var k = int.Parse(Console.ReadLine()!);
            Console.WriteLine("Q Approximation is :{0}", Calculate(k: k));
        }

        public static double Calculate(int k)
        {
            double approximation = 0;
            for (var i = 0; i <= k; i++)
                approximation += Fun(i);

            return approximation;
        }

        private static double Fun(int n)
        {
            return ((1.0 / Factorial(n)) + n * n);
        }

        private static int Factorial(int n)
        {
            var result = 1;
            for (var i = n; i > 0; i--)
                result *= i;
            return result;
        }
    }
}