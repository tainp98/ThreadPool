#include <iostream>
#include "FunctionWrapper.h"
#include "ThreadPool.h"
#include <future>
#include <functional>
#include <vector>
#include "InterruptFlag.h"
#include <assert.h>
using namespace std;

void task(int i){
    std::cout << "task " << i << "\n";
//    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::condition_variable cv;
    std::mutex mux;
    std::unique_lock<std::mutex> lk(mux);
    if(interruptionWait(cv, lk, 5000)){
        std::cout << "receive response\n";
    }
    else{
        std::cout << "time out\n";
    }
}
void function2(int i, int j){
    std::cout << "hello - function2 " << i << " " << j << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
}
void function3(int i, int j, int z){
    std::cout << "hello - function3 " << i << " " << j << " " << z << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

class A{
public:
    A(int x){
        val = x;
    }
    A()
    {
//        tThreads.push_back(InterruptibleThread(&A::request, this));
        tThread = new InterruptibleThread(&A::request, this);
    }
    void request(){
        std::cout << "request " << "\n";
        task(1);
    }
    void interrupt(){
        tThread->interrupt();
    }
    ~A(){
        delete tThread;
    }

    public:
//    std::vector<InterruptibleThread> tThreads;
    int val;
    InterruptibleThread* tThread;
};
typedef void (*callBackFunc) (std::unique_ptr<A> a, A* userPtr);
void slowRequest(A* userPtr, callBackFunc f = nullptr)
{
    if(f == nullptr){
        std::cout << "nullptr\n";
        return ;
    }
    else{
        int x = 20;
        std::unique_ptr<A> a(new A(20));
        (*f)(std::move(a), userPtr);
    }
}

void callBack(std::unique_ptr<A> a, A* userPtr){
    *userPtr = *a.release();
    assert(a.get() == nullptr);
    assert(a == nullptr);
}
int main()
{
    std::vector<FunctionWrapper> funcs;
//    A a;
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    a.interrupt();
//    funcs.push_back(FunctionWrapper(function2, 1, 10));
//    funcs.push_back(FunctionWrapper(function3, 1, 10, 100));
//    ThreadPool threadPool;
//    threadPool.submit(FunctionWrapper(function1, 1));
//    threadPool.submit(FunctionWrapper(function2, 1, 10));
//    threadPool.submit(FunctionWrapper(function3, 1, 10, 100));
    A* a = new A(10);
    std::cout << "val = " << a->val << "\n";
    slowRequest(a, callBack);
    std::cout << "val = " << a->val << "\n";
//    printf("b = %d\n", *b);
    return 0;
}
