using System;

namespace ForeachWithVar
{
    class Program
    {
        static void Main(string[] args)
        {
            //변수를 선언합니다.
            string[] array = { "사과", "배", "포도", "딸기", "바나나" };

            //반복을 수행합니다.
            foreach (var item in array)
            {
                //출력합니다.
                Console.WriteLine(item);
            }
        }
    }
}
