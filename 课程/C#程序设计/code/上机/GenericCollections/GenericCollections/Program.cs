using System;
using System.Collections;
using System.Collections.Generic;

namespace GenericCollections
{
    //编程实现List、Queue、Stack、Dictionary这四个泛型集合中添加、删除、遍历输出元素的操作
    internal static class Program
    {
        public static void Main(string[] args)
        {
            ListTest();
            QueueTest();
            StackTest();
            DictionaryTest();
        }

        private static void ListTest()
        {
            Console.WriteLine();
            Console.WriteLine("ArrayList: ");
            var myList = new ArrayList();
            for (var i = 0; i < 10; i++)
            {
                myList.Add(i);
            }

            Console.WriteLine("\nArrayList 初始化后输出: ");
            foreach (var item in myList)
            {
                Console.Write(item);
            }

            myList.Remove(2);
            myList.Insert(4, "X");
            Console.WriteLine("\nArrayList 添加和删除后输出:");
            foreach (var item in myList)
            {
                Console.Write(item);
            }
        }

        private static void QueueTest()
        {
            Console.WriteLine();
            Console.WriteLine("Queue: ");

            Queue myQueue = new Queue();
            for (var i = 0; i < 10; i++)
            {
                myQueue.Enqueue(i);
            }

            Console.WriteLine("\nQueue 初始化后输出");
            foreach (var item in myQueue)
            {
                Console.Write(item);
            }

            // 出队列
            myQueue.Dequeue();

            Console.WriteLine("\n出队列后输出");
            foreach (var item in myQueue)
            {
                Console.Write(item);
            }
        }

        private static void StackTest()
        {
            Console.WriteLine();
            Console.WriteLine("\n\n\n Stack: ");
            var myStack = new Stack();
            for (var i = 0; i < 10; i++)
            {
                myStack.Push(i);
            }

            myStack.Pop();
            Console.WriteLine("\nStack 添加 0-9, Pop栈顶后输出 ");
            foreach (var item in myStack)
            {
                Console.Write(item);
            }
        }

        private static void DictionaryTest()
        {
            Console.WriteLine();
            var myDictionary = new Dictionary<int, string>();

            myDictionary.Add(1, "One");
            myDictionary.Add(2, "Two");
            myDictionary.Add(3, "Three");

            Console.WriteLine("\nDictionary 初始化后输出: ");
            foreach (var item in myDictionary)
            {
                Console.Write(item);
            }

            myDictionary.Remove(2);

            Console.WriteLine("\nDictionary 删除后输出: ");
            foreach (var item in myDictionary)
            {
                Console.Write(item);
            }
        }
    }
}