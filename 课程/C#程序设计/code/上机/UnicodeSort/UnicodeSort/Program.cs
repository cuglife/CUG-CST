using System;
using static System.String;

namespace UnicodeSort
{
    internal class Program
    {
        public static void Main(string[] args)
        {
            Chair[] arrayOfChairs= new Chair[6]
            {
                new Chair(111,"Apple","x1234"),
                new Chair(222,"Xiaomi","pro2"),
                new Chair(333,"Huawei","ultra33"),
                new Chair(444,"Samsung","edge4"),
                new Chair(555,"Oppo","Max5"),
                new Chair(666,"Vivo","Plus66")
            };
            Console.WriteLine("\nArray of Chairs \n");
            foreach(var c in arrayOfChairs)
                Console.WriteLine(c.myID + "\t\t" + c.myPrice);

            
            Array.Sort(arrayOfChairs);
            Console.WriteLine("\nArray - Sorted by myID\n");
            foreach(var c in arrayOfChairs)
                Console.WriteLine(c.myID + "\t\t" + c.myPrice);
        }
    }
    public class Chair: IComparable
    {
        public double myPrice;
        public string myVendor;
        public string myID;
        public Chair() { }
        public Chair(double price, string vendor, string sku)
        {
            myPrice = price;
            myVendor = vendor;
            myID = sku;
        }
        
        int IComparable.CompareTo(object obj)
        {
            var c=(Chair)obj;
            return CompareOrdinal(this.myID,c.myID);
        }
    }

}