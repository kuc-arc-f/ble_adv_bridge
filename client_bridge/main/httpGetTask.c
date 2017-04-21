#include "httpGetTask.h"

/* Constants that aren't configurable in menuconfig */
EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;

const char *TAG_HTTP_TASK = "HTTP_GET";

#define EXAMPLE_WIFI_SSID "your-SSID"
#define EXAMPLE_WIFI_PASS "your-pass"

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "api.thingspeak.com"
#define WEB_PORT 80
#define WEB_URL "http://api.thingspeak.com"
	
#define mAPI_KEY "your-KEY"

char mHttpBuff[32];

static const char *REQUEST_2="User-Agent: esp-idf/1.0 esp32\n"
    "\n";
//
void set_HttpBuff(char* src){
    strcpy(mHttpBuff ,src);
}
	
//
esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

void initialise_wifi(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_WIFI_SSID,
            .password = EXAMPLE_WIFI_PASS,
        },
    };
    ESP_LOGI(TAG_HTTP_TASK, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

//
void http_get_task(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s, r;
    char recv_buf[64];

    while(1) {
    	//check-HttpBuff
    	if(strlen(mHttpBuff) >0){
	        xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
	                            false, true, portMAX_DELAY);
	        ESP_LOGI(TAG_HTTP_TASK, "Connected to AP");

	        int err = getaddrinfo(WEB_SERVER, "80", &hints, &res);

	        if(err != 0 || res == NULL) {
	            ESP_LOGE(TAG_HTTP_TASK, "DNS lookup failed err=%d res=%p", err, res);
	            vTaskDelay(1000 / portTICK_PERIOD_MS);
	            continue;
	        }
	        addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
	        ESP_LOGI(TAG_HTTP_TASK, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

	        s = socket(res->ai_family, res->ai_socktype, 0);
	        if(s < 0) {
	            ESP_LOGE(TAG_HTTP_TASK, "... Failed to allocate socket.");
	            freeaddrinfo(res);
	            vTaskDelay(1000 / portTICK_PERIOD_MS);
	            continue;
	        }
	        ESP_LOGI(TAG_HTTP_TASK, "... allocated socket\r\n");

	        if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
	            ESP_LOGE(TAG_HTTP_TASK, "... socket connect failed errno=%d", errno);
	            close(s);
	            freeaddrinfo(res);
	            vTaskDelay(4000 / portTICK_PERIOD_MS);
	            continue;
	        }
	        ESP_LOGI(TAG_HTTP_TASK, "... connected");
	        freeaddrinfo(res);
	        //reqest        
			char sReq1[64+1];
			char sReq2[64+1];
			char sBuff[128+1];
			sprintf(sReq1, "GET %s/update?key=%s&field1=%s  HTTP/1.1\n" ,  WEB_URL, mAPI_KEY , mHttpBuff );
			sprintf(sReq2, "Host:  %s \n" ,  WEB_SERVER);
			sprintf(sBuff, "%s%s%s" ,  sReq1, sReq2, REQUEST_2 );
			printf("%s", sBuff);
	    	if (write(s, sBuff, strlen(sBuff)) < 0) {
	            ESP_LOGE(TAG_HTTP_TASK, "... socket send failed");
	            close(s);
	            vTaskDelay(4000 / portTICK_PERIOD_MS);
	            continue;
	        }
	        ESP_LOGI(TAG_HTTP_TASK, "... socket send success");

	        /* Read HTTP response */
	        do {
	            bzero(recv_buf, sizeof(recv_buf));
	            r = read(s, recv_buf, sizeof(recv_buf)-1);
	            for(int i = 0; i < r; i++) {
	                putchar(recv_buf[i]);
	            }
	        } while(r > 0);

	        ESP_LOGI(TAG_HTTP_TASK, "... done reading from socket. Last read return=%d errno=%d\r\n", r, errno);
	        close(s);
    		//reset-buff
    		mHttpBuff[0]='\0';
    		/*
	        for(int countdown = 60; countdown >= 0; countdown--) {
	            ESP_LOGI(TAG_HTTP_TASK, "%d... ", countdown);
	            ESP_LOGI(TAG_HTTP_TASK, "mHttpBuff=%s\n ", mHttpBuff );
	            vTaskDelay(1000 / portTICK_PERIOD_MS);
	        }
    		*/
	        ESP_LOGI(TAG_HTTP_TASK, "Starting again!");    		
	        return;
    	}
         vTaskDelay(1000 / portTICK_PERIOD_MS);
    } //end_while
}

