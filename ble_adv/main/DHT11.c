/* DHT11 temperature sensor library

   Usage:
   		Set DHT PIN using  setDHTPin(pin) command
   		getFtemp(); this returns temperature in F

   Sam Johnston 
   October 2016
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "DHT11.h"


int humidity = 0;
int temperature = 0;
int Ftemperature = 0;

//int DHT_DATA[3] = [0,0,0]
int DHT_DATA[3] = {0,0,0};
int DHT_PIN = 4;

void setDHTPin(int PIN)
{
	DHT_PIN = PIN;
}
void errorHandle(int response)
{
	switch(response) {
	
		case DHT_TIMEOUT_ERROR :
			printf("DHT Sensor Timeout!\n");
		case DHT_CHECKSUM_ERROR:
			printf("CheckSum error!\n");
		case DHT_OKAY:
			break;
		default :
			printf("Dont know how you got here!\n");
	}
	temperature = 0;
	humidity = 0;
			
}
void sendStart()
{
  //Send start signal from ESP32 to DHT device
  gpio_set_direction(DHT_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level(DHT_PIN,0);
  //vTaskDelay(36 / portTICK_RATE_MS);
  ets_delay_us(22000);
  gpio_set_level(DHT_PIN,1);
  ets_delay_us(43);
  gpio_set_direction(DHT_PIN, GPIO_MODE_INPUT);
}

int getData()
{
  //Variables used in this function
  int counter = 0;
  uint8_t bits[5];
  uint8_t byteCounter = 0;
  uint8_t cnt = 7;
  
  for (int i = 0; i <5; i++)
  {
  	bits[i] = 0;
  }
  
  sendStart();
  
  //Wait for a response from the DHT11 device
  //This requires waiting for 20-40 us 
  counter = 0;
  
  while (gpio_get_level(DHT_PIN)==1)
  {
      if(counter > 40)
      {
      	return DHT_TIMEOUT_ERROR;
      }	
      counter = counter + 1;
      ets_delay_us(1);
  }
  //Now that the DHT has pulled the line low, 
  //it will keep the line low for 80 us and then high for 80us
  //check to see if it keeps low
  counter = 0;
  while(gpio_get_level(DHT_PIN)==0)
  {
  	if(counter > 80)
  	{
  		return DHT_TIMEOUT_ERROR;
  	}
  	counter = counter + 1;
  	ets_delay_us(1);
  }
  counter = 0;
  while(gpio_get_level(DHT_PIN)==1)
  {
  	if(counter > 80)
  	{
  		return DHT_TIMEOUT_ERROR;
  	}
  	counter = counter + 1;
  	ets_delay_us(1);
  }
  // If no errors have occurred, it is time to read data
  //output data from the DHT11 is 40 bits.
  //Loop here until 40 bits have been read or a timeout occurs
  
  for(int i = 0; i < 40; i++)
  {
      //int currentBit = 0;
      //starts new data transmission with 50us low signal
      counter = 0;
      while(gpio_get_level(DHT_PIN)==0)
  	  {
  	  	if (counter > 55)
  	  	{
  	  	   return DHT_TIMEOUT_ERROR; 
  	  	}
  	  	counter = counter + 1;
  	  	ets_delay_us(1);
  	  }
  	  
  	  //Now check to see if new data is a 0 or a 1
      counter = 0;
      while(gpio_get_level(DHT_PIN)==1)
  	  {
  	  	if (counter > 75)
  	  	{
  	  	   return DHT_TIMEOUT_ERROR; 
  	  	}
  	  	counter = counter + 1;
  	  	ets_delay_us(1);
  	  }  	  
  	  //add the current reading to the output data
  	  //since all bits where set to 0 at the start of the loop, only looking for 1s
  	  //look for when count is greater than 40 - this allows for some margin of error
  	  if (counter > 40)
  	  {
  	  
  	  	bits[byteCounter] |= (1 << cnt);
  	  	
  	  }
  	  //here are conditionals that work with the bit counters
  	  if (cnt == 0)
  	  {
  	  	
  	  	cnt = 7;
  	  	byteCounter = byteCounter +1;
  	  }else{
  	  
  	  	cnt = cnt -1;
  	  } 	  
  }
  humidity = bits[0];
  temperature = bits[2];
  Ftemperature = temperature * 1.8 + 32;
  
  uint8_t sum = bits[0] + bits[2];
  
  if (bits[4] != sum)
  {
  	return DHT_CHECKSUM_ERROR;
  }
  //DHT_DATA = [temperature,Ftemperature, humidity];
  DHT_DATA[0] = temperature;
  DHT_DATA[1] = Ftemperature;
  DHT_DATA[2] = humidity;
  return DHT_DATA;
}

int getFtemp()
{
	int ftemp = 0;
	//int Data = getData();
	//ftemp = Data[1];
	getData();
	ftemp = DHT_DATA[1];
	return ftemp;	
}

int getTemp()
{
	int temp = 0;
	getData();
//	int Data = getData();
//	int Data[3] = getData();
	temp = DHT_DATA[0];
	return temp;
}

int getHumidity()
{
	int humid = 0;
//	int Data = getData();
	getData();
	//humid = Data[2];
	humid = DHT_DATA[2];
	return humid;
}

