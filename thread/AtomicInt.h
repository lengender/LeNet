/*************************************************************************
	> File Name: AtomicInt.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月10日 星期一 10时45分56秒
 ************************************************************************/

#ifndef _ATOMICINT_H
#define _ATOMICINT_H

namespace LeNet{
    template<typename T>
    class AtomicInt : boost::noncopyable{
    public:
        AtomicInt(T x = 0){
            set(x);
        }

        T get(){
            return __sync_val_compare_and_swap(&value, 0, 0);
        }

        void set(T newValue){
            getAndSet(newValue);
        }

        T getAndSet(T x){
            return __sync_fetch_and_set(&value, x);
        }

        T getAndAdd(T x){
            return __sync_fetch_and_add(&value, x);
        }

        T andAndGet(T x){
            return __sync_add_and_fetch(&value, x);
        }

        T incAndGet(){
            return addAndGet(1);
        }

        T decAndGet(){
            return addAndGet(-1);
        }

        T getAndDec(){
            return getAndAdd(-1);
        }

        T getAndInc(){
            return addAndGet(1);
        }

        void add(T x){
            getAndAdd(x);
        }

        void inc(){
            getAndInc();
        }

        void dec(){
            getAndDec();
        }
    private:
        volatile T value;
    };

    typedef AtoMicInt<int8_t> AtomicInt8;
    typedef AtoMicInt<int16_t> AtomicInt16;
    typedef AtomicInt<int32_t> AtomicInt32;
    typedef AtomicInt<int64_t> AtomicInt64;
    }
#endif
