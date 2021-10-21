#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <WorkStealingQueue.h>
#include "ThreadSafeQueue.h"
#include <queue>
#include <memory>
#include <future>
#include <iostream>
class joinThreads{
public:
    explicit joinThreads(std::vector<std::thread>& threads):
        threads_(threads)
    { }
    ~joinThreads()
    {
        for(unsigned long i = 0; i < threads_.size(); i++){
            if(threads_[i].joinable())
                threads_[i].join();
        }
    }
private:
    std::vector<std::thread>& threads_;
};

class ThreadPool{
//    WorkStealingQueue poolWorkQueue;
//    typedef std::queue<FunctionWrapper> localQueueType;
//    static thread_local std::unique_ptr<localQueueType> localWorkQueue_;
private:
    void workerThread(){
        while (true) {
            FunctionWrapper task;
            if(workQueue_.tryPop(task)){
                std::cout << "thread_id: " << std::this_thread::get_id() << " workQueue size =" << workQueue_.size() << "\n";
                task();
            }
            else{
                printf("thread_id: %d, tryPop fail\n", std::this_thread::get_id());
                std::this_thread::yield();
            }
        }
    }
public:
    ThreadPool():
        done_(false), joiner(threads_)
    {
        unsigned const thread_count = std::thread::hardware_concurrency();
        std::cout << "thread_count = " << thread_count << "\n";
        try {
            for(unsigned i = 0; i < 3; i++){
                threads_.push_back(std::thread(&ThreadPool::workerThread, this));
            }
        } catch (...) {
            done_ = true;
            throw;
        }
    }
    ~ThreadPool()
    {
        done_ = true;
    }
    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type>
    submit(FunctionType f){
        typedef typename std::result_of<FunctionType()>::type result_type;
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        workQueue_.push(std::move(task));
        return res;
    }
private:
    ThreadSafeQueue<FunctionWrapper> workQueue_;
    std::atomic_bool done_;
    std::vector<std::thread> threads_;
    joinThreads joiner;
};

#endif // THREADPOOL_H
