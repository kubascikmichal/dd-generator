/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "AP/AP.h"
#include "HTTP_Server/HTTP_Server.h"

extern "C"
{
    void app_main(void);
}

void app_main(void)
{
    AP *ap = new AP("Detector", "123456789");
    HTTP_Server *server = new HTTP_Server();
    server->init();
    while (true)
    {
        
    }
}
