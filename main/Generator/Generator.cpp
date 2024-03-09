#include "Generator.h"
Generator::Generator(gpio_num_t pin)
{
    this->m_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,   // timer mode
        .duty_resolution = LEDC_TIMER_8_BIT, // resolution of PWM duty
        .timer_num = LEDC_TIMER_0,           // timer index
        .freq_hz = 100000,                   // frequency of PWM signal
        .clk_cfg = LEDC_AUTO_CLK,            // Auto select the source clock
    };
    ledc_timer_config(&m_timer);
    this->m_channel = {
        .gpio_num = pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
    };
    ledc_channel_config(&m_channel);
    ledc_fade_func_install(0);
}

Generator::~Generator()
{
}

bool Generator::setup(uint32_t delay, uint32_t minimum, uint32_t maximum, uint32_t step_size, uint32_t step_timeout, uint32_t interval)
{
    this->m_delay = delay;
    this->m_minimum = minimum;
    this->m_maximum = maximum;
    this->m_step_size = step_size;
    this->m_step_timeout = step_timeout;
    this->m_interval = interval;
    printf("%ld\n\r%ld\n\r%ld\n\r%ld\n\r%ld\n\r%ld\n\r", this->m_delay, this->m_minimum, this->m_maximum,
           this->m_step_size, this->m_step_timeout, this->m_interval);
    return true;
}

bool Generator::generate()
{
    printf("Generating\n\r");
    ets_delay_us(this->m_delay * 1000);
    ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, this->m_minimum);
    ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, (uint32_t)128, 0);
    uint32_t actual = this->m_minimum;
    uint64_t start = esp_timer_get_time();
    bool increment = true;
    while ((esp_timer_get_time() - start) < (this->m_interval * 1000000))
    {
        ets_delay_us(this->m_step_timeout * 1000);
        if (((actual + this->m_step_size) > this->m_maximum))
        {
            increment = false;
        }
        else if ((actual - this->m_step_size) < this->m_minimum)
        {
            increment = true;
        }
        if (increment)
        {
            actual += this->m_step_size;
        }
        else
        {
            actual -= this->m_step_size;
        }
        ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, actual);
    }
    ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 0);
    return true;
}