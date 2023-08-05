#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include <iostream>
using namespace std;

int main()
{
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> b(begin(a), end(a));
    for (auto i : b)
        cout << i << endl;
    return 0;
}