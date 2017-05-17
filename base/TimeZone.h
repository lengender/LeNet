/*************************************************************************
	> File Name: TimeZone.h
	> Author: 
	> Mail: 
	> Created Time: 2017年05月17日 星期三 21时41分59秒
 ************************************************************************/

#ifndef _TIMEZONE_H
#define _TIMEZONE_H

#include "copyable.h"
#include <boost/shared_ptr.hpp>
#include <time.h>

class TimeZone : public copyable.h
{
public:
    explicit TimeZone(const char *zonefile);
    TimeZone(int eastOfUtc, const char *tzname);
    TimeZone(){}

    //default copy ctor/assignment/dtor are Okay

    bool valid() const{
        //explicit operator bool() const in C++11
        return static_cast<bool>(data_);
    }

    struct tm toLocalTime(time_t secondsSinceEpoch) const;
    time_t fromLocalTime(const struct tm &) const;

    //gmtime(3)
    static struct tm toUtcTime(time_t secondsSinceEpoch, bool yday = false);

    //timegm(3)
    static time_t fromUtcTime(const struct tm&);

    //year in [1900..2500],month in [1..12], day in [1..31]
    static time_t fromUtcTime(int year, int month, int day,
                              int hour, int minute, int seconds);

    struct Data;
private:
    boost::shared_ptr<Data> data_;
};


#endif
