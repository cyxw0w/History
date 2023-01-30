using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ex_1
{
    class Program
    {
        static void Main(string[] args)
        {
            string input = Console.ReadLine();

            if (input.Contains("바이"))
                Console.Write("잘가..!!\n");
            else
                Console.Write("^^\n");

        }
    }
}
