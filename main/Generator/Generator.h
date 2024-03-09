#ifndef __GENERATOR_H__
#define __GENERATOR_H__
#include <stdint.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_timer.h"
#include "rom/ets_sys.h"
class Generator
{
private:
    ledc_timer_config_t m_timer;
    ledc_channel_config_t m_channel;
    uint32_t m_delay;
    uint32_t m_minimum;
    uint32_t m_maximum;
    uint32_t m_step_size;
    uint32_t m_step_timeout;
    uint32_t m_interval;

public:
    Generator(gpio_num_t pin);
    bool setup(uint32_t delay, uint32_t minimum, uint32_t maximum, uint32_t step_size, uint32_t step_timeout, uint32_t interval);
    ~Generator();
    bool generate();
};

#endif