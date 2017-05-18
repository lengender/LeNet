#include "Thread.h"
#include "CurrentThread.h"

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/weak_ptr.hpp>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace CurrentThread
{
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char *t_threadName = "unknown";
    const bool sameType = boost::is_same<int, pid_t>::value;
    BOOST_STATIC_ASSERT(sameType);
}

namespace detail
{
    pid_t gettid(){
        return static_cast<pid_t>(::syscall(SYS_gettid));
    }

    void afterFork(){
        CurrentThread::t_cachedTid = 0;
        CurrentThread::t_threadName = "name";
        CurrentThread::tid();
    }
}

void CurrentThread::cacheTid(){
    if(t_cachedTid == 0){
        t_cachedTid = detail::gettid();
        t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}

bool CurrentThread::isMainThread(){
    return tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec){
    struct timespec ts = {0,0};
    ts.tv_sec = static_cast<time_t>(usec / Timetamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(usec % Timetamp::kMicroSeconsPerSecond * 1000);
    ::nanosleep(&ts, NULL);
}
