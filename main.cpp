#include <iostream>
#include "FunctionWrapper.h"
#include "ThreadPool.h"
#include <future>
#include <functional>
#include <vector>
//#include "InterruptFlag.h"
#include <assert.h>
#include <queue>
#include <memory>
#include <future>
#include <iostream>
#include <functional>
#include <queue>
#include <mutex>
#include <thread>
//template <typename T>
//class ThreadSafeQueue{
//public:
//    ThreadSafeQueue(){}
//    ThreadSafeQueue(const ThreadSafeQueue& other) = delete;
//    ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete ;

//    void push(T data){
//        std::lock_guard<std::mutex> lock(theMutex_);
//        theQueue_.push(std::move(data));
//    }

//    bool empty(){
//        std::lock_guard<std::mutex> lock(theMutex_);
//        return theQueue_.empty();
//    }

//    bool tryPop(T& res){
//        std::lock_guard<std::mutex> lock(theMutex_);
//        if(theQueue_.empty()){
//            return false;
//        }
//        res = std::move(theQueue_.front());
//        printf("[tryPop] threadID = %d, address of task = %p\n",std::this_thread::get_id(), res.get());
////        res();
//        std::cout << __PRETTY_FUNCTION__ << "\n";
//        theQueue_.pop();
//        return true;
//    }

//    T pop(){
//        std::cout << __PRETTY_FUNCTION__ << "\n";
//        std::lock_guard<std::mutex> lock(theMutex_);
//        if(theQueue_.empty()){
//            return nullptr;
//        }
//        T res = std::move(theQueue_.front());
//        printf("[tryPop] threadID = %d, address of task = %p\n",std::this_thread::get_id(), res.get());
////        res();

//        theQueue_.pop();
//        return res;
//    }

//    int size(){
//        std::lock_guard<std::mutex> lock(theMutex_);
//        return theQueue_.size();
//    }
//private:
//    std::queue<T> theQueue_;
//    mutable std::mutex theMutex_;
//};


////class joinThreads{
////public:
////    joinThreads(){
////    }
////    explicit joinThreads(std::vector<std::thread>& threads):
////        threads_(threads)
////    { }
////    ~joinThreads()
////    {
////        for(unsigned long i = 0; i < threads_.size(); i++){
////            if(threads_[i].joinable())
////                threads_[i].join();
////        }
////    }
////private:
////    std::vector<std::thread> threads_;
////};

//typedef void (*CommonFunction) ();

//class ThreadPool{
////    WorkStealingQueue poolWorkQueue;
////    typedef std::queue<FunctionWrapper> localQueueType;
////    static thread_local std::unique_ptr<localQueueType> localWorkQueue_;
//public:
//    void workerThread(){
////        while (true) {
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//            std::function<void()> task;
//            std::unique_ptr<std::function<void()>> func;
//            workQueue_->tryPop(func);
//            (*func)();
////            printf("[worker] thread_id: %d, address of workQueue = %p\n", std::this_thread::get_id(), workQueue_.get());
//////            printf("queue size = %d\n", workQueue_.size());
//////            std::cout << "thread_id: " << std::this_thread::get_id() << " workQueue size =" << workQueue_.size() << "\n";
////            if(workQueue_->tryPop(func)){
////                std::cout << __PRETTY_FUNCTION__ << "\n";
//////                std::cout << "thread_id: " << std::this_thread::get_id() << " workQueue size =" << workQueue_.size() << "\n";
////                (*func)();
////                printf("address of task = %p\n", func.get());
////            }
////            else{
////                std::this_thread::sleep_for(std::chrono::seconds(1));
//////                printf("thread_id: %d, tryPop fail\n", std::this_thread::get_id());
////                std::this_thread::yield();
////            }
////        }
//    }
//public:
//    ThreadPool():
//        done_(false)//, joiner(threads_)
//    {
//        workQueue_ = std::make_shared<ThreadSafeQueue<std::unique_ptr<std::function<void()>>>>();
//        unsigned const thread_count = std::thread::hardware_concurrency();
//        std::cout << "thread_count = " << thread_count << "\n";
////        try {
////            for(unsigned i = 0; i < 1; i++){
//////                joiner.threads_.push_back(std::thread(&ThreadPool::workerThread, this));
//////                tThread_ = std::thread(&ThreadPool::workerThread, this);
////                threadStore_.tThread_ = std::thread(&ThreadPool::workerThread, this);
////            }
////        } catch (...) {
////            done_ = true;
////            throw;
////        }
//    }
//    ~ThreadPool()
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
//        printf("[submit] address of func = %p\n", func.get());
//        workQueue_->push(std::move(func));
////        task_ = std::move(func);
//        printf("[submit] threadID = %d, after submit\n", std::this_thread::get_id());
////        printf("[submit] address of workQueue = %p\n", workQueue_.get());

////        workQueue_.push(
////                    [smartFunctionPointer, &args...]() -> void
////                    {
////                        (*smartFunctionPointer)(std::forward<Args>(args)...);
//////                          (*smartFunctionPointer)(std::tuple<Args...>(std::forward<Args>(args)...));
////                        return;
////                    }
////                    );
////        std::packaged_task<result_type()> task(std::move(f));
////        std::future<result_type> res(task.get_future());
////        workQueue_.push(std::move(task));
////        return smartFunctionPointer->get_future();
////        std::unique_ptr<std::function<void()>> f_;
////        workQueue_->tryPop(f_);
////        printf("[submit] address of f_ = %p\n", f_.get());
////        (*f_)();
//    }
//public:
//    std::shared_ptr<ThreadSafeQueue<std::unique_ptr<std::function<void()>>>> workQueue_;
//    std::atomic_bool done_;
////    std::vector<std::thread> threads_;
//};
//using namespace std;
//class ThreadStore{
//public:
//    ThreadStore(){
//        threadPool_ = new ThreadPool();
//        tThread_.push_back(std::thread(&ThreadPool::workerThread, threadPool_));
//        tThread_.push_back(std::thread(&ThreadPool::workerThread, threadPool_));
//    }
//    ThreadStore(ThreadPool* threadPool){
//        threadPool_ = threadPool;
//        tThread_.push_back(std::thread(&ThreadPool::workerThread, threadPool_));
//        tThread_.push_back(std::thread(&ThreadPool::workerThread, threadPool_));
//    }
//    ~ThreadStore(){
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


//    std::vector<std::thread> tThread_;
//    ThreadPool* threadPool_;
//};
void function2(int i, int j){
    std::cout << "hello - function2 " << i << " " << j << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
}
// call back class functor
class RequestCallBack{
public:
    RequestCallBack() = default;
    virtual ~RequestCallBack() = default;
    virtual void operator()(int arg1, int arg2) = 0;
};

class LinkMonitorCallBack : public RequestCallBack{
public:
    LinkMonitorCallBack(){}
    ~LinkMonitorCallBack() override{};
    void operator()(int arg1, int arg2) override{
        std::cout << "LinkMonitorCallBack: arg1 = " << arg1 << "\n";
        std::cout << "LinkMonitorCallBack: arg2 = " << arg2 << "\n";
    }
};


void callBack1(int arg1, int arg2){
    std::cout << "arg1 = " << arg1 << "\n";
    std::cout << "arg2 = " << arg2 << "\n";
}

typedef void (*CALLBACK) (std::unique_ptr<int> arg1, int arg2);
class QDTMessage{
public:
    void request(std::unique_ptr<int> arg1, CALLBACK f){
        int arg2 = 100;
        if(arg1 == nullptr){
            std::cout << "nullptr\n";
        }
//        (*f)(*arg1, arg2);
    }
};

void request(std::unique_ptr<int> arg1, CALLBACK f){
    int arg2 = 100;
    if(arg1 == nullptr){
        std::cout << "nullptr\n";
    }
    (*f)(std::move(arg1), arg2);
}

void callBack5(std::unique_ptr<int> arg1, int arg2){
    if(arg1 == nullptr){
        std::cout << "nullptr callback5\n";
    }
    else{
        std::cout << "arg1 = " << *arg1 << "\n";
    }
     std::cout << "arg2 = " << arg2 << "\n";
}

class Test1{
public:
    Test1(std::shared_ptr<ThreadSafeQueue<std::unique_ptr<std::function<void()>>>> workQueue)
        : workQueue_(workQueue)
    {

    }
    void run(){
        printf("[Test1] threadID = %d, after submit\n", std::this_thread::get_id());
        std::unique_ptr<std::function<void()>> func;
        workQueue_->tryPop(func);
        (*func)();
    }
private:
    std::shared_ptr<ThreadSafeQueue<std::unique_ptr<std::function<void()>>>> workQueue_;
};



int main()
{
    std::vector<FunctionWrapper> funcs;
    QDTMessage msg;
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    a.interrupt();
    funcs.push_back(FunctionWrapper(function2, 1, 10));
    FunctionWrapper f;
    f = FunctionWrapper(FunctionWrapper(function2, 1, 10));
//    f();
//    ThreadPool* threadPool = new ThreadPool();
//    b->display();
//    threadPool.submit(&A::slowRequest2, &a, std::move(b), callBack3);
    std::unique_ptr<int> num(new int(10));
    std::unique_ptr<int> num1(new int(11));
    std::vector<std::thread> threadVec;
//    threadVec.push_back(std::thread(&ThreadPool::workerThread, &threadPool));
//    threadVec.push_back(std::thread(&ThreadPool::workerThread, &threadPool));
    ThreadPool threadStore;
////    threadStore.tThread_ = std::thread(&ThreadPool::workerThread, &threadPool);
    threadStore.submit(request, std::move(num), callBack5);
    threadStore.submit(request, std::move(num1), callBack5);
//    threadPool.workerThread();
//    threadPool.tThread_ = std::thread(&ThreadPool::workerThread, &threadPool);
//    (*threadPool.task_)();

//    std::thread t;
//    threadVec.push_back(std::thread(&ThreadPool::workerThread, &threadPool));
//    t.join();
//    std::unique_ptr<int> num2(new int(21));
//    {
//        std::unique_ptr<int> i = std::move(num2);
//    }
//    if(num2 == nullptr){
//        std::cout << "nullll\n";
//    }
//    Test1 test(threadPool.workQueue_);
//    std::thread tThread(&Test1::run, &test);
//    tThread.join();
//    std::unique_ptr<std::function<void()>> func;
//    threadPool.workQueue_->tryPop(func);
//    (*func)();
//    threadPool.submit(FunctionWrapper(function1, 1));
//    threadPool.submit(FunctionWrapper(function2, 1, 10));
//    threadPool.submit(FunctionWrapper(function3, 1, 10, 100));
//    A* a = new A(10);
//    std::cout << "val = " << a->val << "\n";
//    slowRequest(a, callBack);
//    std::cout << "val = " << a->val << "\n";
////    printf("b = %d\n", *b);
////    slowRequest1(10, callBack1);
////    slowRequest1(10, &A::callBack1, a);
////    slowRequest2(10, a->aCallBack);
////    a->request();
//    slowRequest1(10, callBack3);
//    std::shared_ptr<A> a1 = std::make_shared<A>(10);
//    auto func = FunctionWrapper(&A::slowRequest2, a1, callBack3);
//    func();
//    auto func2 = FunctionWrapper(callBack4, std::make_unique<D>(), 30);
//    func2();
    return 0;
}
