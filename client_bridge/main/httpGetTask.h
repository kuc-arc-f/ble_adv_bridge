/* HTTP GET Example using plain POSIX sockets

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "sys/time.h"
#include "sdkconfig.h"
#include "esp_deep_sleep.h"
	
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

//static esp_err_t event_handler(void *ctx, system_event_t *event);
//static void initialise_wifi(void);
esp_err_t event_handler(void *ctx, system_event_t *event);
void initialise_wifi(void);

void http_get_task(void *pvParameters);
//void http_get_task(void *pvParameters, char *req_1);

void set_HttpBuff(char* src);

void set_requestBuff(char* src);
void http_execDeepSleep();

