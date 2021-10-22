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

void slowRequest2(int timeout, RequestCallBack* callBack){
    int arg1 = 10, arg2 = 22;
//    FunctionWrapper func;
    if(callBack != nullptr){
            (*callBack)(arg1, arg2);
    }
}

class Base{
public:
    Base(){}
    virtual ~Base(){}
    virtual void display() = 0;
};

class D : public Base{
public:
    D(){}
    virtual ~D() override{}
    void display() override{
        std::cout << "D display\n";
    }
};

typedef void (*ACallBack) (std::unique_ptr<Base> arg1, int arg2);
class A{
public:
    A(int x){
        val = x;
        aCallBack = new LinkMonitorCallBack();
    }
    A()
    {
//        tThreads.push_back(InterruptibleThread(&A::request, this));
        tThread = new InterruptibleThread(&A::request, this);
    }
    void slowRequest2(ACallBack acallBack){

        int arg1 = 10, arg2 = 22;
    //    FunctionWrapper func;
        if(acallBack != nullptr){
                (*acallBack)(std::make_unique<D>(), arg2);
        }
    }
    void request(){
        std::cout << "request " << "\n";
//        slowRequest2(10, aCallBack);
    }
    void interrupt(){
        tThread->interrupt();
    }

    void callBack1(int arg1, int arg2){
        std::cout << "A : arg1 = " << arg1 << "\n";
        std::cout << "A : arg2 = " << arg2 << "\n";
    }
    ~A(){
        delete tThread;
    }

    public:
//    std::vector<InterruptibleThread> tThreads;
    int val;
    InterruptibleThread* tThread;
    RequestCallBack* aCallBack = nullptr;
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

template<typename F, typename... P>
void slowRequest1(int timeout, F f, P... p){
    int arg1 = 10, arg2 = 22;
//    FunctionWrapper func;
    if(f != nullptr){
//            FunctionWrapper func(f,p...,arg1,arg2);
//            func();
            FunctionWrapper func1(f,p...,arg1);
            func1();
    }
}

void callBack1(int arg1, int arg2){
    std::cout << "arg1 = " << arg1 << "\n";
    std::cout << "arg2 = " << arg2 << "\n";
}

void callBack3(std::unique_ptr<Base> arg1, int arg2){
     arg1->display();
     std::cout << "arg2 = " << arg2 << "\n";
}

void callBack4(std::unique_ptr<D> arg1, int arg2){
     arg1->display();
     std::cout << "arg2 = " << arg2 << "\n";
}



int main()
{
    std::vector<FunctionWrapper> funcs;
//    A a;
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    a.interrupt();
    funcs.push_back(FunctionWrapper(function2, 1, 10));
    funcs.push_back(FunctionWrapper(function3, 1, 10, 100));
    FunctionWrapper f;
    f = FunctionWrapper(FunctionWrapper(function2, 1, 10));
//    f();
//    ThreadPool threadPool;
////    threadPool.submit(FunctionWrapper(function1, 1));
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
    std::shared_ptr<A> a1 = std::make_shared<A>(10);
    auto func = FunctionWrapper(&A::slowRequest2, a1, callBack3);
    func();
    auto func2 = FunctionWrapper(callBack4, std::make_unique<D>(), 30);
    func2();
    return 0;
}
