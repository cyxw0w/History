using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace ex_4
{
    class Program
    {
        static void Main(string[] args)
        {                        
            int x = 40;
            for (; ; )
            {
                Console.Clear();
                Console.SetCursorPosition(x, 12);
                Console.Write("_^@");
                Thread.Sleep(1000);
                x++;
            }
        }
    }
}
