#ifndef WORKSTEALINGQUEUE_H
#define WORKSTEALINGQUEUE_H
#include "FunctionWrapper.h"
#include <deque>
#include <mutex>
#include <thread>
class WorkStealingQueue{
typedef FunctionWrapper data_type;
public:
    WorkStealingQueue(){}
    WorkStealingQueue(const WorkStealingQueue& other) = delete;
    WorkStealingQueue& operator=(const WorkStealingQueue& other) = delete ;

    void push(data_type data){
        std::lock_guard<std::mutex> lock(theMutex_);
        theQueue_.push_front(std::move(data));
    }

    bool empty(){
        std::lock_guard<std::mutex> lock(theMutex_);
        return theQueue_.empty();
    }

    bool tryPop(data_type& res){
        std::lock_guard<std::mutex> lock(theMutex_);
        if(theQueue_.empty()){
            return false;
        }
        res = std::move(theQueue_.front());
        theQueue_.pop_front();
        return true;
    }

    bool trySteal(data_type& res){
        std::lock_guard<std::mutex> lock(theMutex_);
        if(theQueue_.empty()){
            return false;
        }
        res = std::move(theQueue_.back());
        theQueue_.pop_back();
        return true;
    }
private:
    std::deque<data_type> theQueue_;
    mutable std::mutex theMutex_;
};

#endif // WORKSTEALINGQUEUE_H
