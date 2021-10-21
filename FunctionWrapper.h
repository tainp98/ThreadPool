#ifndef FUNCTIONWRAPPER_H
#define FUNCTIONWRAPPER_H
#include <memory>
#include <functional>
class FunctionWrapper{
    struct impl_base{
        virtual void call() = 0;
        virtual ~impl_base(){}
    };
    std::unique_ptr<impl_base> impl;
    template<typename F>
    struct impl_type: impl_base
    {
      F f_;
      impl_type(F&& f) : f_(std::move(f)){}
      void call() { f_(); }
    };
public:
    template<typename F, typename... Args>
    FunctionWrapper(F&& f, Args... args):
        impl(new impl_type(std::bind(std::move(f), args...)))
    { }

    void operator() () {impl->call(); }

    FunctionWrapper() = default;
    FunctionWrapper(FunctionWrapper&& other):
        impl(std::move(other.impl))
    {}

    FunctionWrapper& operator=(FunctionWrapper&& other)
    {
        impl = std::move(other.impl);
        return *this;
    }

    FunctionWrapper(const FunctionWrapper&) = delete;
    FunctionWrapper(FunctionWrapper&) = delete;
    FunctionWrapper& operator=(const FunctionWrapper&) = delete;
};

#endif // FUNCTIONWRAPPER_H
