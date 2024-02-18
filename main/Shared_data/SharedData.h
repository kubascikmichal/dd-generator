#ifndef __SHARED_DATA_H__
#define __SHARED_DATA_H__
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "string"
#include <cstring>
#include "pthread.h"

using namespace std;

class SharedData
{
private:
    string ip;
    int fps;
    pthread_mutex_t data_mutex;

public:
    SharedData();
    ~SharedData();

    bool setIp(string p_ip);
    bool setFPS(int p_FPS);

    void mutex_lock();
    void mutex_unlock();

    string getIP();
    int getFPS();
};

#endif