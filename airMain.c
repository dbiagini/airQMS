#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "./ssd1331/ssd1331.h"
#include "./pms5003/pms5003.h"
#include "./dht11/dht.h"

#define ERROR_MAX 5
#define BUTTON_PIN 3

#define PM25_LOW 35
#define PM25_HIGH 75

#define PM1_LOW 35
#define PM1_HIGH 75

#define PM10_LOW 50
#define PM10_HIGH 110

#define PMS_SIM 0

#define READ_TEMP 1

char value[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
int pms[3] = {0,0,0};
int dht_data[5] = { 0, 0, 0, 0, 0};

volatile int numberOfPresses = 0;

void btnInterrupt(){
	numberOfPresses++;
	numberOfPresses %= 4;
}

int pmsSim(int* values){
   int error = 0;
   if( values!=NULL ){
	unsigned int iseed = (unsigned int)time(NULL);
	srand(iseed);
	values[0] = rand() % 100;	
	values[1] = rand() % 100;
	values[2] = rand() % 100;
   }
   else
	   error =1;
   return(error);

}

void drawPage(int page){

	time_t now;
    	struct tm *timenow;
	//draw time//
	time(&now);
        timenow = localtime(&now);
	
	char hour[3] = {(char) value[timenow->tm_hour / 10], (char) value[(timenow->tm_hour % 10)], '\0'}; 
	char minutes[3] = {(char) value[timenow->tm_min / 10], (char) value[(timenow->tm_min % 10)], '\0'};
	
	#ifdef READ_TEMP
	////temp and humidity
	char temperature[3] = {(char) value[dht_data[2]/10],(char) value[dht_data[2] % 10], '\0' };
	char humidity[3] = {(char) value[dht_data[0]/10],(char) value[dht_data[0] % 10], '\0' };
	#endif

	SSD1331_string(0, 0, hour, 12,1, BLUE);
	SSD1331_string(12, 0, ":", 12, 1, RED);
	SSD1331_string(16, 0, minutes,12, 1, BLUE);

	
  	//color coding values//
 	int colors[3] = {GREEN, GREEN, GREEN};
	if ((pms[0] >= PM1_LOW) &&  (pms[0] < PM1_HIGH)) colors[0] = YELLOW;
	else if(pms[0] >= PM1_HIGH) colors[0] = RED;
	if ((pms[1] >= PM25_LOW) &&  (pms[1] < PM25_HIGH)) colors[1] = YELLOW;
	else if(pms[1] >= PM25_HIGH) colors[1] = RED;
	if ((pms[2] >= PM10_LOW) &&  (pms[2] < PM10_HIGH)) colors[2] = YELLOW;
	else if(pms[2] >= PM10_HIGH) colors[2] = RED;

	switch(page){
	
		case 0:
			//print temp humidity//
			
			#ifdef READ_TEMP
			////temp and humidity
			SSD1331_string(32, 0, temperature, 12,1, YELLOW);
			SSD1331_string(44, 0, "C", 12, 1, YELLOW);
			SSD1331_string(56, 0, humidity, 12,1, WHITE);
			SSD1331_string(68, 0, "%", 12, 1, WHITE);
			#endif

			SSD1331_char1616(0, 13, value[pms[0] / 10], colors[0]);
			SSD1331_char1616(16, 13, value[pms[0] % 10 ], colors[0]);
			SSD1331_string(40, 13, "PM 1.0", 12, 1, WHITE); 
			//printf("pm 1.0 %d \n", pms[0]);

			SSD1331_char1616(0, 31, value[pms[1] / 10], colors[1]);
			SSD1331_char1616(16, 31, value[pms[1] % 10 ], colors[1]);
			SSD1331_string(40, 31, "PM 2.5", 12, 1, WHITE); 
			
			SSD1331_char1616(0, 49, value[pms[2] / 10], colors[2]);
			SSD1331_char1616(16, 49, value[pms[2] % 10 ], colors[2]);
			SSD1331_string(40, 49, "PM 10", 12, 1, WHITE); 
		
			//printf("pm 2.5 %d \n", pms[1]);
			//printf("pm 10 %d \n", pms[2]);

			break;
		case 1:
			//draw pm 2.5//
			SSD1331_char3216(18, 15, value[pms[1] / 10], colors[1]);
			SSD1331_char3216(42, 15, value[pms[1] % 10 ], colors[1]);
			SSD1331_string(18, 49, "ug/m3 PM 2.5", 12, 1, WHITE); 

			break;
		case 2:
			//draw pm 10.0//
			SSD1331_char3216(18, 15, value[pms[2] / 10], colors[2]);
			SSD1331_char3216(42, 15, value[pms[2] % 10 ], colors[2]);
			SSD1331_string(18, 49, "ug/m3 PM 10", 12, 1, WHITE); 
			break;
		#ifdef READ_TEMP
		case 3:
			//print temperature and humidity
			SSD1331_char1616(0, 18, value[dht_data[0] / 10], WHITE);
			SSD1331_char1616(16, 18, value[dht_data[0] % 10 ], WHITE);
			SSD1331_string(32, 18, " %", 16, 1, WHITE); 

			SSD1331_char1616(0, 40, value[dht_data[2] / 10], WHITE);
			SSD1331_char1616(16, 40, value[dht_data[2] % 10 ], WHITE);
			SSD1331_string(32, 40, ".", 16, 1, WHITE); 
			SSD1331_char1616(40, 40, value[dht_data[3] / 10 ], WHITE);
			SSD1331_char1616(56, 40, value[dht_data[3] % 10 ], WHITE);			
			SSD1331_string(72, 40, " C", 16, 1, WHITE); 


			//SSD1331_char3216(18, 15, value[dht_data[2] / 10], colors[1]);
			//SSD1331_char3216(42, 15, value[dht_data[1] % 10 ], colors[1]);
			//SSD1331_string(18, 49, "ug/m3 PM 2.5", 12, 1, WHITE); 
			
		#endif
		default:
			break;
	}

}

int main(int argc, char **argv)
{
    //setup the sensor//
    int status = pms_init();
    if (status != UART_OK) {
        output_uart_code(status);
        return 1;
    }

    int err_cnt = 0;
    PMS5003_DATA d;
	
    if(wiringPiSetup() < 0)
    {
    	printf("airQMS: Unable to setup Wiring PI\n");
	return -1;
    }
    
    if( wiringPiISR (BUTTON_PIN, INT_EDGE_RISING, &btnInterrupt) < 0) {
    	printf("airQMS: unable to setup ISR for button");
	return -1;
    }
    printf("airQMS: OLED run Press Ctrl + C to exit.\n");
    SSD1331_begin();
    SSD1331_clear();

    while(1)
    {
        
	//get sensor status//    
	#if PMS_SIM
	int status = pmsSim(pms);
	printf("airQMS: Sensor simulation started. \n");
	#endif
	status = read_pms5003_data(&d);
        if (status != UART_OK) {
            output_uart_code(status);
            err_cnt++;
            if (err_cnt > ERROR_MAX) {
		printf("airQMS: uart %d errors, stopping!. \n", err_cnt);
		break;
            } else {
		for (int i = 0; i < sizeof(pms); i++){
			    pms[i] = 0;
		   }
	    }
        } else {

		err_cnt =0; //reset max retries
		pms[0]= d.pm1at;
		pms[1]= d.pm2_5at;
		pms[2]= d.pm10at;
	}
	
	#ifdef READ_TEMP
	int dht_temp[5] = {0, 0, 0, 0, 0};
	if(read_dht11_dat(dht_temp)){
	
		//printf("cannot read DHT data");	

	}else{
	
		memcpy(dht_data, dht_temp, sizeof(dht_data));
	}
	#endif
        //SSD1331_string(0, 52, "MUSIC", 12, 0, WHITE); 
        //SSD1331_string(64, 52, "MENU", 12, 1, WHITE); 
        	//printf("pm 2.5 %d \n", pms[1]);
		//printf("pm 10 %d \n", pms[2]);
	drawPage(numberOfPresses);
	SSD1331_display();
		//printf("%d\n", numberOfPresses);

	
	delay(1000);
    }

    SSD1331_clear();
    pms_close();

    return 0;
}
