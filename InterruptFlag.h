#ifndef INTERRUPTFLAG_H
#define INTERRUPTFLAG_H
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <future>
#include <iostream>
#include "FunctionWrapper.h"
class InterruptFlag{
public:
    InterruptFlag():
        threadCond_(0)
    {}

    void set()
    {
        flag_.store(true, std::memory_order_relaxed);
        std::lock_guard<std::mutex> lk(setClearMutex_);
        if(threadCond_)
        {
            threadCond_->notify_all();
        }
    }

    bool isSet() const
    {
        return flag_.load(std::memory_order_relaxed);
    }

    void setConditionVariable(std::condition_variable& cv)
    {
        std::lock_guard<std::mutex> lk(setClearMutex_);
        threadCond_ = &cv;
    }

    void clearConditionVariable()
    {
        std::lock_guard<std::mutex> lk(setClearMutex_);
        threadCond_ = 0;
    }

//    struct ClearCVOnDestruct
//    {
//        ~ClearCVOnDestruct()
//        {
//            this_thread_interrupt_flag.clearConditionVariable();
//        }
//    };

private:
    std::atomic_bool flag_;
    std::condition_variable* threadCond_;
    std::mutex setClearMutex_;
};

thread_local InterruptFlag this_thread_interrupt_flag;

class InterruptibleThread{
public:
    template<typename FunctionType, typename... Args>
    InterruptibleThread(FunctionType f, Args... args)
    {
        std::cout << "stop here\n";
        std::promise<InterruptFlag*> p;
        internalThread_ = std::thread([f1 = std::bind(f, args...), &p] {
            std::cout << "stop here\n";
            p.set_value(&this_thread_interrupt_flag);
            f1();
        });
        flag_ = p.get_future().get();
    }
    ~InterruptibleThread(){
        if(internalThread_.joinable()){
            internalThread_.join();
        }
    }

    void interrupt()
    {
        if(flag_)
        {
            std::cout << "flag had set\n";
            flag_->set();
        }
        else{
            std::cout << "flag not set\n";
        }
    }

private:
    std::thread internalThread_;
    InterruptFlag* flag_;
};

bool interruptionPoint()
{
    if(this_thread_interrupt_flag.isSet())
    {
        std::cout << "interrupt had set\n";
        return true;
    }
    else{
        std::cout << "interrupt had not set\n";
        return false;
    }
}

bool interruptionWait(std::condition_variable& cv, std::unique_lock<std::mutex>& lk, double timeMS = 1000)
{
    interruptionPoint();
    this_thread_interrupt_flag.setConditionVariable(cv);
    interruptionPoint();
    auto start = std::chrono::system_clock::now();
    cv.wait_for(lk, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(timeMS/1000)));
    auto stop = std::chrono::system_clock::now();
    std::cout << "time wait = " << std::chrono::duration_cast<std::chrono::seconds>(stop-start).count() << " seconds \n";
    return interruptionPoint();
}
#endif // INTERRUPTFLAG_H
