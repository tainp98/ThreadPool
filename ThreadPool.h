#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <WorkStealingQueue.h>
#include "ThreadSafeQueue.h"
#include <queue>
#include <memory>
#include <future>
#include <iostream>
#include <functional>
//class joinThreads{
//public:
//    explicit joinThreads(std::vector<std::thread>& threads):
//        threads_(threads)
//    { }
//    ~joinThreads()
//    {
//        for(unsigned long i = 0; i < threads_.size(); i++){
//            if(threads_[i].joinable())
//                threads_[i].join();
//        }
//    }
//private:
//    std::vector<std::thread>& threads_;
//};

class TaskPool{
public:
    TaskPool()
        : done_(false)
    {
        workQueue_ = std::make_shared<ThreadSafeQueue<std::unique_ptr<std::function<void()>>>>();
    }
    ~TaskPool()
    {
        done_ = true;
    }
    template<typename Function, typename... Args>
//    std::future<typename std::invoke_result_t<Function, Args...>::type>
    void submit(Function&& f, Args&&... args){
        using ReturnType = typename std::invoke_result_t<Function, Args...>;
        using FuncType = ReturnType(Args...);
        using Wrapped = std::packaged_task<FuncType>;
        std::shared_ptr<Wrapped> smartFunctionPointer = std::make_shared<Wrapped>(std::move(f));
        std::unique_ptr<std::function<void()>> func = std::make_unique<std::function<void()>>([smartFunctionPointer, &args...]() -> void
        {
            (*smartFunctionPointer)(std::forward<Args>(args)...);
//                          (*smartFunctionPointer)(std::tuple<Args...>(std::forward<Args>(args)...));
            return;
        });
//        printf("[submit] address of func = %p\n", func.get());
        workQueue_->push(std::move(func));
//        workQueue_->push(
//                    [smartFunctionPointer, &args...]() -> void
//                    {
////                        (*smartFunctionPointer)(std::forward<Args>(args)...);
//                          (*smartFunctionPointer)(std::forward<Args>(args)...);
//                        return;
//                    }
//        );
    }
private:
    void doTask()
    {
//        while(!done_)
//        {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printf("queue size = %d\n", workQueue_->size());
            std::unique_ptr<std::function<void()>> task;
            if(workQueue_->tryPop(task))
            {
                (*task)();
            }
            else
            {
                printf("dont have task to do\n");
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::this_thread::yield();
            }
//        }
    }
private:
    std::shared_ptr<ThreadSafeQueue<std::unique_ptr<std::function<void()>>>> workQueue_;
    std::atomic_bool done_;
    friend class ThreadPool;
};

class ThreadPool{
//    WorkStealingQueue poolWorkQueue;
//    typedef std::queue<FunctionWrapper> localQueueType;
//    static thread_local std::unique_ptr<localQueueType> localWorkQueue_;
public:
    ThreadPool()
    {
        taskPool_ = new TaskPool();
        for(int i = 0; i < 3; i++){
            threads_.push_back(std::thread(&TaskPool::doTask, taskPool_));
        }
    }
    ~ThreadPool()
    {
        for(int i = 0; i < threads_.size(); i++){
            if(threads_[i].joinable())
                threads_[i].join();
        }
    }
    template<typename Function, typename... Args>
    void submit(Function&& f, Args&&... args){
        taskPool_->submit(f, std::forward<Args>(args)...);
    }
public:
    std::vector<std::thread> threads_;
    TaskPool* taskPool_ = nullptr;
};

// ============================================================================
//class TaskPool{
////    WorkStealingQueue poolWorkQueue;
////    typedef std::queue<FunctionWrapper> localQueueType;
////    static thread_local std::unique_ptr<localQueueType> localWorkQueue_;
//private:
//    void workerThread(){
//        while (!done_) {
//            std::this_thread::sleep_for(std::chrono::seconds(1));
//            std::function<void()> task;
//            std::unique_ptr<std::function<void()>> func;
//            if(workQueue_->tryPop(func))
//                (*func)();
//            else{
//                std::cout << "dont have task to do\n";
//                std::this_thread::yield();
//            }
//        }
//    }
//public:
//    TaskPool():
//        done_(false)//, joiner(threads_)
//    {
//        workQueue_ = std::make_shared<ThreadSafeQueue<std::unique_ptr<std::function<void()>>>>();
//        unsigned const thread_count = std::thread::hardware_concurrency();
//        std::cout << "thread_count = " << thread_count << "\n";
//    }
//    ~TaskPool()
//    {
//        done_ = true;
//    }
//    template<typename Function, typename... Args>
////    std::future<typename std::invoke_result_t<Function, Args...>::type>
//    void submit(Function&& f, Args&&... args){
//        using ReturnType = typename std::invoke_result_t<Function, Args...>;
//        using FuncType = ReturnType(Args...);
//        using Wrapped = std::packaged_task<FuncType>;
//        std::cout << __PRETTY_FUNCTION__ << "\n";
//        std::shared_ptr<Wrapped> smartFunctionPointer = std::make_shared<Wrapped>(std::move(f));
//        std::unique_ptr<std::function<void()>> func = std::make_unique<std::function<void()>>([smartFunctionPointer, &args...]() -> void
//        {
//            (*smartFunctionPointer)(std::forward<Args>(args)...);
////                          (*smartFunctionPointer)(std::tuple<Args...>(std::forward<Args>(args)...));
//            return;
//        });
////        printf("[submit] address of func = %p\n", func.get());
//        workQueue_->push(std::move(func));
////        printf("[submit] threadID = %d, after submit\n", std::this_thread::get_id());
//    }
//public:
//    std::shared_ptr<ThreadSafeQueue<std::unique_ptr<std::function<void()>>>> workQueue_;
//    std::atomic_bool done_;
////    std::vector<std::thread> threads_;
//    friend class ThreadPool;
//};
//using namespace std;
//class ThreadPool{
//public:
//    ThreadPool(){
//        threadPool_ = new TaskPool();
//        for(int i = 0; i < 3; i++)
//            tThread_.push_back(std::thread(&TaskPool::workerThread, threadPool_));
//    }
//    ThreadPool(TaskPool* threadPool){
//        threadPool_ = threadPool;
//        tThread_.push_back(std::thread(&TaskPool::workerThread, threadPool_));
//        tThread_.push_back(std::thread(&TaskPool::workerThread, threadPool_));
//    }
//    ~ThreadPool(){
//        for(int i = 0; i < tThread_.size(); i++){
//            if(tThread_[i].joinable())
//                tThread_[i].join();
//        }
//    }
//    template<typename Function, typename... Args>
////    std::future<typename std::invoke_result_t<Function, Args...>::type>
//    void submit(Function&& f, Args&&... args){
//        threadPool_->submit(f, std::forward<Args>(args)...);
//    }

//private:
//    std::vector<std::thread> tThread_;
//    TaskPool* threadPool_;
//};
#endif // THREADPOOL_H
