#include <windows.h>
//GetStdHandle 和 SetConsoleTextAttribute 在头文件 windows.h 中
#include <iostream>
using namespace std;
void SetColor(unsigned short ForeColor = 3, unsigned short BackGroundColor = 0)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE); //本例以输出为例
    SetConsoleTextAttribute(hCon, ForeColor | BackGroundColor);
}
int main(void)
{
    SetColor();
    std::cout << "Hello world!" << endl;
    SetColor(40, 30);
    std::cout << "Hello world!" << endl;
    std::cout << "Hello world!" << endl;
    return 0;
}
void coordinate(int x, int y)
{
    COORD c; //定义表示一个字符在控制台屏幕上的坐标的对象
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    /*定位光标位置的函数，坐标为GetStdHandle（）返回标准的输出的句柄，也就是获得输出屏幕缓冲区的句柄，并赋值给对象c*/
}