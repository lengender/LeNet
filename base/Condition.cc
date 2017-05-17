#include "Condition.h"
#include<errno.h>

//return true if time out, false otherwise
bool Condition::waitForSeconds(double seconds){
    struct timespec abstime;

    clock_gettime(CLOCK_REALTIME, &abstime);

    const int64_t kNanoSecondsPerSecond = 1e9;
    int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

    abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
    abstime.tv_nsec = static_cast<time_t>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

    MutexLock::UnassignGuard ug(mutex_);
    return ETIMEDOUT == pthread_cond_timewait(&pcond_, mutex_.getPthreadMutex(), &abstime);
}
