/*************************************************************************
	> File Name: Mutex.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 11时37分26秒
 ************************************************************************/

#ifndef _MUTEX_H
#define _MUTEX_H

#include<boost/noncopyable.hpp>
#include<pthread.h>
#include"../log/Log.h"

namespace LeNet{
    class Mutex: boost::noncopyable{
    public:
        Mutex(){
            if(pthread_mutex_init(&m_mutex, NULL) < 0){
                Log::Inst()->log("init mutex error.");
            }
        }

        ~Mutex(){
            if(pthread_mutex_destroy(&m_mutex) < 0){
                Log::Inst()->log("destroy mutex error.");
            }
        }

        void lock(){
            if(pthread_mutex_lock(&m_mutex) < 0){
                Log::Inst()->log("lock mutex error.");
            }
        }

        void unlock(){
            if(pthread_mutex_unlock(&m_mutex) < 0){
                Log::Inst()->log("unlock mutex error.");
            }
        }

        pthread_mutex_t * getPthreadMutex(){
            return &m_mutex;
        }

    private:
        pthread_mutex_t m_mutex;
    };
}
#endif
