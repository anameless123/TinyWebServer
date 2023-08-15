#include "src/header/ThreadPool.h"
#include <iostream>
#include <string>

void print(int a, double b, const char *c, std::string d) {
    std::cout << a << b << c << d << std::endl;
}

void test() {
    std::cout << "help" << std::endl;
}

int main(int argc, char const *argv) {
    ThreadPool *pool = new ThreadPool;
    std::function<void()> func = std::bind(print, 1, 3.14, "hello", std::string("world"));
    pool->add(func);
    func = test;
    pool->add(func);
    delete pool;
    return 0;
}