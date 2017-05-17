/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月05日 星期三 19时23分54秒
 ************************************************************************/

#include<iostream>
#include"Log.h"
#include"Mutex.h"

using namespace std;
using namespace LeNet;

int main(){
    Mutex mutex;
    MutexGuard mg(mutex);
    Log::Inst()->log("Run...");
}

