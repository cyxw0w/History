using System;

namespace ex_3
{
    class Program
    {
        static void Main(string[] args)
        {
            char star = '*';
            char space = ' ';
            int a = 10;
            int b = 1;
            for (int j = 0; j < 10; j++, a--, b += 2)
            {
                for (int i = 0; i < a; i++)
                {
                    Console.Write(space);
                }
                for (int k = 0; k < b; k++)
                {
                    Console.Write(star);
                }
                Console.WriteLine();
            }
        }
    }
}
