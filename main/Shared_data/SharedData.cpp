#include "SharedData.h"

SharedData::SharedData()
{
    pthread_mutex_init(&this->data_mutex, NULL);
}

SharedData::~SharedData()
{
    pthread_mutex_destroy(&(this->data_mutex));
}

bool SharedData::setIp(string p_ip)
{
    return true;
}
bool SharedData::setFPS(int p_FPS)
{
    this->fps = p_FPS;
    return true;
}

string SharedData::getIP()
{
    return "";
}
int SharedData::getFPS()
{
    return this->fps;
}

void SharedData::mutex_lock()
{
    pthread_mutex_lock(&this->data_mutex);
}
void SharedData::mutex_unlock()
{
    pthread_mutex_unlock(&this->data_mutex);
}