using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ex_2
{
    class Program
    {
        static void Main(string[] args)
        {           
            ConsoleKeyInfo cki;

                cki = Console.ReadKey(false);     // 입력한 문자가 화면에 표시된다.
                switch (cki.Key)
                {
                    case ConsoleKey.Home:
                        Console.WriteLine("홈키 입력");
                        break;

                    case ConsoleKey.End:
                        Console.WriteLine("엔드키 입력");
                        break;

                    case ConsoleKey.PageUp:
                        Console.WriteLine("PgUp키 입력");
                        break;

                    case ConsoleKey.PageDown:
                        Console.WriteLine("PgDn키 입력");
                        break;

                    default:
                    Console.WriteLine("다른 키를 눌렀습니다.");
                    return;
                }
            }
            
    }
}
