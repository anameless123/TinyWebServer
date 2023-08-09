#include <iostream>
#include <string>

using namespace std;

char *rrr()
{
    char *s = "aiofaj";
    return s;
}
int main() {
    char *cstr = rrr();
    char *saiofj = rrr();
    cout << cstr << endl;
}