/*************************************************************************
	> File Name: Condition.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 11时13分06秒
 ************************************************************************/

#ifndef _CONDITION_H
#define _CONDITION_H

#include"Mutex.h"
#include"Log.h"
#include<boost/noncopyable.hpp>


namespace LeNet{
    class Condition : boost::noncopyable{
    public:
        explicit Condition(Mutex &mutex)
            :mutex(mutex)
        {
            if(pthread_cond_init(&m_cond, NULL) < 0){
                Log::Inst()->log("init conditon error.");
            }
        }

        ~Condition(){
            if(pthread_cond_destroy(&m_cond) < 0){
                Log::Inst()->log("destroy condition error.");
            }
        }

        void wait(){
            if(pthread_cond_wait(&m_cond, mutex.getPthreadMutex()) < 0){
                Log::Inst()->log("wait condition error.");
            }
        }

        void notify(){
            if(pthread_cond_signal(&m_cond)){
                Log::Inst()->log("notify conditon error.");
            }
        }

        void notifyAll(){
            if(pthread_cond_broadcast(&m_cond)){
                Log::Inst()->log("notifyall condition error.");
            }
        }
    
    private:
        Mutex &mutex;
        pthread_cond_t m_cond;
    };
}
#endif
