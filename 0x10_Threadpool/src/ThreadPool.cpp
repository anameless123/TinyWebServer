#include "header/ThreadPool.h"

ThreadPool::ThreadPool(int size) : stop(false) {
    for (int i = 0; i < size; ++i) {
        threads.emplace_back(std::thread([this]() {
            //不断取任务->执行任务
            while (true) {
                std::function<void()> task;
                {
                    // 在此作用域加锁，出作用域自动解锁
                    std::unique_lock<std::mutex> lock(tasks_mtx);
                    // 等待条件变量，条件为：
                    // 任务队列为空 且 线程池未关闭 时等待
                    // 否则不用等待，正常运行
                    // 思想：任务队列为空时CPU不应该不断轮询耗费CPU资源
                    cv.wait(lock, [this]() {
                        return stop || !tasks.empty();
                    });

                    // 无任务或停止时返回
                    if (stop && tasks.empty())
                        return;
                    // 取一个任务
                    task = tasks.front();
                    tasks.pop();
                }
                // 执行该任务
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        stop = true;
    }
    cv.notify_all();
    for(std::thread &th:threads){
        if(th.joinable())
            th.join();
    }
}

void ThreadPool::add(std::function<void()> func){
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        if(stop)
            throw std::runtime_error("Failed to add task, ThreadPool already stop");
        tasks.emplace(func);
    }
    cv.notify_one();
}