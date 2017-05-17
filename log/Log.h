/*************************************************************************
	> File Name: Log.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月05日 星期三 18时59分53秒
 ************************************************************************/

#ifndef _LOG_H
#define _LOG_H

#include<memory>
#include<ctime>
#include<iostream>
#include<fstream>

namespace LeNet{
    class Log{
    public:
        static Log *Inst(){
            if(0 == _instance.get()){
                _instance.reset(new Log);
            }

            return _instance.get();
        }
    
        void log(std::string msg);  //写日志的方法
    private:
        Log(){};
        virtual ~Log(){}
        friend class std::auto_ptr<Log>;
        static std::auto_ptr<Log> _instance;
    };
    
    std::auto_ptr<Log> Log::_instance;

    void Log::log(std::string msg){
        std::ofstream ofs;
        time_t t = time(0);
        char tmp[64];

        strftime(tmp, sizeof(tmp), "\t[%Y.%m.%d %X %A]", localtime(&t));
        ofs.open("LeNet.log", std::ofstream::app);
        ofs.write(msg.c_str(), msg.size());
        ofs << tmp << '\n';
        ofs.close();
    }
}
#endif
