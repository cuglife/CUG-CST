using System;

namespace HandleException
{
    class Test
    {
        static void Main(string[] args)
        {
            //当执行以下代码时，会有异常发生，试设计自定义异常，对抛出的异常进行处理：
            Circle myCir = new Circle();
            ObjShowMe.ShowMe(myCir);
        }
    }

    [Serializable()]
    public class CustomException : System.Exception
    {
        public CustomException() : base()
        {
        }

        public CustomException(string message) : base(message)
        {
        }

        public CustomException(string message, System.Exception inner) : base(message, inner)
        {
        }

        // A constructor is needed for serialization when an
        // exception propagates from a remoting server to the client.
        protected CustomException(System.Runtime.Serialization.SerializationInfo info,
            System.Runtime.Serialization.StreamingContext context) : base(info, context)
        {
        }
    }

    public interface IFun1
    {
        string ShowMe();
    }

    public interface IFun2
    {
        string ShowMe();
    }

    class Circle : IFun1
    {
        public string ShowMe()
        {
            return "Circle-IFun1";
        }
    }

    public class ObjShowMe
    {
        public static void ShowMe(object obj)
        {
            try
            {
                if (!(obj is IFun1 && obj is IFun2))
                {
                    //抛出异常
                    throw new CustomException("自定义异常发生");
                }
            }
            catch (CustomException e)
            {
                Console.WriteLine(e.Message);
                throw;
            }
            finally
            {
                // something done
                Console.WriteLine("finally 处理");
            }
        }
    }
}