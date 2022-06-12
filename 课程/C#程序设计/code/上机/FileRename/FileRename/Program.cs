using System;

namespace FileRename
{
    static class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("input file name:");
            //string filename = @"hello world.txt";
            var filename = Console.ReadLine();
            Console.WriteLine("changed file name: \n" + ChangeFileName(filename));
        }

        public static string ChangeFileName(string fileName)
        {
            fileName = fileName.Trim();
            var indexDot = fileName.LastIndexOf('.');
            const string extendName = "dat";
            fileName = fileName.Substring(0, indexDot + 1);
            fileName += extendName;

            var splitFileName = fileName.Split(" ");
            var joinString = string.Join("_", splitFileName);

            return joinString;
        }
    }
}