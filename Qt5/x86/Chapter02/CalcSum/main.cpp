#include <iostream>

extern "C" int CalcSum_(int a, int b, int c);

using namespace std;

int main(void)
{
    int a = 10, b = 20, c = 30;
    int sum = CalcSum_(a,b,c);
    std::cout << a << " + " << b << " + " << c <<" = " << sum << std::endl;
    return 0;
}
